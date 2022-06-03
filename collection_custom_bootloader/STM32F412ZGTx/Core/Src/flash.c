/**
  ******************************************************************************
  * @file    flash.c
  * @author  dongsik sim

*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "flash.h"

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
  else if ((Address < ADDR_FLASH_END) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_SECTOR_11;
  }
  else
  {
    sector = FLASH_SECTOR_11;
  }
  return sector;
}