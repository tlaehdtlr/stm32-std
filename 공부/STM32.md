## software tools

#### [STM32 Cube]

\1. Java JDK

- [ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1.%20tool/Java/jdk-8u241-windows-x64.exe](ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1. tool/Java/jdk-8u241-windows-x64.exe)

\2. STM32CubeIDE

- [ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1.%20tool/STM32CubeIDE/st-stm32cubeide_1.5.0_8698_20201117_1050_x86_64.exe](ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1. tool/STM32CubeIDE/st-stm32cubeide_1.5.0_8698_20201117_1050_x86_64.exe)

\3. STM32CubeMx

- [ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1.%20tool/STM32CubeMX/SetupSTM32CubeMX-6.1.1.exe](ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1. tool/STM32CubeMX/SetupSTM32CubeMX-6.1.1.exe)

\4. STM32CubeProg

- [ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1.%20tool/STM32CubeProg/SetupSTM32CubeProgrammer_win64_v2.6.0.exe](ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1. tool/STM32CubeProg/SetupSTM32CubeProgrammer_win64_v2.6.0.exe)

\5. STM32CubeMon

- [ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1.%20tool/STM32CubeMon/setupSTM32CubeMonitor_1.1.0.exe](ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1. tool/STM32CubeMon/setupSTM32CubeMonitor_1.1.0.exe)

#### [Driver]

\1. Prolific USB to UART Driver

- [ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1.%20tool/USB%20to%20UART/PL23XX_Prolific_DriverInstaller_v200.zip](ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1. tool/USB to UART/PL23XX_Prolific_DriverInstaller_v200.zip)

\2. ST-Link USB driver

