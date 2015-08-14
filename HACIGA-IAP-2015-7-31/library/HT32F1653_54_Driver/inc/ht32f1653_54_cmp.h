/*********************************************************************************************************//**
 * @file    ht32f1653_54_cmp.h
 * @version $Rev:: 316          $
 * @date    $Date:: 2015-01-08 #$
 * @brief   The header file of the CMP library.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F1653_54_CMP_H
#define __HT32F1653_54_CMP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1653_54.h"

/** @addtogroup HT32F1653_54_Peripheral_Driver HT32F1653/1654 Peripheral Driver
  * @{
  */

/** @addtogroup CMP
  * @{
  */


/* Exported types ------------------------------------------------------------------------------------------*/

/** @defgroup CMP_Exported_Types CMP exported types
  * @{
  */

typedef struct
{
  u32 CMP_Wakeup;
  u32 CMP_OutputSelection;
  u32 CMP_ScalerSource;
  u32 CMP_ScalerOutputBuf;
  u32 CMP_ScalerEnable;
  u32 CMP_CoutSync;
  u32 CMP_OutputPol;
  u32 CMP_InvInputSelection;
  u32 CMP_Hysteresis;
  u32 CMP_Speed;
} CMP_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup CMP_Exported_Constants CMP exported constants
  * @{
  */

/* Definitions of CMP Protection Key                                                                        */
#define CMP_PROTECT_KEY       ((u32)0x9C3A0000)


/* Definitions of CMP Output Status                                                                         */
#define CMP_OUTPUT_HIGH       ((u32)0x00008000)
#define CMP_OUTPUT_LOW        ((u32)0x00000000)


/* Definitions of CMP Wakeup Control Bit                                                                    */
#define CMP_WUP_ENABLE        ((u32)0x00004000)
#define CMP_WUP_DISABLE       ((u32)0x00000000)

#define IS_CMP_Wakeup_Set(x)  ((x == CMP_WUP_ENABLE) || (x == CMP_WUP_DISABLE))


/* Definitions of CMP Output Selection for IP Trigger Source                                                */
#define CMP_TRIG_NONE         ((u32)0x00000000)
#define CMP_TRIG_GPTM_CH3     ((u32)0x00000800)
#define CMP_TRIG_MCTM_CH3     ((u32)0x00001000)
#define CMP_TRIG_MCTM_BK1     ((u32)0x00001800)
#define CMP_TRIG_ADC          ((u32)0x00002000)

#define IS_CMP_OutputSelection(x)  ((x == CMP_TRIG_NONE) || (x == CMP_TRIG_GPTM_CH3) || (x == CMP_TRIG_MCTM_CH3) || \
                                    (x == CMP_TRIG_MCTM_BK1) || (x == CMP_TRIG_ADC))

/* Definitions of CMP Scaler Source Selection                                                               */
#define CMP_SCALER_SRC_VDDA   ((u32)0x00000000)
#define CMP_SCALER_SRC_VREF   ((u32)0x00000400)

#define IS_CMP_ScalerSource(x)   ((x == CMP_SCALER_SRC_VDDA) || (x == CMP_SCALER_SRC_VREF))


/* Definitions of CMP Scaler Output Enable Bit                                                              */
#define CMP_SCALER_OBUF_DISABLE ((u32)0x00000000)
#define CMP_SCALER_OBUF_ENABLE  ((u32)0x00000200)

#define IS_CMP_ScalerOutputBuf(x)   ((x == CMP_SCALER_OBUF_DISABLE) || (x == CMP_SCALER_OBUF_ENABLE))


/* Definitions of CMP Scaler Enable Bit                                                                     */
#define CMP_SCALER_DISABLE    ((u32)0x00000000)
#define CMP_SCALER_ENABLE     ((u32)0x00000100)

#define IS_CMP_ScalerEnable(x)   ((x == CMP_SCALER_DISABLE) || (x == CMP_SCALER_ENABLE))


/* Definitions of CMP Sync Output Enable bit                                                                */
#define CMP_ASYBC_OUTPUT      ((u32)0x00000000)
#define CMP_SYNC_OUTPUT       ((u32)0x00000080)

#define IS_CMP_CoutSynchronized(x)   ((x == CMP_ASYBC_OUTPUT) || (x == CMP_SYNC_OUTPUT))


