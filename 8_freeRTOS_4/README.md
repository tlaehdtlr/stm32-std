## 8_freeRTOS_4

- timer, Interrupt (버튼이나 shell로?)



### Interrupt

- EXTI(버튼)으로  queue 통신
- interrupt 를 위해 non blocking 의 FromISR 함수를 써준다



#### MX

- GPIO EXTI, NVIC, freeRTOS



#### IDE

- main.c

```c
/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_13)
  {
    GPIO_PinState btn_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
    uint8_t _btn_state;
    _btn_state = btn_state == GPIO_PIN_RESET? 0:1;
    vButton_QueueSendFromISR(&_btn_state);   
  }
}
```

- freertos.h

```c
void vButton_QueueSendFromISR(uint8_t* pucButtonState);
```

- freertos.c

```c
/* USER CODE BEGIN Variables */
TaskHandle_t xButtonTaskHandle = NULL;
QueueHandle_t xButtonQueueHandle = NULL;


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void vButton_QueueSendFromISR(uint8_t* pucButtonState);
void vButton_Task();


...

/* create queue for button task */
  xButtonQueueHandle = xQueueCreate(4, sizeof(uint8_t));
  /* be viewable in a RTOS kernel aware debugger */
  vQueueAddToRegistry(xButtonQueueHandle, "buttonQueue");
  /* create button task */
  xTaskCreate(vButton_Task, "buttonTask", configMINIMAL_STACK_SIZE, NULL, 1, &xButtonTaskHandle);

...

void vButton_QueueSendFromISR(uint8_t* pucButtonState)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;  
  if (xButtonQueueHandle != NULL)
  {
    xQueueSendFromISR(xButtonQueueHandle, pucButtonState, &xHigherPriorityTaskWoken);
  }
}

void vButton_Task()
{
  printf("On!! \r\n");  
  uint8_t btnCtl;

  for (;;)
  {

    if (xQueueReceive(xButtonQueueHandle, &btnCtl, 1000)==pdTRUE)
    {
        if (btnCtl == 0)
        {
          printf("pull up ! \r\n");
        }
        else
        {
          printf("press down ! \r\n");
        }
    }
    else
    {
      printf("empty queue \r\n");
    }
  }
}

```





### Timer

- https://mcuoneclipse.com/2018/05/27/tutorial-understanding-and-using-freertos-software-timers/

- ```c
  timerHndl1Sec = xTimerCreate(
        "timer1Sec", /* name */
        pdMS_TO_TICKS(1000), /* period/time */
        pdTRUE, /* auto reload */
        (void*)0, /* timer ID */
        vTimerCallback1SecExpired); /* callback */
  ```

- 



