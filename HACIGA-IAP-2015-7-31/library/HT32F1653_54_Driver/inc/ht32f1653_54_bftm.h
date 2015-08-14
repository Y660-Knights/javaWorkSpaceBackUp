/*********************************************************************************************************//**
 * @file    ht32f1653_54_bftm.h
 * @version $Rev:: 307          $
 * @date    $Date:: 2014-12-31 #$
 * @brief   The header file of the BFTM library.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F1653_54_BFTM_H
#define __HT32F1653_54_BFTM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1653_54.h"

/** @addtogroup HT32F1653_54_Peripheral_Driver HT32F1653/1654 Peripheral Driver
  * @{
  */

/** @addtogroup BFTM
  * @{
  */


/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup BFTM_Exported_Constants BFTM exported constants
  * @{
  */
#define IS_BFTM(x) ((x == HT_BFTM0) || (x == HT_BFTM1))

#define BFTM_FLAG_MATCH           (1UL << 0)
#define BFTM_INT_MATCH            (1UL << 0)
/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup BFTM_Exported_Functions BFTM exported functions
  * @{
  */
void BFTM_DeInit(HT_BFTM_TypeDef* BFTMx);
void BFTM_EnaCmd(HT_BFTM_TypeDef* BFTMx, ControlStatus NewState);
void BFTM_SetCompare(HT_BFTM_TypeDef* BFTMx, u32 wCompare);
u32 BFTM_GetCompare(HT_BFTM_TypeDef* BFTMx);
void BFTM_SetCounter(HT_BFTM_TypeDef* BFTMx, u32 wCounter);
u32 BFTM_GetCounter(HT_BFTM_TypeDef* BFTMx);
void BFTM_OneShotModeCmd(HT_BFTM_TypeDef* BFTMx, ControlStatus NewState);
void BFTM_IntConfig(HT_BFTM_TypeDef* BFTMx, ControlStatus NewState);
FlagStatus BFTM_GetFlagStatus(HT_BFTM_TypeDef* BFTMx);
void BFTM_ClearFlag(HT_BFTM_TypeDef* BFTMx);
/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif