# :rocket: Custom Bootloader

:bulb: MCU마다 arm core, flash memory map, Cube MX 버전, HAL 코드 생성의 차이로 인해 고려사항이 있으니 변동 사항 발생할 수 있음



## :large_blue_diamond: Jump to address

- Bootloader 에서 DFU 조건이 아니면 Application을 실행시키기 위해 필요
- Peripheral, clock, IRQ 등 초기화
- Stack pointer (arm core M4 는 full descending 을 사용) 초기화
- Reset vector 로 이동



## :large_blue_diamond: DFU

- MCU 의 flash 에 따라 erase, read, write 방식이 달라짐
- Dual bank 도 지원 여부 확인



## :large_blue_diamond: Linker file

- STM32CubeIDE - STM32xxxx_FLASH.ld

- MEMORY 의 FLASH origin, length 수정 (Bootloader, Application 둘 다)



## :large_blue_diamond: Verify

- Programmer 활용 (after programming)
  - MCU flash 와 application binary file 을 1byte 씩 확인한다.
- MCU (during programming)
- CRC
  - CRC 값을 binary로 저장하고 특정 flash 에 write
  - DFU 이후 crc 값을 위의 값과 비교



## :large_blue_diamond: Secure functionalities

- Add secure value binary file
  - Private key, version check etc.
  - 특정 flash 영역 (previous, current) 저장, 비교



## :large_blue_diamond: Application

- reset handler 이후 main 이 불리면 systeminit 함수가 호출된다.
  - Vector table offset 설정을 위해 system_stm32xxxx.c 의 VECT_TAB_OFFSET 을 시작 주소로 변경



## :large_blue_diamond: STM32CubeProgrammer

#### Memory & File edition

- device memory 와 binary file 비교

#### Erasing & Programming

- start address 를 application start address 로 지정

#### Log

- verbosity level 을 3으로 조정하면 자세한 로그를 확인 가능 (리턴 값, 실제 data 등)



## :large_blue_diamond: MCU

- Custom bootloader 를 생성해봤던 MCU Spec  참고하여 비슷한 종류 활용



#### STM32F412ZG

- Flash (1MB, 12 sector) : 4(sec.)x(16KB), 1(sec.)x(32KB),  7(sec.)x(64KB),
- SRAM (256KB)

#### STM32L476 / STM32L496

- https://github.com/akospasztor/stm32-bootloader 참고

#### **STM32L053R8Tx / STM32L052K8Ux**

- Flash (64KB, 16 sector) : 4(sec.)x(4KB), 12(sec.)x(4KB),
- SRAM (8KB)
- http://devops.em-tech.com/confluence/display/HDSW/3.3.+Custom+Bootloader 참고

