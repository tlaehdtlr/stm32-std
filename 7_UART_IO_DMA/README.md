## 7_UART_IO_DMA

### callback 이용

- https://m.blog.naver.com/PostView.nhn?blogId=eziya76&logNo=220943681644&proxyReferer=https:%2F%2Fwww.google.com%2F

- TX는 굳이 쓸 필요없는거 같아서 RX만 함

#### MX

- ![image-20210217183206261](C:\Users\JJW_N-771\Desktop\stmpjt\7_UART_IO_DMA\README.assets\image-20210217183206261.png)
  ![image-20210217173003476](C:\Users\JJW_N-771\Desktop\stmpjt\7_UART_IO_DMA\README.assets\image-20210217173003476.png)



#### IDE

- 링 버퍼를 10칸 줬으니 10개가 채워질 때마다 콜백이 불림
  UART Interrupt 방식과 다른 점은 출력하고 나서도 HAL_UART_Receive_DMA 함수를 계속 켜줄 필요가 없다(활성화되어 있음, 중단하는 함수들 있음)

  ```c
  /* USER CODE BEGIN 0 */
  uint8_t rxBuffer[10];
  
  void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
  {
    HAL_UART_Transmit(&huart1, rxBuffer, 10, 0xFFFF);
  }
  
    /* USER CODE BEGIN 2 */
    HAL_UART_Receive_DMA(&huart1, rxBuffer, 10);
  
  ```



### 직접 짜기 (어쩌다가 하게된 것)

- 언젠가는 쓰이겠지...?

#### MX

- DMA setting
  ![image-20210217121344615](C:\Users\JJW_N-771\Desktop\stmpjt\7_UART_IO_DMA\README.assets\image-20210217121344615.png)
- NVIC
  ![image-20210217121412752](C:\Users\JJW_N-771\Desktop\stmpjt\7_UART_IO_DMA\README.assets\image-20210217121412752.png)



#### IDE

- 
  참고 코드 그대로 하면 아스키 코드로 출력되길래 transmit 함수 사용함

  ```c
  #include <string.h>
  
  #define DMA_BUF_SIZE 100
  uint8_t rx_dma_buf[DMA_BUF_SIZE];
  #define get_dma_data_length() huart1.hdmarx->Instance->CNDTR
  #define get_dma_total_size()  huart1.RxXferSize
  
  void uart_rx_dma_handler(){
  	/* buffer index*/
      static uint32_t old_pos=0;
      uint32_t pos;
  
      /* trans buffer from dma buffer*/
      uint8_t rx_buf[DMA_BUF_SIZE];
      uint32_t rx_size = 0;
  
      /*trans variable*/
      uint8_t ch;
      uint8_t i;
  
      /*find current index in buffer*/
      pos = get_dma_total_size() - get_dma_data_length();
  
      if(pos != old_pos) 
      {
          if(pos > old_pos) 
          {
  			/* get data from dma buf*/
              memcpy(rx_buf, &rx_dma_buf[old_pos], pos - old_pos);
              rx_size += pos - old_pos;
          } 
          else 
          {
  			/* Overflow */
  			/* fisrt get data until end index */
              memcpy(rx_buf, &rx_dma_buf[old_pos], get_dma_total_size() - old_pos);
              rx_size += get_dma_total_size() - old_pos;
  
              /* check cur and get data until cur index*/
              if(pos > 0)
              {
                  memcpy(rx_buf[get_dma_total_size() - old_pos], &rx_dma_buf[0], pos);
                  rx_size += pos;
              }
          }
  
          old_pos = pos;
          if(rx_size > 0) 
          {
              for(i = 0;i<rx_size;i++) 
              {
                  ch = rx_buf[i];
                  //printf("%02X", ch);
                  HAL_UART_Transmit(&huart1, &ch, sizeof(uint8_t), 1000);
              }
              //printf("\n\r");
          }
      }
  }
  
  
  
  ...
      
  
    HAL_UART_Receive_DMA(&huart1, rx_dma_buf, DMA_BUF_SIZE);
  while (1)
  {
           uart_rx_dma_handler();   
  }
  ```



