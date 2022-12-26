/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include <stdio.h>
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

#define BUF_LEN     30


int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1);
	return ch;
}

uint8_t cs_pin = 0;
uint8_t spi_rx = 0;
uint8_t m_spi_rx[255] = {0,};
uint8_t spi_rx_len = 2;
// uint8_t spi_rx_len = 255;
uint8_t m_spi_tx[255] = {0,};
uint8_t spi_tx_len = 1;

// const uint16_t BUF_LEN = 255;
uint8_t buf_rx[BUF_LEN] = {0,};
uint8_t buf_tx[BUF_LEN] = {0,};

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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  printf("\r\n ============= spi slave start =============\r\n");
  memset(m_spi_rx, 0, spi_rx_len);
  memset(m_spi_tx, 0, spi_tx_len);
  // m_spi_tx[0] = 0x11;
  m_spi_tx[0] = 0x02;
  m_spi_tx[1] = 0x06;
  HAL_SPI_Receive_DMA(&hspi1, m_spi_rx, spi_rx_len);
  // HAL_SPI_Receive_DMA(&hspi1, m_spi_rx, spi_rx_len);
  uint8_t tx_flag = 0;

  buf_tx[0] = 0x7e;
  buf_tx[BUF_LEN-1] = 0x70;
  HAL_UART_Receive_IT(&huart2, buf_rx, BUF_LEN);
  while (1)
  {
    switch (cs_pin)
    {
      case 1:
      {
        cs_pin = 0;
        printf("cs pin rising \r\n");
      }
        break;
      case 2:
      {
        cs_pin = 0;
        printf("cs pin falling \r\n");
        printf("spi ready \r\n");
      }
        break;
      default:
        break;
    }

    switch (spi_rx)
    {
      case 1:
      {
        spi_rx = 0;
        tx_flag = 1;
        // spi_rx = 2;
        printf("spi_rx : %d %d %d %d \r\n", m_spi_rx[0], m_spi_rx[1], m_spi_rx[2], m_spi_rx[3]);
        // HAL_SPI_Receive_DMA(&hspi1, m_spi_rx, spi_rx_len);
      }
        break;
      default:
        break;
    }
    if (tx_flag)
    {
      tx_flag = 0;
      // printf("spi_tx : %d %d %d %d \r\n", m_spi_tx[0], m_spi_tx[1], m_spi_tx[2], m_spi_tx[3]);
      // HAL_SPI_Transmit_DMA(&hspi1, m_spi_tx, spi_tx_len);
      // HAL_SPI_Transmit_DMA(&hspi1, m_spi_tx, 2);
    }

    // HAL_Delay(1);

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_4)
  {
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET)
    {
      cs_pin = 1;
    }
    else
    {
      HAL_SPI_TransmitReceive_DMA(&hspi1, m_spi_tx, m_spi_rx, spi_rx_len);
      cs_pin = 2;
    }
  }
  else
  {
    printf("button \r\n");
    HAL_UART_Transmit_IT(&huart2, buf_tx, BUF_LEN);
  }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  spi_rx = 1;
  HAL_SPI_Receive_DMA(&hspi1, m_spi_rx, spi_rx_len);
  // if (m_spi_rx[0]==0x06)
  // {
  //   HAL_SPI_Receive_DMA(&hspi1, m_spi_rx, spi_rx_len);
  //   return ;
  // }
  // else if (m_spi_rx[0]==0x05)
  // {
  //   // HAL_SPI_Transmit_DMA(&hspi1, m_spi_tx, 1);
  //   HAL_SPI_TransmitReceive_DMA(&hspi1, m_spi_tx, m_spi_rx, spi_rx_len);
  // }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  // HAL_SPI_Receive_DMA(&hspi1, m_spi_rx, spi_rx_len);
  spi_rx = 1;
  HAL_SPI_TransmitReceive_DMA(&hspi1, m_spi_tx, m_spi_rx, spi_rx_len);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    HAL_UART_Receive_IT(&huart2, buf_rx, BUF_LEN);
    printf("uart rx int \r\n");
  }
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
