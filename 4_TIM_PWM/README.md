## 4. TIM_PWM

### Dimming LED (1sec.)



#### PinMap

| Category | No.  | Name | Type | Signal | Label | 용도 |
| -------- | ---- | ---- | ---- | ------ | ----- | ---- |
| LED      |      | PB13 | I/O  | Output |       |      |



#### 코드 생성

- timer 설정 
  - TIM15- general
    - channel 1
    - PWM 으로 LD4의 경우 CH1N 으로만 가능 (위상 반대)
      ![image-20210209162031472](C:\Users\JJW_N-771\Desktop\stmpjt\4_TIM_PWM\README.assets\image-20210209162031472.png)
- NVIC 활성화도 해주기
  ![image-20210209154040783](C:\Users\JJW_N-771\Desktop\stmpjt\4_TIM_PWM\README.assets\image-20210209154040783.png)



- clock
  ![image-20210209154107099](C:\Users\JJW_N-771\Desktop\stmpjt\4_TIM_PWM\README.assets\image-20210209154107099.png)
- bus clock 80MHz, prescaler 8000-1, clock period 100-1
  - output clock 은 80MHz/ 8000 / 100 = 100Hz , 10ms 주기
  - duty 제어는 clock period 값으로 생각해야함
  - 만약 저기서 prescaler 80, clock period 10000 으로 하면 resolution 을 더 좋게 (값을 세밀하게 쪼개면서 제어할 수 있단 말이지 0~10000 이렇게)

#### 코드 작성

- 위상이 반대였던 CH1N 를 위한 함수!!!
  up, down 체크 flag

  직접 CCR 제어 (__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,dutyCycle-10) 같은 함수로도 가능)
  0~100 범위를 1의 값을 변화 시키므로 10ms 딜레이를 주면서 1s 를 맞춤

```c
  /* USER CODE BEGIN 2 */
  HAL_TIMEx_PWMN_Start(&htim15, TIM_CHANNEL_1);



/* USER CODE BEGIN WHILE */
  uint8_t check_mode = 0;

  while (1)
  {

    /*LED */
    switch (check_mode)
    {
      case (0):
        htim15.Instance->CCR1 += 1;
        if (htim15.Instance->CCR1 > 99)
        {
          check_mode = 1;
        }
        break;
      case (1):
        htim15.Instance->CCR1 -= 1;
        if (htim15.Instance->CCR1 < 1)
        {
          check_mode = 0;
        }
        break;
    }
    HAL_Delay(10);
```

