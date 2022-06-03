# :rocket: STM32F413ZG Bootloader

:bulb: Specification

- Flash (1.5MB, 16 sector) : 4(sec.)x(16KB), 1(sec.)x(64KB),  11(sec.)x(128KB),
- SRAM (320KB)



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
    - Bootloader (sector 0~4), User (sector 4~5), Application (6~10), Backup (11~15)
    - USBD_DFU_APP_DEFAULT_ADD : 0x08040000
    - USBD_DFU_MEDIA Interface : `04*016Ka,01*064Kg,11*128Kg`
      - 여기 설정 잘못으로 인해 삽질한 기록...
        https://community.st.com/s/question/0D53W00001EGm5iSAD/stm32cubeprogrammer-erase-a-strange-address-when-i-use-a-custom-bootloader-and-usb-dfu

#### Clock

- USB clock 을 48MHz 로 맞춰줌



### 2. GPIO, UART, FLASH

#### LED

- LED : 동작 상태 (DFU mode, application etc.)

#### UART

- Debug 용

#### Flash

- DFU 진입 조건



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

- /n100_application.bin 로 DFU 가능

- 수정 부분 참고

  - http://developers.em-tech.com/gerrit/#/c/2600/

- linker file  설정
  memory, section 할당

  ```
  MEMORY
  {
    RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 256K
    FLASH    (rx)    : ORIGIN = 0x8040000,   LENGTH = 640K
  }

  SECTIONS
  {
    /* custom integrity */
    .dfu_integrity 0x80DFC00:
    {
      . = ALIGN(4);
      KEEP(*(.dfu_integrity));
      . = ALIGN(4);
    } > FLASH

    /* The startup code into "FLASH" Rom type memory */
    .isr_vector 0x8040000:
    {
      . = ALIGN(4);
      KEEP(*(.isr_vector)) /* Startup code */
      . = ALIGN(4);
    } >FLASH

    .dfu_security :
    {
      . = ALIGN(1024);
      KEEP(*(.dfu_security));
      . = ALIGN(4);
    } >FLASH

    .metainfo :
    {
      . = ALIGN(1024);
      KEEP(*(.metainfo));
      . = ALIGN(4);
    } >FLASH
  }
  ```



- system_stm32f4xx.c

  ```c
  #define VECT_TAB_OFFSET  0x40000
  ```

- metainfo.c

  ```c
  #define DFU_KEY     "1234567890abcdefghij1234567890!"
  #define DFU_VERSION 10
  #define DFU_COMPLETE  0x99

  uint32_t dfu_complete __attribute__((section(".dfu_integrity"))) = DFU_COMPLETE;

  typedef struct
  {
    uint8_t dfu_key[32];
    uint16_t dfu_version;
  } dfu_secure_t;

  dfu_secure_t dfu_secure_info __attribute__((section(".dfu_security"))) =
  {
    .dfu_key = DFU_KEY,
    .dfu_version = DFU_VERSION
  };
  ```







