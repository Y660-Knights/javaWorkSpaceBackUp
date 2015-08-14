/*********************************************************************************************************//**
 * @file    USBD/GK_LQFP100_STD/Src_AP/ap_handler.h
 * @version V1.00
 * @date    11/09/2012
 * @brief   The header file of AP Handler.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __AP_HANDLER_H
#define __AP_HANDLER_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
/* Settings ------------------------------------------------------------------------------------------------*/
//<o0.0> Enable AP Debug mode
// <i> Enable AP Debug mode which output debug message to retarget COM port or SWV (ITM).
#define AP_DEBUG_MODE                 (0)

/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/
#define BOOT_MODE_APP                 (0x55AAFAF0)                  /* Magic number for SBVT1               */
#define BOOT_MODE_IAP                 (0x55AAFAF5)

/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
void AP_Init(void);
void AP_Handler(void);
u32 AP_Reset(u32 uMode);

#endif /* __AP_HANDLER_H -----------------------------------------------------------------------------------*/