/* Definitions of CMP Output Polarity Selection                                                             */
#define CMP_NONINV_OUTPUT     ((u32)0x00000000)
#define CMP_INV_OUTPUT        ((u32)0x00000040)

#define IS_CMP_OutputPol_Set(x)   ((x == CMP_NONINV_OUTPUT) || (x == CMP_INV_OUTPUT))


/* Definitions of CMP Inverted Input Source Selection                                                       */
#define CMP_EXTERNAL_CN_IN    ((u32)0x00000000)
#define CMP_SCALER_CN_IN      ((u32)0x00000010)

#define IS_CMP_InvInputSelection(x)   ((x == CMP_EXTERNAL_CN_IN) || (x == CMP_SCALER_CN_IN))


/* Definitions of CMP Hysteresis Level Selection                                                            */
#define CMP_NO_HYSTERESIS     ((u32)0x00000000)
#define CMP_LOW_HYSTERESIS    ((u32)0x00000004)
#define CMP_MID_HYSTERESIS    ((u32)0x00000008)
#define CMP_HIGH_HYSTERESIS   ((u32)0x0000000C)

#define IS_CMP_Hysteresis_Set(x)   ((x == CMP_NO_HYSTERESIS) || (x == CMP_LOW_HYSTERESIS) || (x == CMP_MID_HYSTERESIS) || \
                                    (x == CMP_HIGH_HYSTERESIS))

/* Definitions of CMP Speed Mode Selection                                                                  */
#define CMP_HIGH_SPEED        ((u32)0x00000002)
#define CMP_LOW_SPEED         ((u32)0x00000000)

#define IS_CMP_Speed_Set(x)   ((x == CMP_HIGH_SPEED) || (x == CMP_LOW_SPEED))


/* Definitions of CMP Enable bit                                                                            */
#define CMP_ENABLE            ((u32)0x00000001)


/* Definitions of CMP Output Edge Interrupt Enable bit                                                      */
#define CMP_INT_RE            ((u32)0x00000002)
#define CMP_INT_FE            ((u32)0x00000001)

/* Check the CMP Interrupt Parameter                                                                        */
#define IS_CMP_INT(x) ((x & 0xFFFFFF00) != 0x0)


/* Definitions of CMP Output Edge Detection Enable bit                                                      */
#define CMP_RE_Detect         ((u32)0x00000200)
#define CMP_FE_Detect         ((u32)0x00000100)

#define IS_CMP_EdgeDetect(x)   ((x == CMP_RE_Detect) || (x == CMP_FE_Detect))


/* Definitions of CMP Output Edge Flag                                                                      */
#define CMP_FLAG_RE           ((u32)0x00000002)
#define CMP_FLAG_FE           ((u32)0x00000001)

/* Check the CMP flag Parameter                                                                             */
#define IS_CMP_FLAG(x) ((x & 0xFFFFFF00) != 0x0)


/* Check the CMPx Parameter                                                                                 */
#define IS_CMP(x) ((x == HT_CMP0) || (x == HT_CMP1))


/* Check the Scaler Value                                                                                   */
#define IS_SCALER_VALUE(x) (x <= 0x3F)

/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup CMP_Exported_Functions CMP exported function prototypes
  * @{
  */
void CMP_DeInit(void);
void CMP_UnprotectConfig(HT_CMP_TypeDef* CMPx);
void CMP_Init(HT_CMP_TypeDef* CMPx, CMP_InitTypeDef* CMP_InitStruct);
void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct);
void CMP_Cmd(HT_CMP_TypeDef* CMPx, ControlStatus NewState);
void CMP_IntConfig(HT_CMP_TypeDef* CMPx, u32 CMP_INT, ControlStatus NewState);
void CMP_EdgeDetectConfig(HT_CMP_TypeDef* CMPx, u32 CMP_EdgeDetect, ControlStatus NewState);
FlagStatus CMP_GetFlagStatus(HT_CMP_TypeDef* CMPx, u32 CMP_FLAG);
void CMP_ClearFlag(HT_CMP_TypeDef* CMPx, u32 CMP_FLAG);
FlagStatus CMP_GetOutputStatus(HT_CMP_TypeDef* CMPx);
void CMP_SetScalerValue(HT_CMP_TypeDef* CMPx, u8 Scaler_Value);
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
