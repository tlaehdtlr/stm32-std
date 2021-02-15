## 5_TIM_mode_3

### Input Capture Mode

- 다른 타이머의 rising/falling edge 를 캡쳐하여 frequency, duty ratio 확인

- Core i7, 7세대에서는 잘 안 되었지만 간혹 캡쳐가 되었음 (xeon, ram 64에서는 정말 잘 됨. 분석 필요)

#### MX

- TIM1
  ![image-20210215092053371](C:\Users\JJW_N-771\Desktop\stmpjt\5_TIM_mode_3\README.assets\image-20210215092053371.png)
- TIM15
  ![image-20210215092112830](C:\Users\JJW_N-771\Desktop\stmpjt\5_TIM_mode_3\README.assets\image-20210215092112830.png)
- UART2
  ![image-20210215092136512](C:\Users\JJW_N-771\Desktop\stmpjt\5_TIM_mode_3\README.assets\image-20210215092136512.png)



#### IDE

- ```c
  /* USER CODE BEGIN 0 */
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
  
  
  bool ch1done = false;
  bool ch2done = false;
  
  void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
  {
    if(htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      ch1done = true;
    }
  
    if(htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      ch2done = true;
    }
  }
  ```

- ```c
  printf("start \r\n");
    while (1)
    {
      /* USER CODE END WHILE */
  
      /* USER CODE BEGIN 3 */
      if(ch1done)
      {
        printf("ch1 \r\n");
        ch1done = false;
      }
  
      if(ch2done)
      {
        printf("ch2 \r\n");
        ch2done = false;
      }
  
    }
  ```



#### 결과

- 콜백함수가 호출되는 것은 확인됨 (낮은 확률로 성공)
  ![image-20210215091410357](C:\Users\JJW_N-771\Desktop\stmpjt\5_TIM_mode_3\README.assets\image-20210215091410357.png)