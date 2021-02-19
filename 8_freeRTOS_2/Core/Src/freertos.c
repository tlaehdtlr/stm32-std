/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
    uint8_t buf[8];
    uint8_t idx;
} _Message;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
QueueHandle_t Queue1Handle;
SemaphoreHandle_t SemaphoreHandle;


/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void SenderTask(void const * argument);
void ReceiverTask_1(void const * argument);
void ReceiverTask_2(void const * argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  Queue1Handle = xQueueCreate(8, sizeof(_Message));
  vSemaphoreCreateBinary(SemaphoreHandle);

  xTaskCreate((void*)SenderTask, "Sender" , configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate((void*)ReceiverTask_1, "Receiver1" , configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate((void*)ReceiverTask_2, "Receiver2" , configMINIMAL_STACK_SIZE, NULL, 1, NULL);



  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void SenderTask(void const * argument)
{
  /* USER CODE BEGIN SenderTask */
  _Message msg = { 0 };
  msg.buf[0] = 0xAA;
  msg.buf[1] = 0xBB;
  msg.idx = 0;

  /* Infinite loop */
  for (;;)
  {
    printf("Sender on! %X %d\r\n", msg.buf[0], msg.idx);
    xQueueSend(Queue1Handle, &msg, 100);
    msg.idx++;
    msg.buf[0]++;
    vTaskDelay(1000);
  }
  /* USER CODE END SenderTask */
}


void ReceiverTask_1(void const * argument)
{
  /* USER CODE BEGIN ReceiverTask */
  _Message Msg = { 0 };

  /* Infinite loop */
  for (;;)
  {
    printf("Receiver_1 on!! \r\n");

    if (xSemaphoreTake(SemaphoreHandle, 1000) == pdTRUE)
    {
      if (xQueueReceive(Queue1Handle, &Msg, 1000))
        {
          printf("Receiver 1 received: msg.buf[0]=0x%X, msg.idx=%d \r\n", Msg.buf[0], Msg.idx);
        }
      xSemaphoreGive(SemaphoreHandle);
    }
    else
    {
      printf("reject receiver_1 \r\n");
    }
  }
  /* USER CODE END ReceiverTask */
}

void ReceiverTask_2(void const * argument)
{
  /* USER CODE BEGIN ReceiverTask */
  _Message Msg = { 0 };

  /* Infinite loop */
  for (;;)
  {
    printf("Receiver_2 on!! \r\n");
    if (xSemaphoreTake(SemaphoreHandle, 800) == pdTRUE)
    {
      if (xQueueReceive(Queue1Handle, &Msg, 1000))
      {
        printf("Receiver 2 received: msg.buf[0]=0x%X, msg.idx=%d \r\n", Msg.buf[0], Msg.idx);
      }
      xSemaphoreGive(SemaphoreHandle);
    }
    else
    {
      printf("reject receiver_2 \r\n");
    }
  }
  /* USER CODE END ReceiverTask */
}


/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
