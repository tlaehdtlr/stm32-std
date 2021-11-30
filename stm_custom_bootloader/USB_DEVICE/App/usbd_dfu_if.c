/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_dfu_if.c
  * @brief          : Usb device for Download Firmware Update.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu_if.h"

/* USER CODE BEGIN INCLUDE */
#include <stdio.h>
#include <stdbool.h>
#include "flash.h"
#include "bootloader.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_DFU
  * @brief Usb DFU device module.
  * @{
  */

/** @defgroup USBD_DFU_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_Defines
  * @brief Private defines.
  * @{
  */

#define FLASH_DESC_STR      "@Internal Flash   /0x08000000/04*016Ka,01*064Kg,07*128Kg"

/* USER CODE BEGIN PRIVATE_DEFINES */
#define ONE 1
#define DEBUG_LOG 0

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_DFU_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static uint16_t MEM_If_Init_FS(void);
static uint16_t MEM_If_Erase_FS(uint32_t Add);
static uint16_t MEM_If_Write_FS(uint8_t *src, uint8_t *dest, uint32_t Len);
static uint8_t *MEM_If_Read_FS(uint8_t *src, uint8_t *dest, uint32_t Len);
static uint16_t MEM_If_DeInit_FS(void);
static uint16_t MEM_If_GetStatus_FS(uint32_t Add, uint8_t Cmd, uint8_t *buffer);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

#if defined ( __ICCARM__ ) /* IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN USBD_DFU_MediaTypeDef USBD_DFU_fops_FS __ALIGN_END =
{
   (uint8_t*)FLASH_DESC_STR,
    MEM_If_Init_FS,
    MEM_If_DeInit_FS,
    MEM_If_Erase_FS,
    MEM_If_Write_FS,
    MEM_If_Read_FS,
    MEM_If_GetStatus_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Memory initialization routine.
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Init_FS(void)
{
  /* USER CODE BEGIN 0 */
  printf("dfu init \r\n");
  if (flash_init())
  {
    return (USBD_FAIL);
  }

  return (USBD_OK);

  /* USER CODE END 0 */
}

/**
  * @brief  De-Initializes Memory
  * @retval USBD_OK if operation is successful, MAL_FAIL else
  */

uint16_t MEM_If_DeInit_FS(void)
{
  /* USER CODE BEGIN 1 */
  printf("dfu deinit \r\n");
  if (flash_deinit())
  {
    return (USBD_FAIL);
  }
  // HAL_NVIC_SystemReset();
  return (USBD_OK);
  /* USER CODE END 1 */
}

/**
  * @brief  Erase sector.
  * @param  Add: Address of sector to be erased.
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Erase_FS(uint32_t Add)
{
  /* USER CODE BEGIN 2 */
  printf("usb dfu erase \r\n");
  uint32_t startsector = flash_get_sector(Add);
  if (startsector < FLASH_SECTOR_4)
  {
    printf("sector 0~3 are bootloader regions \r\n");
    /*
      todo : because programmer will re-try , add stoping if want
    */
    return (USBD_FAIL);
  }
  /* cube progammer  */
  flash_erase(startsector, 1);
  // flash_erase(startsector, APP_NUMS_SECTOR);

  return (USBD_OK);

  /* USER CODE END 2 */
}

/**
  * @brief  Memory write routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Write_FS(uint8_t *src, uint8_t *dest, uint32_t Len)
{
  /* USER CODE BEGIN 3 */
  printf("write, src : %08x, dest : %08x, len : %ld \r\n", src, dest, Len);
#if DEBUG_LOG
  uint8_t cnt = 0;
  for (int j=0; j<Len; j++)
  {
    // printf("idx %d - %d ", j, *(src+j));
    if (cnt == 4)
    {
      cnt = 0;
      printf(" ");
    }
    printf("%02x", *(src+j+(3-2*cnt)));
    cnt++;
  }
  printf("\r\n");
#endif
  dfu_status_t dfu_status;
  if ((uint32_t)dest == ADDR_FW_INFO)
  {
    printf("ADDR_FW_INFO \r\n");
    dfu_status = bootloader_check_secure(src);
    if (dfu_status)
    {
      printf("dfu secure error \r\n");
      bootloader_error_handler(dfu_status);
      // MX_USB_DEVICE_stop();
      return USBD_FAIL;
    }
  }


  uint32_t i = 0;

  for (i = 0; i < Len; i += 4)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
     * be done by byte */
    if (HAL_FLASH_Program
        (FLASH_TYPEPROGRAM_WORD, (uint32_t) (dest + i),
         *(uint32_t *) (src + i)) == HAL_OK)
    {
      /* Check the written value */
      if (*(uint32_t *) (src + i) != *(uint32_t *) (dest + i))
      {
        /* Flash content doesn't match SRAM content */
        printf("verified fail \r\n");
        // dfu_status = DFU_verified_err;
        dfu_status = DFU_ERR_VERIFY;
        bootloader_error_handler(dfu_status);
        return 2;
      }
    }
    else
    {
      printf("Fail write flash \r\n");
      /* Error occurred while writing data in Flash memory */
      dfu_status = DFU_UNKNOWN;
      bootloader_error_handler(dfu_status);
      return 1;
    }
  }
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  Memory read routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *MEM_If_Read_FS(uint8_t *src, uint8_t *dest, uint32_t Len)
{
  /* Return a valid address to avoid HardFault */
  /* USER CODE BEGIN 4 */
  printf("read,  len :%ld, address : %08x \r\n", Len, src);
#if DEBUG_LOG
  uint8_t cnt = 0;
  for (int j=0; j<Len; j++)
  {
    // printf("idx %d - %d ", j, *(src+j));
    if (cnt == 4)
    {
      cnt = 0;
      printf(" ");
    }
    printf("%02x", *(src+j+(3-2*cnt)));
    cnt++;
  }
  printf("\r\n");
#endif
  uint32_t i = 0;
  uint8_t *psrc = src;

  for (i = 0; i < Len; i++)
  {
    dest[i] = *psrc++;
  }

  // return (uint8_t*)(USBD_OK);
  return dest;
  /* USER CODE END 4 */
}

/**
  * @brief  Get status routine
  * @param  Add: Address to be read from
  * @param  Cmd: Number of data to be read (in bytes)
  * @param  buffer: used for returning the time necessary for a program or an erase operation
  * @retval USBD_OK if operation is successful
  */
uint16_t MEM_If_GetStatus_FS(uint32_t Add, uint8_t Cmd, uint8_t *buffer)
{
  /* USER CODE BEGIN 5 */
  // printf("get status add : %08x, cmd : %d \r\n", Add, Cmd);
  switch (Cmd)
  {
    case DFU_MEDIA_PROGRAM:
      buffer[1] = (uint8_t) FLASH_PROGRAM_TIME;
      buffer[2] = (uint8_t) (FLASH_PROGRAM_TIME << 8);
      buffer[3] = 0;
      break;

    case DFU_MEDIA_ERASE:
    default:
      buffer[1] = (uint8_t) FLASH_ERASE_TIME;
      buffer[2] = (uint8_t) (FLASH_ERASE_TIME << 8);
      buffer[3] = 0;
      break;
  }
  return (USBD_OK);
  /* USER CODE END 5 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
