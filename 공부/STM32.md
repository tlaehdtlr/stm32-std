## software tools

#### [STM32 Cube]

\1. Java JDK

- [ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1.%20tool/Java/jdk-8u241-windows-x64.exe](ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1. tool/Java/jdk-8u241-windows-x64.exe)
- 이클립스 기반으로 수행되는 STM32 제공 프로그램이라 이거 설치해야함

\2. STM32CubeIDE

- 코드 작성 통합 개발환경

\3. STM32CubeMx

- 코드 생성기

\4. STM32CubeProg

- (.bin) 이진코드를 보드에 로드하는 프로그램
- connect, verify program, run after program하고 스타트

\5. STM32CubeMon

- 아직 안 써봄

#### [Driver]

\1. Prolific USB to UART Driver

- [ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1.%20tool/USB%20to%20UART/PL23XX_Prolific_DriverInstaller_v200.zip](ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1. tool/USB to UART/PL23XX_Prolific_DriverInstaller_v200.zip)

\2. ST-Link USB driver

- [ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1.%20tool/ST-Link/USB%20Driver/en.stsw-link009.zip](ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1. tool/ST-Link/USB Driver/en.stsw-link009.zip)



## STM32

##### 각 예제 폴더에 README 로 설명 적어놨으니 확인할 것

### 목표

- 사용자가 작성한 코드를 (.c 파일) 이진코드 (.bin)으로 바꿔서 STM32 (flash? 레지스터? 찾아봐 여튼 보드 메모리 공간에 보낸다)



### 순서 (회로도 - PinMap - 코드생성 - 코드작성 - 다운로드 - 모니터링)

https://ndb796.tistory.com/360 여기 굿

- STM32 cube(IDX, MX, Prog, Mon) 전부 설치 (java eclipse 기반으로 만들어졌기 때문에 java 설치 선행 필요)
- STM32를 컴퓨터와 연결 (나중에 IDE에서 run 할 때 연결해도 뭐...)
  - 장치관리자 - 범용 직렬 버스 장치에 ST_link debug 생성됨

### CubeMX (회로도를 보고 핀 들을 제어하는 기본적인 코드를 만들어준다)

- new project
  - 3가지 선택
  - 첫 번째는 새로 만든 하드웨어라면 선택
  - 두 번째는 건들지 않은거
  - 세 번째는 뭔가 템플릿 있는거 같음
- board selector
  - 내 보드 검색해서 찾아줘 (즐추하면 편함)
  - 프젝 시작 (초기화 시켜줄까 하면 ㄳ ㅇㅋ)
- 프로젝트 설정
  - 핀 배치
  - 클럭 설정
  - 프로젝트 매니저
    - 이름, 경로
    - IDE 선택, generate under root 체크 해제
    - MCU 패키지 라이브러리 필요한 것만 카피
    - peripheral 마다  .c/.h 로 초기화 생성 체크
- 코드 생성

### CubeIDE (코드 작성 및 실행)

- 프젝 생성
- main.c ( 여기서 private user code 밑에 code begin 과 end 사이에 코드 적어줘야 MX 수정해도 살아있음)
- 프로젝트 빌드
- 실행



### Clock

- 심장이다 (에너지를 공급하는거지)
  - 무조건 최대말고 저전력을 요구하면 좀 작게 할 필요가 있겠지?

#### 구성 요소?

- CPU clock 은 HSE, HSI 중 선택
  - HSE (High Speed External) : 외부 고속 Clock, stm32 외부에 Crystal/Ceramic resonator 필요, Duty가 50% 이하인 외부 구형파, 삼각파 신호로도 사용 가능
  - HSI (High ... internal) : 내장된 RC 발진 회로에 의해 동작 clock, 자체 calibration 기능 있지만, RC 발진회로의 특성 문제로 온도 상승에 따른 오차 발생
- 초당 clock
  - LSE (Low ... ) : 32.768kHz의 Cryst.... 사용, 용도는 저전력 구현 및 정확한 시간 (RTC)을 맞추기 위함
  - LSI () : independent watchdog와 AWU(Auto Wakeup) 기능 및 RTC clock에 사용 (정확성 확보 어려움)
- MSI (Multi Speed internal) : 저전력이라는데 음?
- HCLK : Core Clock 으로 실제 소스 코드 동작시키는 clock
- SYSCLK : System Clock 으로 Power on reset 직후 무조건 내부 clock으로 먼저 동작
- CSS (Clock Security System) : HSE clock 에 문제 발생시, NMI interrupt 발생 및 clock source 를 HSI clock으로 변경해주는 기능

#### 고려 사항?

- HSI/HSE는 시스템 clock 소스로 사용
  - 바로 사용되지 않고 clock tree를 통해 PLL이나 Prescaler 를 통해 필요한 주파수로 변경 후 사용
- LSI/LSE 는 RTC 와 Independent Watchdog 용으로 사용
- External 은 Bypass Clock source 또는 Crystal/ceramic resonator 로 설정 가능
  - bypass 는 다른 장치에서 clock 을 전달 받아 사용할 때 선택
  - crystal 은 외부에 clock 회로를 구성한 경우 선택
- master clock output 을 설정하면 특정 clock source 를 다시 특정 pin으로 출력하여 다른 주변 IC Clock Source로 사용 가능

