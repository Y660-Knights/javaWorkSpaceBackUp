/*********************************************************************************************************//**
 * @file    USBD/GK_LQFP100_STD/Src_IAP/iap_config.h
 * @version V1.00
 * @date    11/22/2012
 * @brief   The configuration file.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __IAP_CONFIG_H
#define __IAP_CONFIG_H

/* Includes ------------------------------------------------------------------------------------------------*/
/* Settings ------------------------------------------------------------------------------------------------*/

/*  User Command count                                                                                      */
#define USER_CMD_COUNT                (2)

/*  Buffer                                                                                                  */
#define TX_BUFFER_SIZE                (68)                          /* Tx Buffer size                       */
#define RX_BUFFER_SIZE                (1024)                        /* Rx Buffer size                       */

/* IAP                                                                                                      */
#define IAP_VERSION_PAGE              (1)
#define IAP_CODE_PAGE                 (10)
#define IAP_CODE_SIZE                 (1024 * IAP_CODE_PAGE)        /* IAP code size in Bytes               */

#define IAP_APFLASH_SIZE              (1024 * 127)                  /* 127 KB                               */
#define IAP_APSRAM_SIZE               (1024 * 32)                   /* 64  KB                               */

#define LOADER_VERSION                (0x100)

//<o0.0> Enable wait IAP command timeout function
// <i> If IAP Loader did not received any valid IAP command after specific timeout time, it starts AP automatically.
//<o1> Wait IAP command timeout value (ms) <1-5000:1>
#define IAP_TIMEOUT_EN                (0)
#define IAP_TIMEOUT                   (2000)                        /* IAP Command timeout based on ms      */

#define EXT_FLASH_PAGE_SIZE           (1024 * 4)
#define EXT_FLASH_SECTOR_SIZE         (1024 * 64)
#define EXT_FALSH_ADDRESS             (0x01000000)

#define NVM_BLOCK_SIZE       (64*1024)
#define NVM_BASE             (10*1024*1024)
#define NVM_AP_SIZE          (128*1024)
#define NVM_AP_BASE          (NVM_BASE - NVM_AP_SIZE)
#define NVM_AP_END           (NVM_AP_BASE + NVM_AP_SIZE)
#define NVM_AP_BLOCK_MAX     (NVM_AP_SIZE / NVM_BLOCK_SIZE)

#define JCQ_POWER_KEEP_PORT_ID    (GPIOE_ID)
#define JCQ_POWER_KEEP_PIN_ID     (3)
#define JCQ_POWER_KEEP_PORT        GPIO_PORT(JCQ_POWER_KEEP_PORT_ID)
#define JCQ_POWER_KEEP_PIN         GPIO_PIN(JCQ_POWER_KEEP_PIN_ID)
#define JCQ_SET_POWER_KEEP()       (JCQ_POWER_KEEP_PORT->SRR = JCQ_POWER_KEEP_PIN)
#define JCQ_RESET_POWER_KEEP()     (JCQ_POWER_KEEP_PORT->RR = JCQ_POWER_KEEP_PIN)

/* Exported typedef ----------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/

#endif /* __IAP_CONFIG_H -----------------------------------------------------------------------------------*/
