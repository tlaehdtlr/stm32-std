# :rocket: STM32L053R8Tx Bootloader

:bulb: Specification

- Flash (64KB, 16 sector) : 4(sec.)x(4KB), 12(sec.)x(4KB),
- SRAM (8KB)



참고 : http://devops.em-tech.com/confluence/display/HDSW/3.3.+Custom+Bootloader



## :large_blue_diamond: CubeMX 설정

### 1. USB DFU 활성화

#### RCC

- HSE clock - BYPASS Clock Source

#### USB_OTG_FS

- Device only

#### USB_DEVICE

- Class For FS IP
  - Download Firmware Update Class (DFU)
- DFU
  - parameter 설정
    - Bootloader (sector 0~3), Application (4~15)
    -  USBD_DFU_APP_DEFAULT_ADD : 0x08004000
    - USBD_DFU_MEDIA Interface : `128*128Ba,384*128Bg` - Page단위인 128의 크기로 맞춰줘야 동작

#### Clock

- USB clock 을 48MHz 로 맞춰줌



### 2. 프로젝트 생성

- Project manager - Advanced settings
  - MX_USB_DEVICE_init 체크 해제



## :large_blue_diamond: Linker

- ```
  MEMORY
  {
    RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 8K
    FLASH    (rx)    : ORIGIN = 0x8000000,   LENGTH = 16K
  }
  ```



## :large_blue_diamond: main.c

- DFU condition
  - eeprom check
- Jump to application
  - Peripheral deinit
  - Initialize clock, stack pointer
  - jump to address



## :large_blue_diamond: DFU flash

- main.c에서 eeprom의 특정 위치(예시에서는 32bit idx 100)값을 읽어 해당 값이 0x98765432라면 DFU모드 진입, 아닐 시 Application 동작 수행

- USB_DEVICE\App\usbd_dfu_if.c 수정

  - STM32L053RB의 DFU_Standalone의 예제 코드 사용으로 아래의 정의 및 함수들 수정
    
    - ```
      #define FLASH_ERASE_TIME    (uint16_t)50
      #define FLASH_PROGRAM_TIME  (uint16_t)50
      uint16_t MEM_If_Erase_FS(uint32_t Add);
      uint16_t MEM_If_Write_FS(uint8_t *src, uint8_t *dest, uint32_t Len);
      uint8_t *MEM_If_Read_FS(uint8_t *src, uint8_t *dest, uint32_t Len);
      uint16_t MEM_If_GetStatus_FS(uint32_t Add, uint8_t Cmd, uint8_t *buffer);
      ```



## :large_blue_diamond: Application

- *.hex file로 DFU 가능, bin file은 동작 오류 발생

- #define USER_VECT_TAB_ADDRESS 주석을 풀어서 VECT_TAB_OFFSET을 0x4000만큼 움직이도록 설정 해야한다

- dfu 진입을 eeprom의 특정 위치(예시에서는 32bit idx 100)에 flag인 0x98765432를 쓰고 reset을 통해 DFU모드 진입 (쓰는 함수는 eeprom.c 확인)

- **특히 다른 GPIO동작없이 flash의 영역에 설정된 값으로 DFU진입을 구분할 시 이와 같은 방법을 쓴다**

- **custom bootloader와 application을 각각 따로 다운로드를 받아서 동작시켜야 한다**

- 다운로드 이후 다시 USB를 연결하여 flag로 쓴 eeprom의 0x98765432값을 지우고 reset을 하면 완료

- ```
  MEMORY
  {
    RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 8K
    FLASH    (rx)    : ORIGIN = 0x8004000,   LENGTH = 48K
  }
  ```

- ```
  #define USER_VECT_TAB_ADDRESS
  #define VECT_TAB_OFFSET  0x00004000U
  ```
  
- main.c

  ```c
  while (1)
  {
      // TO DO : application make
      ...
  }
  ```

* eeprom.c

  ```
  ...
  
  HAL_StatusTypeDef eeprom_write_bit_32(uint16_t addr_idx, bit_value_32_t *value)
  {
    HAL_FLASHEx_DATAEEPROM_Unlock();
  
    HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_ADDRESS + addr_idx * 4);
  
    HAL_Delay(4);
  
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, EEPROM_ADDRESS + addr_idx * 4, value->ui);
  
    HAL_Delay(4);
  
    HAL_FLASHEx_DATAEEPROM_Lock();
  
    HAL_Delay(5);
  
    return HAL_OK;
  }
  
  /* write value by type, calculating offset and sub index */
  HAL_StatusTypeDef eeprom_write_bit_32_type(void *value, uint16_t idx, bit_type_32_t type)
  {
    bit_value_32_t tmp;
    uint8_t offset;
  
    /* divide idx by type and read value by eeprom */
    switch (type)
    {
      case EEPROM_TYPE_UI:
      {
        tmp.ui = *(uint32_t*)value;
      }
        break;
  
      case EEPROM_TYPE_I:
      {
        tmp.i = *(int32_t*)value;
      }
        break;
  
      case EEPROM_TYPE_US:
      case EEPROM_TYPE_S:
      {
        offset = idx % 2;
        idx = idx / 2;
  
        if (eeprom_read_bit_32(idx, &tmp) != HAL_OK)
        {
          return HAL_ERROR;
        }
  
        if (type == EEPROM_TYPE_US)
        {
          tmp.us[offset] = *(uint16_t*)value;
        }
        else
        {
          tmp.s[offset] = *(int16_t*)value;
        }
      }
        break;
  
      case EEPROM_TYPE_UC:
      case EEPROM_TYPE_C:
      {
        offset = idx % 4;
        idx = idx / 4;
  
        if (eeprom_read_bit_32(idx, &tmp) != HAL_OK)
        {
          return HAL_ERROR;
        }
  
        if (type == EEPROM_TYPE_UC)
        {
          tmp.uc[offset] = *(uint8_t*)value;
        }
        else
        {
          tmp.c[offset] = *(int8_t*)value;
        }
      }
        break;
  
      default:
      {
        return HAL_ERROR;
      }
        break;
    }
  
    /* write target index using eeprom */
    if(eeprom_write_bit_32(idx, &tmp) != HAL_OK)
    {
      return HAL_ERROR;
    }
  
    return HAL_OK;
  }
  
  ...
  ```

* shell.c

  ```
  ...
  
  uint32_t flag = 0x98765432;
  
  if(eeprom_write_bit_32_type((void*)&flag, EEPROM_IDX_DFU, EEPROM_TYPE_UI) != HAL_OK)
  {
      printf("eeprom write error\r\n");
      return false;
  }
  
  NVIC_SystemReset();
  
  ...
  ```

  

