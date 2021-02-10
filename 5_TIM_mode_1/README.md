## 5_TIM_mode_1

### Counter Mode

- CNT 가 ARR 에 의해 0 되면서 인터럽트



#### MX

- TIM6(basic timer), 1s 마다 인터럽트, LED toggle
  ![image-20210210091311286](C:\Users\JJW_N-771\Desktop\stmpjt\5_TIM_mode_1\README.assets\image-20210210091311286.png)

- NVIC, clock 설정



#### IDE

- timer interrupt 활성화, 80MHz, prescaler 8000, clock period 10000 -> 1s 당 콜백 불림


  ```c
    /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim6);
  
  
  /* USER CODE BEGIN 4 */
  void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  {
    if(htim->Instance == TIM6)
    {
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
    }
  }
  ```



### DMA 전송

- APB1 에 USART2, TIM 6 연결되어있음 -> DMA 전송 가능
  ![image-20210210093423226](C:\Users\JJW_N-771\Desktop\stmpjt\5_TIM_mode_1\README.assets\image-20210210093423226.png)

#### MX

- UART2
  ![image-20210210093726566](C:\Users\JJW_N-771\Desktop\stmpjt\5_TIM_mode_1\README.assets\image-20210210093726566.png)

- TIM6 DMA setting
  ![image-20210210100059094](C:\Users\JJW_N-771\Desktop\stmpjt\5_TIM_mode_1\README.assets\image-20210210100059094.png)

#### IDE

- tim.h

  ```c
  /* USER CODE BEGIN Private defines */
  
  extern DMA_HandleTypeDef hdma_tim6_up;
  ```

  

- main.c

  ```c
  /* USER CODE BEGIN 0 */
  uint8_t data[] = "HELLO, DongSik!\r\n";
  
  
  /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim6);
  
    HAL_DMA_Start(&hdma_tim6_up, (uint32_t)data, (uint32_t)&USART2->TDR, sizeof(data)-1);
    __HAL_TIM_ENABLE_DMA(&htim6, TIM_DMA_UPDATE);
  ```

  

#### 결과

- 1초당 한 글자씩
  ![image-20210210100405665](C:\Users\JJW_N-771\Desktop\stmpjt\5_TIM_mode_1\README.assets\image-20210210100405665.png)