#### HAL API

- clock 초기화 시, RCC_OsclnitTypeDef, RCC_ClklnitTypeDef 구조체 변수 사용
- HAL_RCC_OscConfig 함수는 Main PLL 설정하여 PLLCLK 설정
- HAL_RCC_ClockConfig 함수는 PLLCLK 가 SYSCLK 으로 사용되도록 설정
- HAL_SYSTICK_Config 함수는 SysTick 타이머가 1ms 마다 구동하도록 설정
- HAL_RCC_EnableCSS 함수는 HSE 에 오류 발생 시, 이를 검출하여 NMI Exception 을 발생시킴
- HAL_RCC_MCOConfig 함수는 PLLCLK 클럭이 MCO 핀으로 출력되도록함



### 소비전력 계산

- https://m.blog.naver.com/compass1111/221205513966
- MCU 소비전력 계산이고, 다른 장치들이 붙어있기 때문에 실제 장치의 소비전력과 사용시간은 다름.
- MCU의 온도도 표시됨





### DMA (Direct Memory Access)

- https://m.blog.naver.com/PostView.nhn?blogId=eziya76&logNo=221436500639&proxyReferer=https:%2F%2Fwww.google.com%2F
- 주변장치에서 메모리(SRAM?)로 데이터를 옮길 때, 프로세서의 core의 작업 없이 DMA controller 가 수행함 (성능 개선 효과!!) 
- peripheral 세팅할 때, DMA continous request 를 enable 해줘야해
- NTD
- 모드 normal, circular(값을 계속 받을테니 이게 좋겠지?)
- 메모리 주소를 증가시켜야겠지? (peripheral  주소를 증가시켜야하는 경우도 있을까 몰라)

### RCC (Reset Clock Controller)

### CRC(Cyclic Redundancy Check)

### NVIC (Nested vectored interrupt controller)



## Peripheral

### 코드 참고

- C:\Users\유저\STM32Cube\Repository\STM32Cube_FW_L4_V1.16.0\Projects\NUCLEO-L412RB-P\Examples
  - 여기 참고 하면됨



### GPIO (General Purpose Input/Output)

- MCU가 입출력 장치제어하는 포트 또는 핀 (여러 핀으로 이뤄진 포트)

- 나는 NUCLEO-L412RB-P 보드를 쓰고 LD4가 연결된 PB13 핀을 제어하려면 MCU 내부에 있는 GPIO 레지스터를 제어해야함

  - GPIO 레지스터 (configuration, data, set/reset, locking, alternate selection)
  - GPIO 관련 레지스터 어드레스로 레지스터에 접근하기 위해 "Memory map"을 알아야함
    - 근데 이거 어찌 아는지 잘모르겠음
  - 레지스터 종류가 엄청 많은데 뭐 그 때마다 알아서 잘 찾으면 되는 듯해....

- HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)

  - ```c
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
    HAL_Delay(3000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_Delay(3000);
    ```

    어디 포트의 어느 핀인지를 찾아줘야해

    이거는 컨트롤 누르고 클릭해가면서 LD4가 어느 핀인지 확인 가능해 이름을 넣거나, 핀 번호를 넣거나 하면 돼
    
    

- HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)

  - ```c
    HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    HAL_Delay(500);
    //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
    //HAL_Delay(2000);
    ```

    여기도 마찬가지

- HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)

  - 버튼으로 제어하기
  
  - ```c
    // 이게 어떤 식으로 쓰는건지 확실하지는 않은데 참고한 함수 가서 이것도 이렇게 가져옴
      GPIO_PinState bitstatus;
    
    // loop안에다가
     bitstatus = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
    if (bitstatus == GPIO_PIN_RESET)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    } else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
  }
    ```

##### 디버그 방법

- 멈춰 놓고 레지스터 직접 제어 가능하다
  - C 코드 클 때, 유용하겠지?
- 디버그 모드에서 
  - 1) SFRs 여기에서 제어 가능
  - 2) window - show view -memory 들어가면 메모리로 접근 가능

### UART (Universal Asynchronous Receiver/Transmitter)

- USART 는 Synchronous 즉 등기인데 UART와의 차이는 Clock 소스 유무임

  - USART 통신은 동기화 Clock에 따라 데이터 전송

- Uart 통신은 협의된 baud rate (보드 속도, 비트/초의 단위인 데이터 전송 속도를 뜻함) 에 따라서 T1bit 간격으로 샘플링된 값을 데이터로 수신하며 start 비트와 stop 비트는 1.5bit 길이를 가짐

- 시리얼 디버깅을 위한 시리얼 통신 프로그램 설치 필요!! Tera  term 통신 프로그램 설치

  - 연결을 시리얼, 포트는 STM STLink vir... 이걸로 맞추고
  - 설정 - 시리얼포트 에서 속도를 나중에 통신할 것의 Baud rate 와 맞춘다
  - 설정을 저장해놓는다

