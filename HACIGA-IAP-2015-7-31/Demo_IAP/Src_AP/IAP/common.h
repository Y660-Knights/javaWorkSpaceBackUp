/*********************************************************************************************************//**
 * @file   wifi_lpt100.h
 * @version V1.00
 * @date    09/12/2014
 * @brief   The header file of wifi_lpt100
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT_COMMON_H
#define __HT_COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
   
/* Setting -----------------------------------------------------------------------------------------------*/
#define STR_MAX_LEN         (0xFFFF)
#define USE_SYSTICK_DELAY   (1)

/* Exported constants --------------------------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------------------------------------*/
typedef char* STR_T;

/* Exported variables --------------------------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------------------------------------*/
#if (USE_SYSTICK_DELAY != 0)
#define DelayXuS  SYSTICK_DelayXuS
#else
#define DelayXuS  RoughDelayXuS
#endif

/* Exported functions --------------------------------------------------------------------------------------*/
u32 Ascii2Hex(u8 *pStr, u8 size);
u32 HexAscii2Hex(char *pStr, u8 size);
u32 Hex2Bcd(u32 hex);
bool MemCpy(void* pDes, void* pSrc, u16 size);
bool MemCmp(void* pSrc, void* pCmp, int size);
int StrLen(STR_T pStr);
bool StrCmp(STR_T Src, STR_T Cmp);
bool MemCmpStr(char* Src, char* Cmp);
bool MemCpyStr(char* pDes, char* pSrc);
bool StrCpy(char* pDes, char* pSrc);
u8 StrSplitBySign(u8 max, STR_T pStr, char sign, u8 *pPos);
void RoughDelayXuS(u32 X);

#ifdef __cplusplus
}
#endif

#endif /* __HT_COMMON_H -----------------------------------------------------------------------------------*/






