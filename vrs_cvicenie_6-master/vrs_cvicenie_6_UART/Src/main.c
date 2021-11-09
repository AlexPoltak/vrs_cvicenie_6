/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>


void SystemClock_Config(void);

void process_serial_data(uint8_t ch);

int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART2_UART_Init();

  USART2_RegisterCallback(process_serial_data);
  char tx_dataOFF[]={'l','e','d','O','F','F'};
  char tx_dataON[]={'l','e','d','O','N'};
  int i = 0;
  while (1)
	    {
	       if(!LL_GPIO_IsOutputPinSet(GPIOB, LL_GPIO_PIN_3)){
	           LL_USART_TransmitData8(USART2, tx_dataOFF[i]);
	           LL_mDelay(50);
	           i = i+1;
	           if(i == 6){
	              i = 0;
	              LL_mDelay(5000);
	           }
	      }
	       else{
	           LL_USART_TransmitData8(USART2, tx_dataON[i]);
	           LL_mDelay(50);
	           i = i+1;
	           if(i == 5){
	              i = 0;
	              LL_mDelay(5000);
	           }
	       }
	    }

  }


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
}


void process_serial_data(uint8_t ch)
{
	static uint8_t count = 0;

	switch(ch)
	    {
	        case 'l':
	        	count=1;
	            break;

	        case 'e':
	        	if(count==1){count=2;}
	        	else{count=0;}
	            break;
	        case 'd':
	        	if(count==2){count=3;}
	        	else{count=0;}
	        	break;
	        case 'O':
	        	if(count==3){count=4;}
	        	else{count=0;}
	        	break;
	        case 'N':
	        	if(count==4){count=0;
				LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);}
	        	else{count=0;}
	        	break;

	        case 'F':
	        	if(count==4){count=5;}
	      	    if(count==5){count=0;
				LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);}
	        	else{count=0;}
	      	    break;

	        // operator doesn't match any case constant +, -, *, /
	        default:count=0;    }

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
