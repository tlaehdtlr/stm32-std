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
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h.>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#ifdef __GNUC__
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
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

#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
#define SLAVE_TX_SIZE (COUNTOF(slave_buffer_tx) - 1)
#define MASTER_TX_SIZE (COUNTOF(master_buffer_tx) -1)

/* Master, Slave Tx Buffer */
uint8_t master_buffer_tx[] = "STM32L412RB SPI Master Tx";
uint8_t slave_buffer_tx[] = "STM32L412RB SPI Slave TX";

/* Master, Slave Rx Buffer */
uint8_t master_buffer_rx[SLAVE_TX_SIZE] = {0,};
uint8_t slave_buffer_rx[MASTER_TX_SIZE] = {0,};

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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("Hi \r\n");
#if 0
  /* wait for the button event */
  while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != RESET)
  {
  }

  /* receive the data from SPI1(master) */
  if (HAL_SPI_Receive_DMA(&hspi2, (uint8_t*)slave_buffer_rx, MASTER_TX_SIZE) != HAL_OK)
  {
    Error_Handler();
  }

  /* send the data to SPI2(slave) */
  if (HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)master_buffer_tx, MASTER_TX_SIZE) != HAL_OK)
  {
    Error_Handler();
  }

  /* check the SPI tx, rx state */
  while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY)
  {
  }

  while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
  {
  }

  /* send the data to SPI1(master) */
  if (HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)slave_buffer_tx, SLAVE_TX_SIZE) != HAL_OK)
  {
    Error_Handler();
  }

  /* receive the data from SPI2(slave) */
  if (HAL_SPI_Receive_DMA(&hspi1, (uint8_t*)master_buffer_rx, SLAVE_TX_SIZE) != HAL_OK)
  {
    Error_Handler();
  }

  /* check the SPI tx, rx state */
  while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY)
  {
  }

  while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
  {
  }

#else

  uint8_t master_buffer_tx[] = "STM32L412RB SPI Master Tx";
  uint8_t slave_buffer_tx[] = "STM32L412RB SPI Slave TX";

  /* Master, Slave Rx Buffer */
  uint8_t master_buffer_rx[SLAVE_TX_SIZE] = {0,};
  uint8_t slave_buffer_rx[MASTER_TX_SIZE] = {0,};


#endif


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
    HAL_Delay(500);
//    if (HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)master_buffer_tx, (uint8_t*)master_buffer_rx, MASTER_TX_SIZE+SLAVE_TX_SIZE) != HAL_OK)
    if (HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)master_buffer_tx, MASTER_TX_SIZE) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_DMA_STATE_CHANGE(&hspi1);
//    if (HAL_SPI_Receive_DMA(&hspi2, (uint8_t*)slave_buffer_tx, (uint8_t*)slave_buffer_rx, MASTER_TX_SIZE+SLAVE_TX_SIZE) != HAL_OK)
    if (HAL_SPI_Receive_DMA(&hspi2, (uint8_t*)slave_buffer_rx, MASTER_TX_SIZE) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_DMA_STATE_CHANGE(&hspi2);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 25;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_DMA_STATE_CHANGE(SPI_HandleTypeDef *hspi)
{
  hspi->State = HAL_SPI_STATE_READY;
}
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
