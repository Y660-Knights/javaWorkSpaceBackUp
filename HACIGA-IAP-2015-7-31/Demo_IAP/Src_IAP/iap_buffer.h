/*********************************************************************************************************//**
 * @file    USBD/GK_LQFP100_STD/Src_IAP/iap_buffer.h
 * @version V1.00
 * @date    07/08/2012
 * @brief   The header file of buffer.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __IAP_BUFFER_H
#define __IAP_BUFFER_H

/* Includes ------------------------------------------------------------------------------------------------*/

#include "ht32.h"
#include "ht32_board.h"
/* Settings ------------------------------------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------------------------------------*/
/**
 * @brief  Operation information
 */
typedef struct
{
  u32 uRead;
  vu32 uWrite;
  u8 *puMemory;
  u32 uSize;
} Buffer_TypeDef;

/* Exported constants --------------------------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
void Buffer_Init(Buffer_TypeDef *pBuffer, u8 *puMemory, u32 uSize);
u32 Buffer_ReadByte(Buffer_TypeDef *pBuffer, u8 *puData);
u32 Buffer_WriteByte(Buffer_TypeDef *pBuffer, u32 uData);
u32 Buffer_Read(Buffer_TypeDef *pBuffer, u8 *puData, u32 uLength);
u32 Buffer_Write(Buffer_TypeDef *pBuffer, u8 *puData, u32 uLength);
u32 Buffer_isEmpty(Buffer_TypeDef *pBuffer);
u32 Buffer_isFull(Buffer_TypeDef *pBuffer);
void Buffer_Discard(Buffer_TypeDef *pBuffer);

#endif /* __IAP_BUFFER_H -----------------------------------------------------------------------------------*/
