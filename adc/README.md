

#### datasheet

- ![image-20210316221340798](README.assets/image-20210316221340798.png)
- ![image-20210316232112588](README.assets/image-20210316232112588.png)
- ![image-20210316221543308](README.assets/image-20210316221543308.png)

#### MX

- ADC temperature sensor 그거 활성화시키자
- 나 지금 in1 인가 그거 활성화시켰을건데

#### IDE

- https://electronics.stackexchange.com/questions/324321/reading-internal-temperature-sensor-stm32

  - ```c
    /* 지금 식에서는 필요없음*/
    #define TEMP_SENSOR_AVG_SLOPE_MV_PER_CELSIUS                    2.5f
    #define TEMP_SENSOR_VOLTAGE_MV_AT_25                            760.0f
    #define ADC_REFERENCE_VOLTAGE_MV                                3300.0f
    #define ADC_MAX_OUTPUT_VALUE                                    4096.0f
    
    /* 0x1FFF 75CA - 0x1FFF 75CB */
    #define TEMP130_CAL_VALUE                                       ((uint16_t*)((uint32_t)0x1FFF75CA))
    /* 0x1FFF 75A8 - 0x1FFF 75A9 */
    #define TEMP30_CAL_VALUE                                        ((uint16_t*)((uint32_t)0x1FFF75A8))
    #define TEMP130                                                 130.0f
    #define TEMP30                                                  30.0f
    
    
    
    uint16_t temperature;
    float sensorValue;
    float adcCalValue30 = (float)(*TEMP30_CAL_VALUE);
    float adcCalValue110 = (float)(*TEMP110_CAL_VALUE);
    
    /* init에 있나 밑에 저거 맞는지 확이내야함 */
    __HAL_ADC_ENABLE(&hadc1);
    // Disable Vbat signal from input channel and wake up temp sensor from power down mode
    ADC->CCR |= ADC_CCR_TSVREFE;
    ADC->CCR &= ~ADC_CCR_VBATE ;
    
    /* while 문 안에 넣어 */
    HAL_ADC_Start(&hadc1);
    if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
    {
        sensorValue = (float)HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        temperature = (uint16_t)((TEMP110 - TEMP30) / ((float)(*TEMP130_CAL_VALUE) - (float)(*TEMP30_CAL_VALUE)) * (sensorValue - (float)(*TEMP30_CAL_VALUE)) + TEMP30);
    }
    ```

  - 밑에 쓸거 정리

- ```c
  /* 0x1FFF 75CA - 0x1FFF 75CB */
  #define TEMP130_CAL_VALUE                                       ((uint16_t*)((uint32_t)0x1FFF75CA))
  /* 0x1FFF 75A8 - 0x1FFF 75A9 */
  #define TEMP30_CAL_VALUE                                        ((uint16_t*)((uint32_t)0x1FFF75A8))
  #define TEMP130                                                 130.0f
  #define TEMP30                                                  30.0f
  
  
  uint16_t temperature;
  float sensorValue;
  float adcCalValue30 = (float)(*TEMP30_CAL_VALUE);
  float adcCalValue110 = (float)(*TEMP110_CAL_VALUE);
  
  /* init에 있나 밑에 저거 맞는지 확이내야함 */
  __HAL_ADC_ENABLE(&hadc1);
  // Disable Vbat signal from input channel and wake up temp sensor from power down mode
  ADC->CCR |= ADC_CCR_TSVREFE;
  ADC->CCR &= ~ADC_CCR_VBATE ;
  
  /* while 문 안에 넣어 */
  HAL_ADC_Start(&hadc1);
  if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
  {
      sensorValue = (float)HAL_ADC_GetValue(&hadc1);
      HAL_ADC_Stop(&hadc1);
      temperature = (uint16_t)((TEMP130 - TEMP30) / ((float)(*TEMP130_CAL_VALUE) - (float)(*TEMP30_CAL_VALUE)) * (sensorValue - (float)(*TEMP30_CAL_VALUE)) + TEMP30);
  }
  ```

  - 답변 보면 더 정확하게 하려고 Vref 로 보정하는 느낌?
    ![image-20210316233113543](README.assets/image-20210316233113543.png)

- 아님 이 공식?

  ```
  Temperature (in 'C) = {(V25 - VSENSE) / Avg_Slope} + 25.
  V25 = 1.43
  Avg_Slope = 4.3
  ```

  ![image-20210316224903943](README.assets/image-20210316224903943.png)





---

```c
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
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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
#if 0
const float AVG_SLOPE = 2.5E-03;
//const float V30 = 0.76;
const float V30 = 0.76;
const float ADC_TO_VOLT = 3.3/4096;
#else
const float AVG_SLOPE = 4.3;
const float V30 = 1.43;
const float ADC_TO_VOLT = 3.3/4096;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

#if 0
  uint16_t adc1;

  float vSense, temp;
  /* start calibration */
  if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
  {
      Error_Handler();
  }
  /* start the conversion process */
  if (HAL_ADC_Start(&hadc1) != HAL_OK)
  {
      Error_Handler();
  }

#else
   uint16_t value;
   uint16_t vout;
   float temp;
#endif
   printf("STM on!! \r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#if 0
    HAL_ADC_PollForConversion(&hadc1, 100);
    adc1 = HAL_ADC_GetValue(&hadc1);
    //printf("ADC1_Temperature : %d \n", adc1);

//    vSense = adc1 * ADC_TO_VOLT;
    printf("sense : %f \r\n", vSense);
//    temp = (V30-vSense) / AVG_SLOPE + 25.0;
    temp = (uint16_t)(((V30 * 1000.0 - (float)adc1 * 0.8) / AVG_SLOPE) + 25.0);

    printf("ADC1, Temp : %d , %f \r\n", adc1, temp);
    HAL_Delay(1000);
#else

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    value = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

//    vout = 3000 * value / 4096;
//    temp = (float)vout*0.8;
//    temp = (uint16_t)(((V30 * 1000.0 - (float)value * 0.8) / AVG_SLOPE) + 25.0);
    temp = (uint16_t)((V30-(float)(value * ADC_TO_VOLT)) / AVG_SLOPE + 25.0);


    printf("temp : %f \r\n", temp);
    HAL_Delay(1000);
#endif

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
  RCC_OscInitStruct.PLL.PLLN = 40;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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

```

