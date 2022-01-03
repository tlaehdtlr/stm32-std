/**
  ******************************************************************************
  * @file    bootloader.h
  * @author  dongsik sim

*/

#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#include "usbd_conf.h"

/*  user check  */
#define APP_NUMS_SECTOR    5
#define ADDR_FLASH_COPY   ADDR_FLASH_SECTOR_11

#define ADDR_FW_INFO          (USBD_DFU_APP_DEFAULT_ADD + FLASH_ADDR_FW_INFO)
#define ADDR_PREV_FW_INFO     (ADDR_FLASH_COPY + FLASH_ADDR_FW_INFO)
#define PRIVATE_KEY_LENGTH    32

/*  user check  */
#define ADDR_DFU_INTEGRITY    ((uint32_t*)0x80DFC00)
#define DFU_COMPLETE            0x99

typedef struct {
  uint8_t dfu_complete;
} dfu_integity_t;

typedef struct
{
  uint8_t dfu_key[32];
  uint16_t dfu_version;
} dfu_secure_t;

/*  i don't know yet to decode pointer char  */
/*
typedef struct
{
  char* manufacturer;
  char* device;
  char* sw_version;
  char* hw_version;
} meta_info_t;
 */

typedef enum
{
    DFU_IDLE,
    DFU_ERR_VERIFY,
    DFU_ERR_KEY,
    DFU_ERR_VERSION,
    DFU_UNKNOWN,
} dfu_status_t;


/* display something depending on error type, soft reset */
void bootloader_error_handler(dfu_status_t dfu_status);

/* if it is secure info, check key, version  */
dfu_status_t bootloader_check_secure(uint8_t* src);

/* flash copy to other sector to back up  */
void bootloader_copy_image(uint32_t src, uint32_t dest, uint8_t nums_sec);

void bootloader_initialize_flash(void);
void bootloader_rollback_version(void);

void bootloader_check_DFU(void);
#endif