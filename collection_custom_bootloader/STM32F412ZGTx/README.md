# :rocket: STM32F412ZG Bootloader

:bulb: Specification

- Flash (1MB, 12 sector) : 4(sec.)x(16KB), 1(sec.)x(32KB),  7(sec.)x(64KB),
- SRAM (256KB)



## :large_blue_diamond: Security features

- version, key
- integrity
  - 의도하지 않은 DFU 중지, 통신 에러 등 상황 대비
- roll back
  - DFU 에러 상황 발생 시, 이전 버전 되돌림
- initialization
  - bootloader 제외 영역 초기화 기능



## :large_blue_diamond: CubeMX 설정

### 1. USB DFU 활성화

#### RCC

- HSE clock

#### USB_OTG_FS

- Device only

#### USB_DEVICE

- DFU
  - parameter 설정
    - Bootloader (sector 0~4), User (sector 4), Application (5~11)
    -  USBD_DFU_APP_DEFAULT_ADD : 0x08020000
    - USBD_DFU_MEDIA Interface : `04*016Ka,01*064Kg,07*128Kg`

#### Clock

- USB clock 을 48MHz 로 맞춰줌



### 2. GPIO, UART

#### Button, LED

- button : DFU 진입 조건
- LED : 동작 상태 (DFU mode, application etc.)

#### UART

- Debug 용



### 3. 프로젝트 생성

- linker settings 의 minimum heap size 늘려줌 0x1000
- Project manager - Advanced settings
  - MX_USB_DEVICE_init 체크 해제



## :large_blue_diamond: Linker

- ```
  MEMORY
  {
    RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 256K
    FLASH    (rx)    : ORIGIN = 0x8000000,   LENGTH = 64K
  }
  ```



## :large_blue_diamond: main.c

- DFU condition
  - invalid application
  - user button
- Jump to application
  - Peripheral deinit
  - Initialize clock, stack pointer
  - jump to address



## :large_blue_diamond: DFU flash

- USB_DEVICE\App\usbd_dfu_if.c 수정
  - 주의!
    - erase 할 때, start sector 와 sector 갯수(cube programmer 에서 sector 몇개 필요한지 체크하여 erase 명령 보내기 때문에 갯수는 1로 하면됨) 설정
    - read 시, return 값 변경 (programmer 에서 verify 에러 때문)
  - Todo
    - DFU 중간에 USB 탈착 시, 처리
      - 탈착을 이벤트로 받아서 usb deinit
      - timer 이용하여 특정 시간 경과까지 DFU 완료 안 되면 상황 발생 인지
- bootloader.c 추가
  - DFU 관련 기능

- flash.c 추가
  - flash read, write, erase etc.




## :large_blue_diamond: Application

- /application_example.bin 로 DFU 가능

- linker file  설정
  memory, section 할당

  ```
  MEMORY
  {
    RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 256K
    FLASH    (rx)    : ORIGIN = 0x8020000,   LENGTH = 384K
  }

  SECTIONS
  {
    /* custom integrity */
    .dfu_section 0x807FC00:
    {
      . = ALIGN(4);
      KEEP(*(.dfu_section));
      . = ALIGN(4);
    } > FLASH

    /* The startup code into "FLASH" Rom type memory */
    .isr_vector 0x8020000:
    {
      . = ALIGN(4);
      KEEP(*(.isr_vector)) /* Startup code */
      . = ALIGN(4);
    } >FLASH

    /* custom fw */
    .fw_info :
    {
      . = ALIGN(1024);
      KEEP(*(.fw_info))
      . = ALIGN(4);
    } >FLASH
  }



- system_stm32f4xx.c

  ```c
  #define VECT_TAB_OFFSET  0x20000
  ```

- main.c

  ```c
  __attribute__((section(".dfu_section"))) uint32_t dfu_complete = 0x99;

  typedef struct {
      uint8_t private_key[32];
      uint16_t version;
  } fw_info_t;

  __attribute__((section(".fw_info"))) fw_info_t fw_info_sim =
  {
      "1234567890abcdefghij1234567890q",
      81
  };

  ...

  int main(void)
  {
      ...

  	while (1)
  	{
          HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
          HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
          HAL_Delay(500);
      }
  }
  ```