- [ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1.%20tool/ST-Link/USB%20Driver/en.stsw-link009.zip](ftp://192.168.19.225/pub/tools/sdk/stm/stm32/1. tool/ST-Link/USB Driver/en.stsw-link009.zip)



## STM32 실행

#### 목표

- 사용자가 작성한 코드를 (.c 파일) 이진코드 (.bin)으로 바꿔서 STM32 (flash? 레지스터? 찾아봐 여튼 보드 메모리 공간에 보낸다)
- .c 파일을 빌드하여 object (.o) 파일로 바꾸고 link 작업을 통해 binary code로 바꾼다



#### 순서 (회로도 - pinmap - 코드생성 - 코드작성 - 다운로드 - 모니터링)

https://ndb796.tistory.com/360 여기 굿

- STM32 cube(IDX, MX, Prog, Mon) 전부 설치 (java eclipse 기반으로 만들어졌기 때문에 java 설치 선행 필요)
- STM32를 컴퓨터와 연결
  - 장치관리자 - 범용 직렬 버스 장치에 ST_link debug 생성됨
- CubeMX 실행 (회로도를 보고 핀 들을 제어하는 기본적인 코드를 만들어준다?)
  - new project
    - 3가지 선택
    - 첫 번째는 새로 만든 하드웨어라면 선택
    - 두 번째는 건들지 않은거
    - 세 번째는 뭔가 템플릿 있는거 같음
  - board selector
    - 내 보드 검색해서 찾아줘
    - 프젝 시작 (뭐 초기화 시켜줄까 하는데) 일단 나는 오케이
  - 프로젝트 설정
    - 이름, 경로
    - IDE 선택, generate under root 체크 해제 (이유는 다음에 알려주신다했음)
    - MCU 패키지 필요한 것만 카피
    - peripheral 마다  .c/.h 로 초기화 생성 체크
  - 코드 생성
  - IDE 찾아주고 
- IDE 에서 코드 작성 및 실행
  - 프젝 생성
  - 어플 - 유저 - 코어 - main.c 작성
  - 프로젝트 빌드
  - 실행

#### 용어

- HAL library (Hardware Abstraction Layer)
  - 하드웨어 추상화 계층 (MCU가 핀을 제어하는 함수들을 만듦
- 



#### IDE

- 함수 블럭 후, F3 누르면 함수의 정의로 감
- 





## CubeMX 

- 회로도를 보고 Pinout Configuration
- 핀들 설정해주라고... 그리고 추가 옵션들이나 이름 달아주기
- 클럭 설정
- 코드 생성



- RCC (Reset Clock Controller)
  - stm32의 리셋과 클럭을 관장
  - 



### Clock

- 구성 요소?
  - HCLK : Core Clock 으로 실제 소스 코드 동작시키는 clock
  - SYSCLK : System Clock 으로 Power on reset 직후 무조건 내부 clock으로 먼저 동작
  - HSE (High Speed External) : 외부 고속 Clock, stm32 외부에 Crystal/Ceramic resonator 필요, Duty가 50% 이하인 외부 구형파, 삼각파 신호로도 사용 가능
  - HSI (High ... internal) : 내장된 RC 발진 회로에 의해 동작 clock, 자체 calibration 기능 있지만, RC 발진회로의 특성 문제로 온도 상승에 따른 오차 발생
  - LSE (Low ... ) : 32.768kHz의 Cryst.... 사용, 용도는 저전력 구현 및 정확한 시간 (RTC)을 맞추기 위함
  - LSI () : independent watchdog와 AWU(Auto Wakeup) 기능 및 RTC clock에 사용 (정확성 확보 어려움)
  - MSI (Multi Speed internal) : 저전력이라는데 음?
  - CSS (Clock Security System) : HSE clock 에 문제 발생시, NMI interrupt 발생 및 clock source 를 HSI clock으로 변경해주는 기능

- 고려 사항?
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





## Peripheral

#### main.c

- 자동 생성된 메인 함수의

  - ```c
    // HAL 라이브러리 초기화
    HAL_Init();
    // 시스템 클럭 설정
    SystemClock_Config();
    // 설정한 GPIO 부분 함수
    MX_GPIO_Init();
    
    ```

    



#### 참고

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







#### Timer

- clock 설정
  - HSE(high speed external), HSI(high speed internal),  LSE(low ...), LSI
  - high와 low는 메가와 킬로인 듯?
- 과정
  - system core - RCC - LSE(뭐 위에거 설정)
  - 핀 바뀜 (이걸 pin out 이라 하는걸까)
  - Timers - RTC 에 Activate clock source 활성화
  - Clock Configuration 에서 RTC Clock Mux 를 LSE(다른 것도)로 선택
- 종류가 여럿...
  - SysTick timer(항상 동작) 이런게 HAL_Delay() 함수 등에 사용됨
  - WatchDog timer(IWDG, WWDG) : CPU의 오동작을 탐지하여 문제가 발생하면 재부팅 시켜주는 타이머
  - Basic timer : 입출력 기능없이 시간만 재는 놈 TIMx (x:6,7)
  - General Purpose timer : 범용 타이머, 출려 비교, 원펄스, 입력 캡쳐 등 TIMx (x: 2~5, 9~14)
  - Advanced-control timer : 모터 제어나 디지털 변환 TIMx (x:1,8)
  - 뭐를 고를지 고려해봐
    - 카운터 해상도(크기), DMA 사용 여부, 최대 인터페이스 속도
    - 카운터 타입, 캡쳐/비교 채널수
    - Prescaler 범위, 보상 출력 유무, 최대 타이머 클럭
- 주요 타이머 레지스터
  - counter register (Timx_CNT) : 카운터 값 자체를 저장
  - Precaler reguster (TIMx_PSC) : 분주비 레지스터 (분주할 비를 설정)
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
  - 최대 clock 설정
  - NVIC 설정에 Systick 타이머 설정 확인
  - 코드 생성
  - 1초단위로 UART 메시지를 출력하는 코드 설계
  - UART 메시지 확인





#### I2C (Inter-Integrated Circuit)