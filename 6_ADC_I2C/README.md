

## 6_ADC_I2C

### MAX30100


- 광혈류측정 (PhotoPlethysmoGraphy, PPG)
  - LED를 인체 조직에 비추고 조직을 통과하는 혈액량을 포토다이오드가 반사된 빛 검출
  - 측정된 값의 피크로 심작 박동 측정



#### MX

- I2C Pin
  ![image-20210215111658803](C:\Users\JJW_N-771\Desktop\stmpjt\6_ADC_I2C\README.assets\image-20210215111658803.png)

- ![image-20210217083645385](C:\Users\JJW_N-771\Desktop\stmpjt\6_ADC_I2C\README.assets\image-20210217083645385.png)



#### IDE

- https://github.com/eepj/MAX30100_for_STM32_HAL
  - 외부 라이브러리 추가 후, 편하게 이용하기 위해 include 경로 등록
    이 때, main.c 의 properties 에서 경로 추가했는데 프로젝트에다가 하는게 맞을지 확실하지 않음
    
    ![image-20210217084314966](C:\Users\JJW_N-771\Desktop\stmpjt\6_ADC_I2C\README.assets\image-20210217084314966.png)
    



- MAX30100 의 datasheet 를 읽고 코드 작성
  ![image-20210215111519535](C:\Users\JJW_N-771\AppData\Roaming\Typora\typora-user-images\image-20210215111519535.png)	
  ![image-20210217084914829](C:\Users\JJW_N-771\Desktop\stmpjt\6_ADC_I2C\README.assets\image-20210217084914829.png)

- main.c 라이브러리 이용하여 간단하게 구현

  ```c
    /* USER CODE BEGIN WHILE */
    printf("start!! \r\n");
    MAX30100_Init(&hi2c1, &huart1);
    MAX30100_SetSpO2SampleRate(MAX30100_SPO2SR_DEFAULT);
    MAX30100_SetLEDPulseWidth(MAX30100_LEDPW_DEFAULT);
    MAX30100_SetLEDCurrent(MAX30100_LEDCURRENT_DEFAULT, MAX30100_LEDCURRENT_DEFAULT);
    MAX30100_SetMode(MAX30100_SPO2_MODE);
  
    while (1)
    {
      MAX30100_ReadFIFO();
      MAX30100_PlotBothToUART(&huart1, _max30100_red_sample, _max30100_ir_sample, 16);
      HAL_Delay(1000);
  ```

  

#### 결과 확인

- teraterm 가능
- https://hackaday.io/project/5334-serialplot-realtime-plotting-software 이거로 쉽게 가능
- pyserial/
  - 참고
    http://www.mikeburdis.com/wp/notes/plotting-serial-port-data-using-python-and-matplotlib/
  - matplotlib, pyserial 이용
  - 반응속도가 좋지 않음
    - FIFO에 어느정도 쌓이면 한 번에 전송하는 방법 쓰면 괜찮지 않을까
  - 일정 시간 경과 후, stuck
    - 배열이 커져서 터질 수 있음
      - csv 파일 or numpy 이용하면 해결 가능하지 않을까
  - ![image-20210217090252197](C:\Users\JJW_N-771\Desktop\stmpjt\6_ADC_I2C\README.assets\image-20210217090252197.png)

