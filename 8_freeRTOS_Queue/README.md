## 8_freeRTOS_Queue

### task 통신?

- https://m.blog.naver.com/eziya76/221832659123

- https://m.cafe.daum.net/easyflow/F13G/64

  https://www.freertos.org/a00116.html
  https://arsviator.blogspot.com/2016/01/freertos-2.html
  - xQueueCreate()
  - xQueueSendToBack()
  - xQueueReceive()

- message / mail
  - message : get / put / peek 등 API 제공
  - mail : 메모리 블록을 alloc / free 기능 추가





- osEvent osMessageGet (osMessageQId queue_id, uint32_t millisec)
  



- ```c
  /* USER CODE BEGIN PTD */
  typedef struct
  {
      uint8_t buf[8];
      uint8_t idx;
  } _Message;
  
  
  /* USER CODE BEGIN Variables */
  
  osThreadId SenderHandle;
  osThreadId ReceiverHandle;
  osMessageQId Queue1Handle;
  
  
  ...
      
        /* add threads, ... */
    
    osMessageQDef(Queue1, 32, _Message);
    Queue1Handle = osMessageCreate(osMessageQ(Queue1), NULL);
    
    osThreadDef(Sender, SenderTask, osPriorityNormal, 0, 128);
    SenderHandle = osThreadCreate(osThread(Sender), NULL);
    
    osThreadDef(Receiver, ReceiverTask, osPriorityNormal, 0, 128);
    ReceiverHandle = osThreadCreate(osThread(Receiver), NULL);
  
  
  /* USER CODE BEGIN Application */
  
  void SenderTask(void const * argument)
  {
    /* USER CODE BEGIN SenderTask */
      _Message msg;
      msg.buf[0] = 0xAA;
      msg.buf[1] = 0xBB;
      msg.idx = 0;
  
      /* Infinite loop */
      for (;;)
      {
          printf("Sender enqueues\r\n");
          osMessagePut(Queue1Handle, &msg, 100); //enqueue
          msg.idx++;
          printf("Sender delays for a sec.\r\n");
          osDelay(1000);
      }
    /* USER CODE END SenderTask */
  }
  
  
  void ReceiverTask(void const * argument)
  {
    /* USER CODE BEGIN ReceiverTask */
      _Message *pMsg;
      osEvent retVal;
  
      /* Infinite loop */
      for (;;)
      {
          printf("Receiver is trying to dequeue\n");
          retVal = osMessageGet(Queue1Handle, 500); //dequeue
          if(retVal.status == osEventMessage)
          {
              pMsg = retVal.value.p;
              printf("Receiver received: msg.buf[0]=0x%X, msg.idx=%u\n", pMsg->buf[0]  , pMsg->idx);
          }
      }
    /* USER CODE END ReceiverTask */
  }
  
  ```

- 