/**
  ******************************************************************************
  * @file    flash.c
  * @author  dongsik sim

*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "flash.h"

#define FLASH_PACKING_SIZE        2
#define FLASH_FLAG_ALL_ERRORS     (FLASH_FLAG_EOP   | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | \
                                   FLASH_FLAG_PGAERR  | FLASH_FLAG_PGPERR  | FLASH_FLAG_PGSERR )

static HAL_StatusTypeDef flash_read(uint32_t base_address, uint8_t offset, uint64_t *buf, uint8_t buf_size);
static HAL_StatusTypeDef flash_write(uint32_t base_address, uint8_t offset, uint64_t *buf, uint8_t buf_size);
static HAL_StatusTypeDef flash_memory_write_value(uint64_t* buffer, uint8_t info_index, int16_t info_value);



/* flash initialize */
HAL_StatusTypeDef flash_init(void)
{
  HAL_StatusTypeDef status;
  status = HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  return status;
}

HAL_StatusTypeDef flash_deinit(void)
{
  HAL_StatusTypeDef status;
  status = HAL_FLASH_Lock();
  return status;
}

void flash_erase(uint32_t startsector, uint8_t nums_sec)
{
  uint32_t sectorerror = 0;

  /* Variable contains Flash operation status */
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef eraseinitstruct;

  /* Get the number of sector */
  eraseinitstruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  eraseinitstruct.Banks = FLASH_BANK_1;
  eraseinitstruct.Sector = startsector;
  eraseinitstruct.NbSectors = nums_sec;
  eraseinitstruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  status = HAL_FLASHEx_Erase(&eraseinitstruct, &sectorerror);

  if (status != HAL_OK)
  {
    printf("erase error %ld sector \r\n", sectorerror);
  }
  return ;
}

void flash_copy(uint32_t addr_src, uint32_t addr_dest, uint8_t nums_sec)
{
  /*  128kb*sectors */
  uint32_t i = 0;
  uint32_t Len = 1024*128*nums_sec;
  for (i = 0; i < Len; i += 4)
  {
    if (HAL_FLASH_Program
        (FLASH_TYPEPROGRAM_WORD, (uint32_t) (addr_dest + i),
         *(uint32_t *) (addr_src + i)) == HAL_OK)
    {
      /* Check the written value */
      if (*(uint32_t *) (addr_src + i) != *(uint32_t *) (addr_dest + i))
      {
        /* Flash content doesn't match SRAM content */
        printf("flash verified fail \r\n");
        return ;
      }
    }
    else
    {
      printf("Fail write flash \r\n");
      /* Error occurred while writing data in Flash memory */
      return ;
    }
  }
}


uint32_t flash_get_sector(uint32_t Address)
{
  uint32_t sector = 0;

  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;
  }
  else if ((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;
  }
  else if ((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;
  }
  else if ((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;
  }
  else if ((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;
  }
  else if ((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_SECTOR_11;
  }
  else if ((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_SECTOR_12;
  }
  else if ((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_SECTOR_13;
  }
  else if ((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_SECTOR_14;
  }
  else if ((Address < ADDR_FLASH_END) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_SECTOR_15;
  }
  else
  {
    sector = FLASH_SECTOR_15;
  }
  return sector;
}

typedef union
{
  uint64_t ul;
  uint32_t ui;
  int16_t us[2];
  float f;
} flash_value;

int16_t flash_read_int_value(uint8_t info_index)
{
  flash_value value = {0};
  uint8_t buf_index = info_index / FLASH_PACKING_SIZE;

  /* read flash value by 8 bytes */
  flash_read(FLASH_USER_START_ADDR + buf_index * sizeof(uint64_t), 0, &value.ul, 1);

  return value.us[info_index % FLASH_PACKING_SIZE];
}

int16_t flash_write_int_value(uint8_t info_index, int16_t info_value)
{
  uint64_t buffer[64] = { 0, }; /// memory warning

  /* read flash value by 8 bytes */
  flash_read(FLASH_USER_START_ADDR, 0, buffer, 64);

  /* write value into flash buffer */
  flash_memory_write_value(buffer, info_index, info_value);

  /* erase */
  flash_init();
  uint8_t startsector = flash_get_sector(FLASH_USER_START_ADDR);
  flash_erase(startsector, 1);
  flash_deinit();

  /* write memory buffer into flash */
  return flash_write(FLASH_USER_START_ADDR, 0, buffer, 64);
}

static HAL_StatusTypeDef flash_memory_write_value(uint64_t* buffer, uint8_t info_index, int16_t info_value)
{
  flash_value value = {0};
  uint8_t buf_index = info_index / FLASH_PACKING_SIZE;

  /* read from buffer and change value */
  value.ul = buffer[buf_index];

  value.us[info_index % FLASH_PACKING_SIZE] = info_value;
  /* write changed value into memory */
  buffer[buf_index] = value.ul;

  return HAL_OK;
}

static HAL_StatusTypeDef flash_write(uint32_t base_address, uint8_t offset, uint64_t *buf, uint8_t buf_size)
{
  uint8_t i = 0;

  uint32_t flash_address = base_address + (sizeof(uint64_t) * offset);

  HAL_StatusTypeDef flash_status;

  /* Unlock the FLASH control register access */
  HAL_FLASH_Unlock();

  /* Clear the FLASH's all pending flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  for (i = 0; i < buf_size; i++)
  {
    flash_status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                                      flash_address + (sizeof(uint64_t) * i), buf[i]);
    if(flash_status != HAL_OK)
    {
      printf("\r\n%s : flash error code = %lu", __FUNCTION__ , HAL_FLASH_GetError());
      return HAL_ERROR;
    }
  }

  /* Lock the FLASH control register access */
  HAL_FLASH_Lock();

  return HAL_OK;
}

static HAL_StatusTypeDef flash_read(uint32_t base_address, uint8_t offset, uint64_t *buf, uint8_t buf_size)
{
  uint32_t flash_address = base_address + (sizeof(uint64_t) * offset);
  uint8_t i = 0;

  for (i = 0 ; i < buf_size ; i++)
    buf[i] = (*(__IO uint32_t*)(flash_address + (sizeof(uint64_t) * i)));

  return HAL_OK;
}

