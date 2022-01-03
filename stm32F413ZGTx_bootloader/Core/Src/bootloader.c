/**
  ******************************************************************************
  * @file    bootloader.c
  * @author  dongsik sim

*/

#include "stm32f4xx_hal.h"
#include "bootloader.h"

#include <stdio.h>
#include <string.h>

#include "flash.h"
#include "usb_device.h"

char g_private_key[32] = "1234567890abcdefghij1234567890!";

/* display something depending on error type, soft reset */
void bootloader_error_handler(dfu_status_t dfu_status)
{
    printf("bootloader error handler \r\n");
    switch (dfu_status)
    {
        case DFU_IDLE:
        {
            printf("DFU_IDLE \r\n");
            return;
            // break;
        }
        case DFU_ERR_VERIFY:
        {
            printf("DFU_ERR_VERIFY \r\n");
            break;
        }
        case DFU_ERR_KEY:
        {
            printf("DFU_ERR_KEY \r\n");
            break;
        }
        case DFU_ERR_VERSION:
        {
            printf("DFU_ERR_VERSION \r\n");
            break;
        }
        default:
        {
            printf("dfu error \r\n");
            break;
        }
    }
    MX_USB_DEVICE_stop();
    HAL_NVIC_SystemReset();
}

/* check key, version  */
dfu_status_t bootloader_check_secure(uint8_t* src)
{
    printf("check secure \r\n");
    dfu_status_t dfu_status = DFU_IDLE;

    /*  check key   */
    dfu_secure_t* p_secure_info = (dfu_secure_t*)(src);
    printf("new key : %s , version : %d \r\n", p_secure_info->dfu_key, p_secure_info->dfu_version);

    if (strcmp((char*)p_secure_info->dfu_key, g_private_key))
    {
        dfu_status = DFU_ERR_KEY;
        printf("private key error \r\n");
        return dfu_status;
    }
    else
    {
        printf("private key pass \r\n");
    }

    /* check version */
    if (*(__IO uint32_t *)(ADDR_PREV_FW_INFO) == 0xFFFFFFFF)
    {
        printf("first dfu, don't need to compare \r\n");
        return dfu_status;
    }

    dfu_secure_t* p_prev_secure_info = (dfu_secure_t*)(ADDR_PREV_FW_INFO);
    printf("previous key : %s , version : %d \r\n", p_prev_secure_info->dfu_key, p_prev_secure_info->dfu_version);

    if (p_secure_info->dfu_version >= p_prev_secure_info->dfu_version)
    {
        printf("version check pass \r\n");
    }
    else
    {
        dfu_status = DFU_ERR_VERSION;
        printf("version check error \r\n");
        return dfu_status;
    }

    return dfu_status;
}


/*  flash copy  */
void bootloader_copy_image(uint32_t src, uint32_t dest, uint8_t nums_sec)
{
    HAL_StatusTypeDef status;
    status = flash_init();
    if (status != HAL_OK)
    {
        /*  todo    */
        printf("flash err \r\n");
        return ;
    }

    uint8_t startsector = flash_get_sector(dest);
    flash_erase(startsector, nums_sec);
    HAL_Delay(50);

    flash_copy(src, dest, nums_sec);
    HAL_Delay(100);

    status = flash_deinit();
    if (status != HAL_OK)
    {
        /*  todo    */
        printf("flash err \r\n");
        return ;
    }
}

/*  initialize all flash except for bootloader  */
void bootloader_initialize_flash(void)
{
    printf("initialize flash \r\n");
    HAL_StatusTypeDef status;
    status = flash_init();
    if (status != HAL_OK)
    {
        /*  todo    */
        printf("flash err \r\n");
        return ;
    }

    uint8_t startsector = flash_get_sector(USBD_DFU_APP_DEFAULT_ADD);
    uint8_t nums_sec = flash_get_sector(ADDR_FLASH_END) - startsector + 1;
    flash_erase(startsector, nums_sec);
    HAL_Delay(50);

    status = flash_deinit();
    if (status != HAL_OK)
    {
        /*  todo    */
        printf("flash err \r\n");
        return ;
    }
}

/*  roll back to previos version */
void bootloader_rollback_version(void)
{
    printf("rollback version \r\n");
    bootloader_copy_image(ADDR_FLASH_COPY, USBD_DFU_APP_DEFAULT_ADD, APP_NUMS_SECTOR);
    printf("roll back success \r\n");
    HAL_Delay(50);
    HAL_NVIC_SystemReset();
}


/* check if occur error during DFU */
void bootloader_check_DFU(void)
{
    dfu_integity_t* dfu_integrity = (dfu_integity_t *)(ADDR_DFU_INTEGRITY);
    if (dfu_integrity->dfu_complete != DFU_COMPLETE)
    {
      printf("DFU fail \r\n");
      bootloader_rollback_version();
    }
}