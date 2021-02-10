## 5_TIM_mode_2

### Output Compare Mode

- Output Compare 모드
  - cnt 가 ccr 과 일치할 때, Output Pin 상태를 제어할 수 있는 모드이다. 
  - Pulse 가 CCRx 레지스터 값으로 CNT 와 비교되는 값. 두 개의 값이 같을 때, 인터럽트 발생
- TIM_OCMODE_TIMING  이용
- 1) 타이머 overflow 시, PeriodElapsedCallback 으로 LED 점멸
  2) button 눌러서 pulse 값 조정 (PWM 같이)

#### MX

- 
  ![image-20210210111249473](C:\Users\JJW_N-771\Desktop\stmpjt\5_TIM_mode_2\README.assets\image-20210210111249473.png)

- 
  

#### IDE

- 5s 마다 HAL_TIM_PeriodElapsedCallback
- 2.5s 마다 HAL_TIM_OC_DelayElapsedCallback

```c
HAL_TIM_Base_Start_IT(&htim15);
HAL_TIM_OC_Start_IT(&htim15, TIM_CHANNEL_1);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == htim15.Instance)
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
  }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == htim15.Instance)
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
  }
}
```