- 소스 코드

  - https://velog.io/@madebygenji/IAR%EC%97%90%EC%84%9C-UART-%ED%86%B5%EC%8B%A0%EC%9C%BC%EB%A1%9C-printf-%EC%B6%9C%EB%A0%A5

  - 한 줄 띄우고 싶으면 `\r\n` 을 붙여야함 

    - ```c
      if (ch == '\n')
      	  HAL_UART_Transmit(&huart2, (uint8_t *) "\r", 1, 0xFFFF);
      ```

    - 이걸 적어서 \r 안 적게할 수 있긴함

  - 여튼 내꺼는

    ```c
    /* USER CODE BEGIN 0 */
    #ifdef __GNUC__
        #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
    #else
        #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
    #endif
    
    PUTCHAR_PROTOTYPE
    {
      if (ch == '\n')
    	  HAL_UART_Transmit(&huart2, (uint8_t *) "\r", 1, 0xFFFF);
    	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
      return ch;
    }
    
     while (1)
      {
    	  printf("Hello dongsik!\n");
    	  HAL_Delay(1000);
        /* USER CODE END WHILE */
    
        /* USER CODE BEGIN 3 */
      }
    ```

    



### EXTI (External Interrupt)

- 버튼 눌러 LED 토글하는 예제

  - 버튼의 회로도를 보고 눌렀을 때 신호가 (high -> low) 이면 GPIO 모드를 falling edge 로 바꿔줘야함

  - NVIC 활성화

    - NVIC 개념 https://m.blog.naver.com/PostView.nhn?blogId=eziya76&logNo=221428695204&categoryNo=38&proxyReferer=https:%2F%2Fwww.google.com%2F
    - NVIC 탭에서 인터넙트 관련 테이블 볼 수 있음
    - 인터럽트 코드에 대한 초기화 순서, 인터럽트 서비스 루틴 (IRQ) 생성 유무 설정

  - 인터럽트 발생 시, EXTI15_10_IRQHandler() -> HAL_GPIO_EXTI_IRQHandler() -> HAL_GPIO_EXTI_Callback()
    순으로 발생, 여기서 이 콜백 함수를 사용자가 이용해야하지

  - 내 코드

    ```c
    /* USER CODE BEGIN 4 */
    void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
    {
    	switch (GPIO_Pin)
    	{
    	case B1_Pin: //GPIO_PIN_13
    		HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    		break;
    	default:
    		;
    	}
    }
    ```

    main.h 에 정의된 포트와 핀 번호 확인 잘하기 바람







### Timer 기본

- clock 설정
  - HSE(high speed external), HSI(high speed internal),  LSE(low ...), LSI
  - high와 low는 메가와 킬로인 듯?
  
- 과정
  - system core - RCC - LSE(뭐 위에거 설정)
  - 핀 바뀜 (이걸 pin out 이라 하는걸까)
  - Timers - RTC 에 Activate clock source 활성화
  - Clock Configuration 에서 RTC Clock Mux 를 LSE(다른 것도)로 선택
  
- 종류가 여럿... datasheet 확인해보면 timer 종류 확인 가능
  - SysTick timer(항상 동작) 이런게 HAL_Delay() 함수 등에 사용됨
  - WatchDog timer(IWDG, WWDG) : CPU의 오동작을 탐지하여 문제가 발생하면 재부팅 시켜주는 타이머
  - Basic timer : 입출력 기능없이 시간만 재는 놈 TIMx (x:6)
  - General Purpose timer : 범용 타이머, 출려 비교, 원펄스, 입력 캡쳐 등 TIMx (x: 2,15,16)
  - Advanced-control timer : 모터 제어나 디지털 변환 TIMx (x:1)
  - 뭐를 고를지 고려해봐
    - 카운터 해상도(크기), DMA 사용 여부, 최대 인터페이스 속도
    - 카운터 타입, 캡쳐/비교 채널수
    - Prescaler 범위, 보상 출력 유무, 최대 타이머 클럭
  
- 주요 타이머 레지스터
  - counter register (Timx_CNT) : 카운터 값 자체를 저장
  - Precaler register (TIMx_PSC) : 분주비 레지스터 (분주할 비를 설정)
  - Auto-reload register (TIMx_ARR) : 카운터 주기 레지스터
  - Capture/Compare register (TTIMx_CCR) : 캡쳐/비교기 레지스터, 원하는 주기에 인터럽트를 설정할 수 있음
  
- 예시로 범용타이머로 주기 설정하려면
  - **Timer에 공급되는 버스 clock 속도, prescaler 값, Period 값 3가지는 설정해**
  - clock 은 clock configuration에서 설정
  - prescaler와 period는  parameter settings 에서 설정
  
- **원하는 주기 구하는 공식**
  
  - `Period * (1/APB1 버스 속도) * Prescaler`
    - ex) 0.01ms = 900 x(1/90MHz) x 1000
  
