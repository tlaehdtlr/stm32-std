## 0. PinMap

| Category | No.  | Name | Type | Signal | Label      | 용도 |
| -------- | ---- | ---- | ---- | ------ | ---------- | ---- |
| UART     | 1    | PA2  | I/O  |        | LPUART1_TX |      |
|          | 2    | PA3  | I/O  |        | LPUART1_RX |      |
| Button   | 3    | PC13 | I/O  | Input  | B1_USER    |      |
| LED      |      | PB13 | I/O  | Output | LD4_USER   |      |
|          |      |      |      |        |            |      |
|          |      |      |      |        |            |      |
|          |      |      |      |        |            |      |
|          |      |      |      |        |            |      |





## 1. UART 연결

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



## Button & LED

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

  

