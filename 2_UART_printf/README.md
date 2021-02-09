## 2. UART 연결

### 시리얼 디버깅

#### Schematic

1. stlink 칩과 nucleo 보드가 서로 연결된 UART2 사용
   - ![image-20210205114941546](C:\Users\JJW_N-771\Desktop\stmpjt\2_UART_printf\README.assets\image-20210205114941546.png)
2. 자체 UART 사용 (이것으로 진행!!)
   - ![image-20210205114819956](C:\Users\JJW_N-771\Desktop\stmpjt\2_UART_printf\README.assets\image-20210205114819956.png)



#### PinMap

| Category | No.  | Name | Type | Signal | Label      | 용도 |
| -------- | ---- | ---- | ---- | ------ | ---------- | ---- |
| UART     |      | PA2  | I/O  |        | LPUART1_TX |      |
|          |      | PA3  | I/O  |        | LPUART1_RX |      |
|          |      | PA9  | I/O  |        | UART1_TX   |      |
|          |      | PA10 | I/O  |        | UART1_TX   |      |

- 1,2 선 연결

  - 1 : stlink 칩과 nucleo 보드가 서로 연결된 UART2 사용 가능

  - 2 : USB serial converter 연결

    RX, TX 선을 서로 교차해서 연결해준다
    ![image-20210205142729466](C:\Users\JJW_N-771\Desktop\stmpjt\2_UART_printf\README.assets\image-20210205142729466-1612849059339.png)

#### 코드 생성

- clock 80MHz (전부)
- ![image-20210209143915690](C:\Users\JJW_N-771\Desktop\stmpjt\2_UART_printf\README.assets\image-20210209143915690.png)
- ![image-20210209143954047](C:\Users\JJW_N-771\Desktop\stmpjt\2_UART_printf\README.assets\image-20210209143954047.png)



#### 코드 작성

- 2번 사용 (UART 선)

```c
/* USER CODE BEGIN Includes */
#include <stdio.h>

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



#### Tera term 연결

- COM8 이 UART 선으로 연결한 것 (COM5는 USB로 ST link 연결된 것
  ![image-20210209145641775](C:\Users\JJW_N-771\Desktop\stmpjt\2_UART_printf\README.assets\image-20210209145641775.png)
- 테라텀 설정
  ![image-20210209145542951](C:\Users\JJW_N-771\Desktop\stmpjt\2_UART_printf\README.assets\image-20210209145542951.png)



#### 결과

- ![image-20210209145955035](C:\Users\JJW_N-771\Desktop\stmpjt\2_UART_printf\README.assets\image-20210209145955035.png)