- 타이머 설정 순서?
  - clock 세팅

    - datasheet 의 clock tree를 확인
      - TIMx 의 클럭은 어떤 APBx 프리스케일러를 통과한 클럭을 소스로 활용하는지
      - TIM1 은 APB2, TIM2 는 APB1
        - TIM2 가 General purpose timer 이므로 이걸로 해본다
      - 주기 설정 공식
        - 1/80MHz * 80 * 1000
        - 근데 prescaler와 counter period를 공식이랑 달리 1작은 값을 넣어주라는데 (데이터 시트 timing schematic? 을 봐야 설명 가능이라는데 잘 모르겠다)
          - 여튼 그러면 79, 999
      - 반복 인터럽트 발생하게하려면 auto-reload preload를 enable

  - NVIC 세팅

    - TIMx 인터럽트 enable -> 타이머 카운터와 counter period 의 값이 일치하면 인터럽트 발생
    - 우선순위 변경 원하면 System core의 NVIC에서 세팅

  - 코드 생성

  - 코드 작성

    - IRQ (Interrupt ReQuest) 가 인터럽트 처리인가벼

    - TIMx_IRQHandler() -> HAL_TIM_IRQHandler() -> HAL_TIM_PeriodElapsedCallback() 호출

    - (1) 타이머 시작 , (2) 주기적인 인터럽트 루틴 만들기

    - (1)

      - ```c
          /* USER CODE BEGIN 2 */
          if (HAL_TIM_Base_Start_IT (&htim2) != HAL_OK)
          {
          	  Error_Handler();
          }
        ```

    - (2)

      - ```c
        /* USER CODE BEGIN PV */
        volatile int gTimerCnt;
        
        /* USER CODE BEGIN 4 */
        void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
        {
        	gTimerCnt++;
        	if (gTimerCnt == 1000)
        	{
        		gTimerCnt = 0;
        		HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        	}
        }
        ```


#### 5가지 모드

1) Counter 모드 

- CNT 가 ARR 에 의해 0 되면서 인터럽트

2) External Input Counter 모드

- 외부 인터럽트에 의해 카운터 값 증가
- ARR 값에 도달하면 reset 되면서 다른 인터럽트 발생

3) PWM Output 모드 

- 걍 뭐 암거나 x% 듀티비의 PWM 파형 생성해

4) Input Capture 모드 

- x 타이머로 PWM 생성
- y 타이머 채널 2개 input capture 모드로 각각 pwm 의 rising, falling 체크하고 주기랑 듀티비 구하기

5) Output Compare 모드

- cnt 가 ccr 과 일치할 때, 인터럽트 발생
- OC 인터럽트

#### 참고

- 타이머 모드와 개념 등 상세 설명
  https://m.blog.naver.com/duvallee/221459826325









### ADC (Analog to Digital Converter)

- 온도센싱 값 프린트 찍기

- 코드 생성

  - RCC 설정
  - ADC 설정
    - temperature sensor channel
    - 연속으로 AD 변환위해 continuous conversion  mode 를 enable
    - ADC_Regular_ConversionMode - Rank - Sampling Time 을 12.5 Cycles 설정
      - https://m.blog.naver.com/PostView.nhn?blogId=ysahn2k&logNo=221308223328&proxyReferer=https:%2F%2Fwww.google.com%2F
      - ADC 입력 단자로부터 받은 입력 신호로 MCU 내부 capacitor를 충전해
      - 이게 낮으면 샘플링 사이클동안 충분히 충전을 못 시킨다.
      - resolution 설정에 따라 바뀌는데 그 사이클 보다 크면 되긴해
  - clock
    - HCLK 값 최대로 주자

- 코드 작성

  - 시리얼 디버깅 구현 해놔

  - ADC Calibration 및 시작 / ADC 결과 값 출력

    - ```c
      /* Infinite loop */
        /* USER CODE BEGIN WHILE */
      
        /* start calibration */
        if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
        {
      	  Error_Handler();
        }
        /* start the conversion process */
        if (HAL_ADC_Start(&hadc1) != HAL_OK)
        {
      	  Error_Handler();
        }
      
        uint16_t adc1;
      
        while (1)
        {
          HAL_ADC_PollForConversion(&hadc1, 100);
          adc1 = HAL_ADC_GetValue(&hadc1);
          printf("ADC1_Temperature : %d \n", adc1);
      	  /* USER CODE END WHILE */
      
          /* USER CODE BEGIN 3 */
          HAL_Delay(100);
        }
      ```

    - HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef *hadc, uint32_t SingleDiff) 여기 singleDiff에 대한 설명

      - SingleDiff Selection of single-ended or differential input 
      - 내 생각엔 센서의 오차 정도를 줄일라고 저런 인자를 넣은게 아닐까 싶어

  - 빌드 실행

  - ADC 결과 값이 아닌 내부 온도를 계산하고 보내기

    - ```
      온도 구하는 공식
      Temperature (in °C) = {(V25 - Vsense) / Avg_slope} + 25
      ```

    - datasheet 확인해보니
      ![image-20210128144203651](C:\Users\JJW_N-771\Desktop\stmpjt\공부\STM32.assets\image-20210128144203651.png)

    - ```c
      /* USER CODE BEGIN PV */
      const float AVG_SLOPE = 2.5E-03;
      const float V30 = 0.76;
      const float ADC_TO_VOLT = 3.3/4096;
      
      ...
      
      float vSense, temp;
      
        while (1)
        {
          HAL_ADC_PollForConversion(&hadc1, 100);
          adc1 = HAL_ADC_GetValue(&hadc1);
          //printf("ADC1_Temperature : %d \n", adc1);
      
          vSense = adc1 * ADC_TO_VOLT;
          temp = (V30-vSense) / AVG_SLOPE + 25.0;
          printf("ADC1, Temp : %d , %f \n", adc1, temp);
      	  /* USER CODE END WHILE */
      
          /* USER CODE BEGIN 3 */
          HAL_Delay(100);
        }
      ```

    - printf 실수 출력하는 방법

      - project 탭 - properties
        C/C++ Build - Settings - Tool Settings - MCU Settings
        Use float with printf ... 체크 후 적용



