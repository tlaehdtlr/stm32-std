## 1. UART 연결

#### PinMap

| Category | No.  | Name | Type | Signal | Label      | 용도 |
| -------- | ---- | ---- | ---- | ------ | ---------- | ---- |
| UART     |      | PA2  | I/O  |        | LPUART1_TX |      |
|          |      | PA3  | I/O  |        | LPUART1_RX |      |



#### 시리얼 디버깅

- 코드 생성

  - clock 80MHz

  - 선 연결 UART1 과 UART2

    - 1. USB serial converter 연결
         ![image-20210205142729466](C:\Users\JJW_N-771\Desktop\stmpjt\OJT\README.assets\image-20210205142729466.png)
         RX, TX 선을 서로 교차해서 연결해준다

      ![image-20210205114819956](C:\Users\JJW_N-771\Desktop\stmpjt\OJT\README.assets\image-20210205114819956.png)]

    - 2. stlink 칩과 nucleo 보드가 서로 연결된 UART2 사용 가능
         ![image-20210205114941546](C:\Users\JJW_N-771\Desktop\stmpjt\OJT\README.assets\image-20210205114941546.png)

- 코드 작성
  1번 사용

  ```c
  /* USER CODE BEGIN 0 */
  #ifdef __GNUC__
      #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #else
      #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #endif
  
  PUTCHAR_PROTOTYPE
  {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
  }
  ```

  ```c
  /* USER CODE BEGIN WHILE */
    uint8_t cnt_from_start = 0;
    while (1)
    {
  	  cnt_from_start++;
  	  printf("Hello DongSik!! \r\n");
  	  printf("time : %d \r\n", cnt_from_start);
  	  HAL_Delay(1000);
  ```



## 2. Button & LED

#### PinMap

| Category | No.  | Name | Type | Signal | Label    | 용도 |
| -------- | ---- | ---- | ---- | ------ | -------- | ---- |
| Button   |      | PC13 | I/O  | Input  | B1_USER  |      |
| LED      |      | PB13 | I/O  | Output | LD4_USER |      |



#### Polling 방식(button 의 level) LED 제어

- 코드 생성
  
  - button 추가
    ![image-20210205115939076](C:\Users\JJW_N-771\Desktop\stmpjt\OJT\README.assets\image-20210205115939076.png)
  - LED 추가
  
  
- 코드 작성

  ```c
  /* USER CODE BEGIN WHILE */
    while (1)
    {
       /*polling*/
       if (HAL_GPIO_ReadPin(B1_USER_GPIO_Port, B1_USER_Pin) == GPIO_PIN_SET)
       {
       HAL_GPIO_WritePin(LD4_USER_GPIO_Port, LD4_USER_Pin, GPIO_PIN_SET);
       } 
       else
       {
       HAL_GPIO_WritePin(LD4_USER_GPIO_Port, LD4_USER_Pin, GPIO_PIN_RESET);
       }
  ```

#### Interrupt

- 코드 생성

  - 버튼 EXIT 로 바꾸고 NVIC 활성화

- 코드 작성

  ```c
  /* USER CODE BEGIN 4 */
  void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
  {
    if (GPIO_Pin == BUTTON_USER_Pin)
    {
      HAL_GPIO_TogglePin(LD4_USER_GPIO_Port, LD4_USER_Pin);
    }
  }
  ```

#### 버튼 누르고 있는 동안 LED 점등

- PULL UP 방식 (active low)

  - 스위치 온 : Vcc -> 0V 된다
  - 디지털 신호는 0 -> 1 (maybe?)
  - 스위치 온을 detect 하는 건 rising edge

- 코드 생성

  - rising, falling 다 봐야하므로
    ![image-20210205164342156](C:\Users\JJW_N-771\Desktop\stmpjt\OJT\image-20210205164342156.png)
    ![image-20210205164357717](C:\Users\JJW_N-771\Desktop\stmpjt\OJT\README.assets\image-20210205164357717.png)

- 코드 작성

  ```c
  /* USER CODE BEGIN 4 */
  void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
  {
    if (GPIO_Pin == BUTTON_USER_Pin)
    {
      if (HAL_GPIO_ReadPin(BUTTON_USER_GPIO_Port, BUTTON_USER_Pin) == GPIO_PIN_SET)
      {
        HAL_GPIO_WritePin(LD4_USER_GPIO_Port, LD4_USER_Pin, GPIO_PIN_SET);
      }
      else
      {
        HAL_GPIO_WritePin(LD4_USER_GPIO_Port, LD4_USER_Pin, GPIO_PIN_RESET);
      }
    }
  }
  ```

  

## 3. PWM

#### LED Dimming (1초)

- 사용 핀 번호 : 1, 2, 4

- 코드 생성

  - TIM (channel) 연결된 핀 PWM 모드 설정

  - https://dkeemin.com/stm32f0xx-pwm-%EC%84%A4%EC%A0%95-%EB%B6%80%EB%A1%9D-%EC%84%A4%EB%AA%85/

  - ![image-20210208120841322](C:\Users\JJW_N-771\Desktop\stmpjt\OJT\README.assets\image-20210208120841322.png)

  - clock 설정

    - datasheet

      ![image-20210208121659473](C:\Users\JJW_N-771\Desktop\stmpjt\OJT\README.assets\image-20210208121659473.png)

      TIM1은 APB2 버스에 연결
      40MHz로 설정 (이유는 내 마음)

    - 100ms 마다 빛 세기를 변화 시키고 싶어 (이유는 눈 깜빡이는 속도가 그쯤이라)
      counter clock을 100KHz 로 하기 위해 prescaler 는 (400 - 1)
       출력 클럭을 10Hz로 하기 위해 period (10000 -1)
      duty ratio 는 코드로 변화 시킬거야 (duty ratio가 100%에서 시작하고 pusle는 10 이 되겠네)

    

- 코드 작성

  - 외부 인터럽트 발생할 때마다 pulse 값 조정, Timer 조정 (PWM 변경)
  - HAL_TIM_PWM_Start
  - __HAL_TIM_SET_COMPARE

