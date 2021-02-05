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
  - 처음에 UART1 을 이용하려했는데 실패
    - 회로도 확인해보니 STlink 와 연결되지 않은 것을 확인
      ![image-20210205114819956](C:\Users\JJW_N-771\Desktop\stmpjt\OJT\README.assets\image-20210205114819956.png)]
    - 따라서 LPUART1 로 수정
      ![image-20210205114941546](C:\Users\JJW_N-771\Desktop\stmpjt\OJT\README.assets\image-20210205114941546.png)

- 코드 작성

  ```c
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
    uint8_t check_button;
    while (1)
    {
  	  /*polling*/
  	  check_button = HAL_GPIO_ReadPin(B1_USER_GPIO_Port, B1_USER_Pin);
  	  if (check_button)
  	  {
  		  HAL_GPIO_WritePin(LD4_USER_GPIO_Port, LD4_USER_Pin, GPIO_PIN_SET);
  	  } else
  	  {
  		  HAL_GPIO_WritePin(LD4_USER_GPIO_Port, LD4_USER_Pin, GPIO_PIN_RESET);
  	  }
  
  ```

  