### WWDG (Window Watch Dog)

- 윈도우 와치독 : 응용 프로그램이 정상적인 순서를 벗어나게하는 외부 간섭이나 예기치 않은 논리 조건으로 인해 발생하는 소프트웨어 오류 발생을 감지하는데 사용

- 프로그램이 WWDG 다운 카운터 값을 새로 갱신하지 않으면, 프로그래밍 시간이 만료되었을 때 MCU 리셋함 

  - 이렇게 리셋되지 않도록 일정한 주기로 WWDG 다운 카운터 갱신하여, 일시적 결함 생겨도 시스템 지속되거나, 주어진 시간 이내에 정상 수행 가능한 상태로 복구되는 시스템 적용 필요

- STM32의 MCU는 시간 정밀도와 결합 복원도에서 다른 2개 IWDG(Independent WDG), WWDG(Window WDG) 내장되어있음

- 밑의 예제는 프로그램이 무한 루프 빠졌을 경우 MCU가 리셋되어 시스템 복원되게함 (리셋 안되게 할거라면서...)

- 코드 생성

  - system core - WWDG (activate)

  - parameter settings - watchdog clocking

    - 시간 설정은 https://m.blog.naver.com/eziya76/221518876037 참고

    - 여튼 포인트는 WWDG는 7비트 다운 카운터 타이머

    - refresh 는 window 시간 내에서만 가능 , 이외에는 리셋됨

    - 설정은 0x7F ~ 0x40

    - 7번째 비트가 1 -> 0 되는 시점인 카운터가 0x3F 시점에 리셋됨

    - prescaler 1,2,4,8 가능

    - 공식은

      ```
      Twwdg = Tpclk1 * 4096 * prescaler * (free-running. - window value + 1) (ms)
      ```

      4096은 전압을 12bit resolution(0~4095)으로 쪼갤 수 있어서인듯

      나 같은 경우는 clock  64MHz / 카운터 0x7F / window 0x5F / prescaler 8 로 한다면

      우선 1/64MHz * 4096 * 8 =  512us

      그리고 refresh 안되는 시간은 512*(0x7F - 0x5F +1) = 16.896ms

      WWDG Timeout : 512*(0x7F - 0x3F +1) = 33.280ms
      
      여튼 이게 16.896 ~ 33.280 ms (0x3F time) 구간(window)에서 갱신해야함 아니면 리셋됨
    

- 코드 작성

  - ```c
    uint8_t i = 0;
    
      while (1)
      {
    	  if (i == 40)
    	  {
    		  i =0;
    		  HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    	  }
    	  HAL_Delay(25);
    	  i++;
    
    	  /* Refresh WWDG */
    
    	  if (HAL_WWDG_Refresh(&hwwdg) != HAL_OK)
    	  {
    		  Error_Handler();
    	  }
    ```

  - 외부 인터럽트로 무한루프 빠지게하여 와치독에 의한 부팅 확인

    - user 버튼 활성화 해줘야해 이거 하려면

    - ```c
      MX_USART2_UART_Init();	
      
        if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET)
        {
      	  // wwdg flag set, : LED4 on
      	  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
      
      	  // 4s delay
      	  HAL_Delay(4000);
      
      	  //clear reset flags
      	  __HAL_RCC_CLEAR_RESET_FLAGS();
        }
        else
        {
      	  // wwdg flag is not set : LED4 off
      	  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
        }
      
        MX_WWDG_Init();
      
      ...
          
      
      /* USER CODE BEGIN 4 */
      void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
      {
      	while (1)
      		;
      }
      ```



### USART (Universal Synchronous Receiver/Transmitter)

- Tx 출력 기능만 이용하면 polling 방식도 괜찮

- 통신으로 Rx 입력 받으면 polling 방식으로는 수신 데이터 유실 및 프로그램 구현에 에로사항

- USART 수신 인터럽트로 수신 데이터를 링 버퍼(ring buffer)에 저장하고 링 버퍼 데이터를 읽어서 송신하는 Echo 예제

- 코드 생성

  - USART 세팅
    - 설정값, NVIC enable
    - HAL_UART_IRQHandler() 코드는 없앰 (NVIC - Code generation의 USART2 global interrupt 의 Call HAL Handler 해제)
  - 클럭설정 적당히

