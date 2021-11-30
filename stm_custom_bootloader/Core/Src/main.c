/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "usb_device.h"
#include "bootloader.h"
#include "flash.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if 0
// int _read(int file, char* p, int len)
// {
//   HAL_UART_Receive(&huart2, (uint8_t*)&p, len, 10);
//   return len;
// }

int _write(int file, char* p, int len)
{
HAL_UART_Transmit_IT(&huart2, (uint8_t*)&p, len);
return len;
}
#else
int __io_putchar(int ch)
{
	// if(HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 10) != HAL_OK)
	// 	return -1;
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 1);
	return ch;
}
#endif


/*
    DFU mode
*/
typedef  void (*pFunction)(void);

pFunction JumpToApplication;
uint32_t JumpAddress;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */


  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("\r\n\r\n stm custom bootloader start \r\n");
  HAL_Delay(1000);


  /*
    Test if user code is programmed starting from address USBD_DFU_APP_DEFAULT_ADD
    https://www.os4all.com/69
    stack pointer
   */
  if (((*(__IO uint32_t *) USBD_DFU_APP_DEFAULT_ADD - 1) & 0x2FFC0000) == 0x20000000)
  {
    printf("valid app \r\n");

    /*  roll back when DFU fail caused by version, key etc.  */
    bootloader_check_DFU();

    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
    {
      printf("pin reset APP start \r\n");
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
      HAL_Delay(100);
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t *) (USBD_DFU_APP_DEFAULT_ADD + 4);
      // JumpAddress = *(__IO uint32_t *) (USBD_DFU_APP_DEFAULT_ADD);
      JumpToApplication = (pFunction) JumpAddress;

      HAL_UART_MspDeInit(&huart2);
      MX_GPIO_DeInit();
      HAL_RCC_DeInit();
      HAL_DeInit();

	    SysTick->CTRL = 0;
	    SysTick->LOAD = 0;
	    SysTick->VAL  = 0;

      // SCB->VTOR = USBD_DFU_APP_DEFAULT_ADD;

      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t *) USBD_DFU_APP_DEFAULT_ADD);

      JumpToApplication();
    }
    else
    {
      /* back up code */

      printf("ready to dfu \r\n");
      bootloader_copy_image(USBD_DFU_APP_DEFAULT_ADD, ADDR_FLASH_COPY, APP_NUMS_SECTOR);
      HAL_Delay(100);
    }
  }
  else
  {
    /*  whether there is a code at back up region  */
    if (((*(__IO uint32_t *) ADDR_FLASH_COPY - 1) & 0x2FFC0000) == 0x20000000)
    {
      printf("DFU fail during erase , roll back \r\n");
      bootloader_rollback_version();
    }
    printf("invalid app \r\n");
  }

  MX_USB_DEVICE_Init();
  printf("DFU mode \r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
//   if (GPIO_Pin == GPIO_PIN_9)
//   {
//     USB_HAL_GPIO_EXTI_Callback(GPIO_Pin);
//   }
//   else if (GPIO_Pin == GPIO_PIN_13)
//   {
//     HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
//   }
// }


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/