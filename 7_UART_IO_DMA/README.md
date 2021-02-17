## 7_UART_IO_DMA



- 참고 링크 (설명 상세함)
  https://m.blog.naver.com/PostView.nhn?blogId=jrkim&logNo=220927030543&proxyReferer=https:%2F%2Fwww.google.com%2F 
  여기 코드는 아마 overflow 상황을 처리가 없는 거 같음

#### MX

- DMA setting
  ![image-20210217121344615](C:\Users\JJW_N-771\Desktop\stmpjt\7_UART_IO_DMA\README.assets\image-20210217121344615.png)
- NVIC
  ![image-20210217121412752](C:\Users\JJW_N-771\Desktop\stmpjt\7_UART_IO_DMA\README.assets\image-20210217121412752.png)



#### IDE

- https://blog.naver.com/chandong83/222036416616 참고
  코드 그대로 하면 아스키 코드로 출력되길래 transmit 함수 사용함

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