- 코드 작성

  - 'ring buffer source' 구글링으로 코드 써도됨

    - 참고하는 책에서 제공하는 라이브러리 파일 다운

  - ~ it.c 에서

    ```c
    /* USER CODE BEGIN USART2_IRQn 0 */
    	if ((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_RXNE) != RESET))
    	{
    		HAL_UART_RxCpltCallback(&huart2);
    		__HAL_UART_CLEAR_PEFLAG(&huart2);
    	}
    ```

  - main.c

    ```c
    /* Private variables ---------------------------------------------------------*/
    UART_HandleTypeDef huart2;
    /* USER CODE BEGIN PV */
    
    /* USER CODE END PV */
    RingFifo_t gtUart2Fifo;
    
     /* USER CODE BEGIN 2 */
      __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
      if (RB_init (&gtUart2Fifo, 16)) //16bytes , 링 버퍼 크기 2의 지수승 (통신 프로토콜에 맞게 넉넉히), 링 버퍼 메모리 할당할 때 malloc 함수 사용하기 때문에 CSTACK 사이즈 고려할것
      {
    	  //assert(0);
      }
    
    /* USER CODE BEGIN WHILE */
      uint8_t ch;
      while (1)
      {
    	  if (!RB_isempty (&gtUart2Fifo))   // 수신되면 링 버퍼 비어 있지 않아서 0 반환
    	  {
    		  ch = RB_read (&gtUart2Fifo);	// 링 버퍼의 데이터 읽기
    		  HAL_UART_Transmit (&huart2, &ch, 1, 0xFF);	// 송신
    	  }
    
    
    /* USER CODE BEGIN 4 */
    void HAL_UART_RxCpltCallback (UART_HandleTypeDef *UartHandle)
    {
    	uint8_t rx;
    	
    	if (UartHandle -> Instance == USART2)
    	{
    		rx = (uint8_t) (UartHandle -> Instance -> RDR & (uint8_t) 0x00FF);
    		RB_write (&gtUart2Fifo, rx);
    	}
    }
    ```

- 실행 후, tera term - 제어 - 명령전송 

  - 실시간 모드, enter 키 해제 / 이 프로세스에만 보내기 체크 후 제출 (여기 입력 값에 따라 링 버퍼 크기를 설정하는거지)



### TIM_PWM (Pulse width Modulation)

- PWM 이란?

  - 디지털 신호는 0과 1 뿐임 (ex. 형광등을 on/off 만 가능하겠지?)
  - 여기 사잇값을 가지게 할 수 있는게 PWM인거임 (ex. 형광등 밝기를 조절할 수 있게 되는거지)
    - 어떻게? 0과 1의 비율로! 이게 바로 Duty Cycle 임

- 타이머 이용 PWM 출력 예제

- 코드 생성

  - parameter
  - https://dkeemin.com/stm32f0xx-pwm-%EC%84%A4%EC%A0%95-%EB%B6%80%EB%A1%9D-%EC%84%A4%EB%AA%85/
  - RCC, clock
  - TIM 
    - clock source 는 internal
    - channel1 은 PWM generation ch1
    - parameter
      - 주기 1kHz, 듀티비 50%로 해보자
      - prescaler = APBx timer clock / Timer counter clock -1
        - 난 APB2 80MHz , Timer counter clock을 1MHz 로 할거야 prescaler는 79
      - counter period = timer counter clock / output clock -1
        - output clock 은 1KHz 로~ / counter period 는 999
      - pulse = (counter period +1)  *(duty ratio/100) 
        - 500

- 코드 작성

  - PWM 시작

    ```c
    /* USER CODE BEGIN 2 */
      // start channel 1
      if (HAL_TIM_PWM_Start (&htim1, TIM_CHANNEL_1) != HAL_OK)
      {
    	  Error_Handler();
      }
    ```

- 오실로스코프 파형 확인 (PA8 핀임)

  - 근데 그냥 안할래 다른걸로 나중에 해보지 뭐





### I2C (Inter-Integrated Circuit)

- https://m.blog.naver.com/PostView.nhn?blogId=eziya76&logNo=221484861357&referrerCode=0&searchKeyword=i2c
- https://igotit.tistory.com/entry/I2C-Bus-%EA%B8%B0%EB%B3%B8%EA%B0%9C%EB%85%90
- SDA (data선), SCL(clock 선)만으로 연결 가능
- 마스터와 슬레이브들 연결 **(로직때문에 풀업저항이어야만함)**
- 프로토콜
  - 컨트롤 용으로 사용되며 저속이기 때문에 전용 HW 없이 일반 GPIO로도 구현 가능
  
  - slave를 지정할 address 필요
  
  - 7-bit의 address (10-bit 도 존재) 
  
  - 통신 속도는 5개 모드
  
    - non busy 일 때, 데이터 전송 (SCL-High, SDL-Low)
  
  - master에 의해서 시작과 종료
    - start condition (S) : SCL high 이고 SDA high -> low
    - stop condition (P) : SCL high 이고, SDA low -> high
    - restart condition : stop 대신 start 전송되면 버스에 계속 데이터 전송된다는 의미래
    
  - I2C 프로토콜은 address frame과 data frame 으로 구성
    - address : 7bit 주소 + 1bit R/W flag (1이 read, 0이 write)
      - 0일 때, master가 address frame 전송 후, 다시 data frame 전송
      - 1일 때, master가 address frame 전송 후, slave가 data frame 전송
      
    - 바이트 크기로 전송되는 데이터 frame의 크기는 제약 없음, 매 바이트 전송 후 ACK 신호(정상 수신, SCL-high, SDA-low) 또는 NACK(수신 오류, SCL-high, SDA-high) 상태 확인 
      
      - NACK 일 때, master 장치는 stop or restart 가능
      
    - Write !!
    
      ![image-20210216090551002](C:\Users\JJW_N-771\Desktop\stmpjt\공부\STM32.assets\image-20210216090551002.png)
      
    - READ !!
    
      ![image-20210216090729264](C:\Users\JJW_N-771\Desktop\stmpjt\공부\STM32.assets\image-20210216090729264.png)
  
