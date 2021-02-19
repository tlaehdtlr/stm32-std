## 8_freeRTOS_Queue

- https://m.blog.naver.com/eziya76/221832659123

### Task 통신 (queue)

- https://m.cafe.daum.net/easyflow/F13G/64

  https://www.freertos.org/a00116.html
  https://arsviator.blogspot.com/2016/01/freertos-2.html
  - xQueueCreate() 
    - QueueHandle_t 타입 큐핸들 반환
  - xQueueSendToBack()
  - xQueueReceive()


#### MX

- FreeRTOS, UART

#### IDE

- ```c
  /* USER CODE BEGIN PTD */
  typedef struct
  {
      uint8_t buf[8];
      uint8_t idx;
  } _Message;
  
  /* USER CODE BEGIN Variables */
  QueueHandle_t Queue1Handle;
  
  ...
  
    /* add threads, ... */
  
    Queue1Handle = xQueueCreate(8, sizeof(_Message));
  
    xTaskCreate((void*)SenderTask, "Sender" , configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate((void*)ReceiverTask, "Receiver" , configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  
  
  ...
      
      
  /* USER CODE BEGIN Application */
  
  void SenderTask(void const * argument)
  {
    /* USER CODE BEGIN SenderTask */
      _Message msg = {0};
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
  
  
  void ReceiverTask(void const * argument)
  {
    /* USER CODE BEGIN ReceiverTask */
      _Message pMsg = {0};
  
      /* Infinite loop */
      for (;;)
      {
          printf("Receiver on!! \r\n");
  
          if(xQueueReceive(Queue1Handle, &pMsg, 1500))
          {
              printf("Receiver received: msg.buf[0]=0x%X, msg.idx=%d \r\n", pMsg.buf[0]  , pMsg.idx);
          }        
      }
    /* USER CODE END ReceiverTask */
  }
  
  ```
  



### Semaphore

- 세마포어는 Task 끼리 또는 Task-Interrupt 간 동기화에 사용 (전역변수로 flag 사용해야했던 것을 안 해도된다)
- https://m.cafe.daum.net/easyflow/F13G/63
  - vSemaphoreCreateBinary()
    - SemaphoreHandle_t 타입 넣어주기
  - xSemaphoreTake()
  - xSemaphoreGive()
- semaphore 만들어서 2개의 receive task 가 겹치게 가져가지 않도록 해보자
  - receive 1 , receive 2 의 세마포어 가져올 때 기다리는 시간을 각각 1000, 800 으로 준다
  - receive2 는 receive1 의 세마포어를 가지고 있을 때, 세마포어를 가져오려하기 때문에 reject가 발생함



#### IDE

```c
/* USER CODE BEGIN PTD */
typedef struct
{
    uint8_t buf[8];
    uint8_t idx;
} _Message;

/* USER CODE BEGIN Variables */
QueueHandle_t Queue1Handle;
SemaphoreHandle_t SemaphoreHandle;

/* USER CODE BEGIN FunctionPrototypes */
void SenderTask(void const * argument);
void ReceiverTask_1(void const * argument);
void ReceiverTask_2(void const * argument);


...
    
    
      /* add threads, ... */
  Queue1Handle = xQueueCreate(8, sizeof(_Message));
  vSemaphoreCreateBinary(SemaphoreHandle);

  xTaskCreate((void*)SenderTask, "Sender" , configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate((void*)ReceiverTask_1, "Receiver1" , configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate((void*)ReceiverTask_2, "Receiver2" , configMINIMAL_STACK_SIZE, NULL, 1, NULL);


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

```



