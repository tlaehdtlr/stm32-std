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
- 



#### UART (Universal Asynchronous Receiver/Transmitter)

- USART 는 Synchronous 즉 등기인데 UART와의 차이는 Clock 소스 유무임
  - USART 통신은 동기화 Clock에 따라 데이터 전송
- Uart 통신은 협의된 baud rate (보드 속도, 비트/초의 단위인 데이터 전송 속도를 뜻함) 에 따라서 T1bit 간격으로 샘플링된 값을 데이터로 수신하며 start 비트와 stop 비트는 1.5bit 길이를 가짐
- 