- 일부 slave 장치들은 데이터 전송 전에 데이터 처리가 끝나지 않은경우, SCL 라인을 low로 유지해서 master 장치가 다음 데이터를 전송하지 못하게 hold 할 수 있음 (clock stretching 이라고 함)
- 코드 생성
  - https://igotit.tistory.com/702
  - 스피드 모드 선택
  - GPIO 세팅에서 외부 장치가 있으면 nopull
- 코드 작성
  - HAL_I2C_Mem_ 이게 EEPROM(비휘발성)만 가능한건지는 모르겠음
    - 여튼 여러 함수가 있는데 그 어떤 I2C 시퀀스를 원하냐에 따라 선택하면되는듯



### USB

- USB CDC(Communication Device Class) 로 사용하기
- 코드 생성
  - USB - mode - device
  - middleware - usb_device - mode - communication device class
  - device descriptor
    - VID 설정
      - https://usb.org/sites/default/files/vendor_ids051920_0.pdf 에서 stmicro.. 검색해보면 1155 임
  - clock 에 빨간불 남
    - USB FS(full speed)로 쓰려면 48MHz 로 설정해야함

- 코드 작성

  - usbd_cdc_if.c 파일에서 데이터 수신 시 발생하는 CDC_Receive_FS 이벤트 핸들러를 수정하여 수신 버퍼의 마지막에 NULL 문자열을 추가

    ```c
    static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
    {
      /* USER CODE BEGIN 6 */
      USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
      Buf[*Len] = 0;
      USBD_CDC_ReceivePacket(&hUsbDeviceFS);
      return (USBD_OK);
    ```

  - main.c
    UserRxBufferFS, UserTxBufferFS 의 extern 선언을 추가한다. 
    while 루프 내부에서 수신 버퍼에 새로운 데이터가 들어온 경우 해당 Rx 버퍼의 내용을 그대로 TxBuffer에 추가하여 전송하도록 코드를 추가

    ```c
    /* Private variables ---------------------------------------------------------*/
    
    /* USER CODE BEGIN PV */
    #define APP_RX_DATA_SIZE  64
    #define APP_TX_DATA_SIZE  64
    extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
    extern uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];
    ```

    

## FreeRTOS

- 무료 제공 RTOS (MIT 라이센스)
- https://m.blog.naver.com/PostView.nhn?blogId=oh930418&logNo=221152205204&proxyReferer=https:%2F%2Fwww.google.com%2F
- https://github.com/d-h-k/Learing-RTOS-with-STM32 (나 같네)
- 참고자료
  - RTOS(Real Time Operating System) & FreeRTOS Porting.pdf 에 한글로 이론적인 것들 설명 되어있음
  - user_manual with RTOS.pdf 에서 고려사항들? 확인 가능
  - Mastering_the_FreeRTOS_Real_Time_Kernel-A_Hands-On_Tutorial_Guide.pdf
    - 각 요소들?의 설명들이 잘 나와있다
  - FreeRTOS_Reference_Manual_V10.0.0.pdf
    - 각 함수들의 설명 

### CBD (component based development) 개발 방법론 적용

- 속한 팀의 룰을 따르기 
- 한 개의 컴포넌트는 task, module, driver 로 구성됨
- task
  - task 간 통신 (스케쥴러가 제어할 대상)
- module
  - driver에서 구성한 함수 등을 이용하여 기능을 수행
- driver
  - 레지스터를 어떻게 쓸지 정의해놓음
- Ex. 어떤 버튼 누르면 온도 측정 데이터 받기
  - driver에 온도 센서의 레지스터를 제어하는 코드가 있음
  - 온도를 측정 데이터를 I2C로 수신하는 함수를 module에 작성
  - 인터럽트 걸리면 task 걸리게 하고 이 task 에서 작성한 모듈로 처리 / 필요하다면 완료라는 printf를 띄워주기 위해 printf task 에 통신을 보내던가



### 기본

#### task와 thread 생성

