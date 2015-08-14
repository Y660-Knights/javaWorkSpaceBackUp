/*********************************************************************************************************//**
 * @file    USBD/GK_LQFP100_STD/Src_IAP/iap_handler.h
 * @version V1.00
 * @date    11/22/2012
 * @brief   The header file of IAP Handler.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __IAP_HANDLER_H
#define __IAP_HANDLER_H

#include "ht32.h"
#include "ht32_board.h"
/* Includes ------------------------------------------------------------------------------------------------*/
/* Settings ------------------------------------------------------------------------------------------------*/
//<o0.0> Enable IAP Debug mode
// <i> Enable IAP Debug mode which output debug message to retarget COM port or SWV (ITM).
#define IAP_DEBUG_MODE                (0)

/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/
#define IAP_TIMEOUT_OFF               (-1)

#define IAP_APFALSH_START             (IAP_CODE_SIZE + (IAP_VERSION_PAGE * 1024))
#define IAP_APFLASH_END               (IAP_APFLASH_SIZE - 1)

#define IAP_APSRAM_START              (0x20000000)
#define IAP_APSRAM_END                (IAP_APSRAM_START + IAP_APSRAM_SIZE)

#define BOOT_MODE_APP                 (0x55AAFAF0)                  /* Magic number for SBVT1               */
#define BOOT_MODE_IAP                 (0x55AAFAF5)

/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
void IAP_Init(void);
void IAP_Handler(void);
u32 IAP_Reset(u32 uMode);
u32 IAP_isAPValid(void);
u32 IAP_isVersionValid(void);
void IAP_GoCMD(u32 address);

#endif /* __IAP_HANDLER_H ----------------------------------------------------------------------------------*/
