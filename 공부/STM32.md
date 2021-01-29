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



---

### 기타

#### 용어

- HAL library (Hardware Abstraction Layer)
  - 하드웨어 추상화 계층 (MCU가 핀을 제어하는 함수들을 만듦



#### Clock

- 심장이다 (에너지를 공급하는거지)
  - 무조건 최대말고 저전력을 요구하면 좀 작게 할 필요가 있겠지?

##### 구성 요소?

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

##### 고려 사항?

- HSI/HSE는 시스템 clock 소스로 사용
  - 바로 사용되지 않고 clock tree를 통해 PLL이나 Prescaler 를 통해 필요한 주파수로 변경 후 사용
- LSI/LSE 는 RTC 와 Independent Watchdog 용으로 사용
- External 은 Bypass Clock source 또는 Crystal/ceramic resonator 로 설정 가능
  - bypass 는 다른 장치에서 clock 을 전달 받아 사용할 때 선택
  - crystal 은 외부에 clock 회로를 구성한 경우 선택
- master clock output 을 설정하면 특정 clock source 를 다시 특정 pin으로 출력하여 다른 주변 IC Clock Source로 사용 가능

##### HAL API

- clock 초기화 시, RCC_OsclnitTypeDef, RCC_ClklnitTypeDef 구조체 변수 사용
- HAL_RCC_OscConfig 함수는 Main PLL 설정하여 PLLCLK 설정
- HAL_RCC_ClockConfig 함수는 PLLCLK 가 SYSCLK 으로 사용되도록 설정
- HAL_SYSTICK_Config 함수는 SysTick 타이머가 1ms 마다 구동하도록 설정
- HAL_RCC_EnableCSS 함수는 HSE 에 오류 발생 시, 이를 검출하여 NMI Exception 을 발생시킴
- HAL_RCC_MCOConfig 함수는 PLLCLK 클럭이 MCO 핀으로 출력되도록함



#### DMA (Direct Memory Access)

- https://m.blog.naver.com/PostView.nhn?blogId=eziya76&logNo=221436500639&proxyReferer=https:%2F%2Fwww.google.com%2F

#### RCC (Reset Clock Controller)

#### CRC

#### NVIC (Nested vectored interrupt controller)





## Peripheral

#### 코드 참고

- C:\Users\유저\STM32Cube\Repository\STM32Cube_FW_L4_V1.16.0\Projects\NUCLEO-L412RB-P\Examples
  - 여기 참고 하면됨



#### GPIO (General Purpose Input/Output)

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

#### UART (Universal Asynchronous Receiver/Transmitter)

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

    



#### EXTI (External Interrupt)

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







#### Timer 기본

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

  

#### ADC (Analog to Digital Converter)

- 온도센싱 값 프린트 찍기

- 코드 생성

  - RCC 설정
  - ADC 설정
    - temperature sensor channel
    - 연속으로 AD 변환위해 continuous conversion  mode 를 enable
    - ADC_Regular_ConversionMode - Rank - Sampling Time 을 12.5 Cycles로 (이거는 왜함?)
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



#### WWDG (Window Watch Dog)

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



#### USART (Universal Synchronous Receiver/Transmitter)

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



#### TIM_PWM (Pulse width Modulation)

- PWM 이란?

  - 디지털 신호는 0과 1 뿐임 (ex. 형광등을 on/off 만 가능하겠지?)
  - 여기 사잇값을 가지게 할 수 있는게 PWM인거임 (ex. 형광등 밝기를 조절할 수 있게 되는거지)
    - 어떻게? 0과 1의 비율로! 이게 바로 Duty Cycle 임

- 타이머 이용 PWM 출력 예제

- 코드 생성

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





#### I2C (Inter-Integrated Circuit)





## 일단 내가 모르고 넘어가는 것들



- IDE 에서 run 했을 때, download 가 안되는 경우가 있음. IDE의 버그인지...(실제로 문의글이 많지만 해결은 안되고있는걸로 봄) 디버그까지 행하다가 안되는지 알수는 없는데 여튼 이럴 때, cube Programmer 로 파일 다운해줘서 해결
- IWDG랑 WWDG 기간 구하는 공식의 4096 (0xFFF +1) 의 값이긴 해. 근데 이게 뭔지 잘 모름