## 5_TIM_mode_1

#### Counter, External Input Counter, Output Compare Mode

- Counter 모드 
  - CNT 가 ARR 에 의해 0 되면서 인터럽트

- External Input Counter 모드
  - 외부 인터럽트에 의해 카운터 값 증가
  - ARR 값에 도달하면 reset 되면서 다른 인터럽트 발생

- Output Compare 모드
  - cnt 가 ccr 과 일치할 때, 인터럽트 발생
  - OC 인터럽트

1. TIM_1(TIM2, general)로 0.5 초마다 시간 카운트 1초 마다 exti_1 발생
2. exti_1 에 의해 TIM_2(TIM15, general) 카운팅 5번이면 LED
3. TIM_3(TIM6, basic) 에 의해 printf



### Counter Mode

#### 코드 생성

- TIM6(basic timer), 1s 마다 인터럽트, LED toggle
  ![image-20210209210349822](C:\Users\JJW_N-771\Desktop\stmpjt\5_TIM_mode_1\README.assets\image-20210209210349822.png)

- NVIC, clock



#### 코드 작성

- 


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









