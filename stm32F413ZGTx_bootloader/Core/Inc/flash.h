/**
  ******************************************************************************
  * @file    flash.h
  * @author  dongsik sim

*/

#ifndef __FLASH_H__
#define __FLASH_H__

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) // Sector 0, 16 Kbytes
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) // Sector 1, 16 Kbytes
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) // Sector 2, 16 Kbytes
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) // Sector 3, 16 Kbytes
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) // Sector 4, 64 Kbytes
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) // Sector 5, 128 Kbytes
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) // Sector 6, 128 Kbytes
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) // Sector 7, 128 Kbytes
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) // Sector 8, 128 Kbytes
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) // Sector 9, 128 Kbytes
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) // Sector10, 128 Kbytes
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) // Sector11, 128 Kbytes
#define ADDR_FLASH_SECTOR_12    ((uint32_t)0x08100000) // Sector12, 128 Kbytes
#define ADDR_FLASH_SECTOR_13    ((uint32_t)0x08120000) // Sector13, 128 Kbytes
#define ADDR_FLASH_SECTOR_14    ((uint32_t)0x08140000) // Sector14, 128 Kbytes
#define ADDR_FLASH_SECTOR_15    ((uint32_t)0x08160000) // Sector15, 128 Kbytes
#define ADDR_FLASH_END          ((uint32_t)0x0817FFFF) // End of Sector 15

#define FLASH_ERASE_TIME    (uint16_t)50
#define FLASH_PROGRAM_TIME  (uint16_t)50

#define FLASH_ADDR_FW_INFO      ((uint32_t)0x400)

/* region where user can use */
#define FLASH_USER_START_ADDR     ADDR_FLASH_SECTOR_5 // 128kb
#define FLASH_IDX_DFU_ENABLE      80

typedef enum
{
  FLASH_DFU_DISABLE,
  FLASH_DFU_ENABLE,
} flash_dfu_t;

HAL_StatusTypeDef flash_init(void);
HAL_StatusTypeDef flash_deinit(void);
void flash_erase(uint32_t startsector, uint8_t nums_sec);
void flash_copy(uint32_t addr_src, uint32_t addr_dest, uint8_t nums_sec);

int16_t flash_read_int_value(uint8_t info_index);
int16_t flash_write_int_value(uint8_t info_index, int16_t info_value);

uint32_t flash_get_sector(uint32_t Address);

#endif