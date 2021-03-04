## 목표

1. ##### 임베디드 개발 이해

2. ##### C 언어 학습

3. ##### STMicroelectronics의 MCU와 보드에 대한 학습



## 일정



|        |                            목표                            | 임베디드  | C                  | STM32                                     | 개발 툴     |
| ------ | :--------------------------------------------------------: | --------- | ------------------ | ----------------------------------------- | ----------- |
| 1 week |      임베디드와 C 언어의 기초를 학습과 전체 계획 수립      | MCU       | 문법, 자료형, 모듈 | 개발환경 세팅과 이해                      | confluence  |
| 2 week | STM32 HAL 예제를 따라하며 필요 임베디드와 C 언어 지식 학습 | Datasheet | pointer, memory    | Peripheral 예제 따라하며 HAL library 학습 |             |
| 3 week |                 RTOS 이해 - ISR, task 구조                 | RTOS      | stack, queue       | freeRTOS                                  | git, gerrit |
| 4 week |                          HAL 응용                          |           | simulation         | Peripheral 심화 학습 및 응용              |             |



## 디렉토리 설명

- GPIO_std/
  - 맛보기 예제로 GPIO 제어
  - 버튼으로 LED 제어 (polling 방식)
  - LED toggle
- 2_UART_printf
  - 시리얼 디버깅
- 3_GPIO_LED
  - LED 제어 기본, 응용
- 4_TIM_PWM
  - LED dimming
- 5_TIM_moed_1
  - Counter mode (DMA 맛보기)
- 5_TIM_moed_2
  - Output Compare mode
- 5_TIM_moed_3
  - Input Capture mode
- 6_ADC_I2C
  - MAX30100 (PPG) 센서와 I2C 통신
  - visualization 위한 pyserial, matplotlib 활용
- 7_UART_IO
  - UART Polling, Interrupt 방식
- 7_UART_IO_DMA
  - UART DMA 방식
- 8_freeRTOS_1
  - FreeRTOS API, task, heap
- 8_freeRTOS_2
  - queue, semaphore
- 8_freeRTOS_3
  - mutex, notification
- 8_freeRTOS_4
  - interrupt, timer
- 9_BitWise_operation
  - bit mask