- **osThreadDef**(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = **osThreadCreate**(osThread(defaultTask), NULL);

  ```c
  osThreadDef(name, thread, priority, instances, stacksz)  \
  const osThreadDef_t os_thread_def_##name = \
  { #name, (thread), (priority), (instances), (stacksz)}
  ```

  - name : 생성할 테스크의 이름
    thread : 동작시킬 함수
    priority : 테스크의 우선순위 (하단에 우선순위 표 첨부)
    instances : 동일한 osThreadDef에 대해 선언 될 수 있는 횟수
    stacksz : 테스크에 사용할 스택 크기 (보통 configMINIMAL_STACK_SIZE를 기준으로 사용한다)

  ```c
  osThreadCreate (const osThreadDef_t *thread_def, void *argument)
  ```

  - *thread_def : 생성할 osThreadDef
    *argument : 테스크 함수에 넣을 매개변수

- xTaskCreate


  ```c
  xTaskCreate( TaskFunction_t pxTaskCode,
         const char * const pcName,
         const uint16_t usStackDepth,
         void * const pvParameters,
         UBaseType_t uxPriority,
         TaskHandle_t * const pxCreatedTask )
  ```

  - pxTaskCode : 동작시킬 함수
    pcName : 생성할 테스크의 이름
    usStackDepth : 테스크에 사용할 스택 크기 (보통 configMINIMAL_STACK_SIZE를 기준으로 사용)
    pvParameters : 테스크 실행시 전달할 매개변수
    uxPriority : 테스크의 우선순위 (숫자가 클수록 우선순위 높음)
    pxCreatedTask : 테스크 핸들을 지정 (TaskHandle_t xTaskHandle;와 같이 선언한 테스크 핸들을 &xTaskHandle와 같이 주소값을 이용하여 지정, 다른 테스크의 핸들을 이용하여 우선순위 변경 등의 작업이 가능)
    테스크 함수, 스택크기, 우선순위를 제외한 나머지 값은 null로 입력해도 상관없다.
    (예:TaskCreate( vTask1, null, configMINIMAL_STACK_SIZE, NULL, 2, NULL ); 

- Ex.

  - ```c
    // 함수 만듦
    void vPrint_Task()
    {
      for (;;)
      {
        printf("task : %lu\r\n", osKernelSysTick());
        osDelay(1000);
      }
    }
    
    osThreadId Print_Handle_1
    
    // (1)
    osThreadDef(myTaskName_1, vPrint_Task_1, osPriorityNormal, 0, 128);
    Print_Handle_1 = osThreadCreate(osThread(myTaskName_1), NULL);
    
    // (2)
    xTaskCreate(vPrint_Task, "myTaskName" , configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    ```

- 나는 CMSIS_RTOS API 말고  freeRTOS API 쓸 거임

  - ```c
    typedef 
    {
        char member1;
        char member2;
    } xStruct;
    
    xStruct xParameter = {1,2};
    
    void vTaskFunc (void * pvParameters)
    {
        xStruct pxParameters;
        pxParameters = (xStruct *) pvParameters;
        
        for (;;)
        {
            if (pxParameters->member1 !=1)
            {
                //code
                ;
            }
        }    
    }
    
    
    void init()
    {
        TaskHandle_t xHandle;
        if (xTaskCreate(vTaskFunc, "Demo task", STACK_SIZE, (void*) &xParamter, TASK_PRIORITY, &xHandle) != pdPASS)
        {
            
        }
        else
        {
    		// 생성됨, 그리고 priority 같은거 바꾸기 가능
            vTaskPrioritySet(xHandle, 2)
        }
    }
    ```

  - 

- RTOS 초기화 및 스케줄러 실행 (MX 가 해줌)

  - main.c

    ```c
      /* Call init function for freertos objects (in freertos.c) */
      MX_FREERTOS_Init();
    
      /* Start scheduler */
      osKernelStart();
    ```



#### Queue, Message, Mail

- Queue 를 이용해서 task 간에 통신 (함수에서 전역변수 쓰는 느낌임)
- https://www.freertos.org/a00116.html

#### Semaphore, mutex

- https://iredays.tistory.com/125
  race condition 에 대한 설명과 세마포어와 뮤텍스의 필요
- https://sanghyunj.tistory.com/19
  세마포어와 뮤텍스의 차이와 구현원리



#### Task Notifications

- task 나 ISR 과의 커뮤니케이션을 위해 queue, message, semaphore 등을 썼는데 이것들은 communication object 라는걸 썼음
- 이거는 communication object 없이 직접적으로 이벤트를 날린다



#### Interrupt

- FreeRTOS API 는 Task 와 ISR 으로부터 불려지는 2가지 함수를 제공함



#### API

- CMSIS-OS 는 여러 종류의 RTOS 를 사용할 때 표준적인 입출력 수단으로 중간계층에 해당한대. 

- 그래서 얘네 API 쓰면 되는데 다른 RTOS porting 하기 쉬워지긴하는데 어쨌든 지금은 FreeRTOS API 를 직접 호출해서 쓸 거니까 매핑되는 것들을 조금씩 정리해본다 (귀찮다)

- https://www.freertos.org/a00106.html (FreeRTOS API)

  https://m.cafe.daum.net/easyflow/F13G/search?r=https%3A%2F%2Fm.cafe.daum.net%2Feasyflow%2FF13G%3FboardType%3D&query=freeRTOS

|      | FreeRTOS               | CMSIS-OS          |
| ---- | ---------------------- | ----------------- |
|      | xTaskCreate            | osThreadCreate    |
|      | vTaskDelay             | osDelay           |
|      | vSemaphoreCreateBinary | osSemaphoreCreate |
|      | xSemaphoreCreateMutex  | osMutexCreate     |
|      | xQueueCreate           | osMessageCreate   |
|      |                        |                   |
|      |                        |                   |
|      |                        |                   |
|      |                        |                   |









## 일단 내가 모르고 넘어가는 것들



- IDE 에서 run 했을 때, download 가 안되는 경우가 있음. IDE의 버그인지...(실제로 문의글이 많지만 해결은 안되고있는걸로 봄) 디버그까지 행하다가 안되는지 알수는 없는데 여튼 이럴 때, cube Programmer 로 파일 다운해줘서 해결

  