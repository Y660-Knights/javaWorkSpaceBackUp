/*********************************************************************************************************//**
 * @file    ht32f1653_54_cmp.c
 * @version $Rev:: 316          $
 * @date    $Date:: 2015-01-08 #$
 * @brief   This file provides all the CMP firmware functions.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1653_54_cmp.h"

/** @addtogroup HT32F1653_54_Peripheral_Driver HT32F1653/1654 Peripheral Driver
  * @{
  */

/** @defgroup  CMP CMP
  * @brief CMP driver modules
  * @{
  */

/* Global functions ----------------------------------------------------------------------------------------*/
/** @defgroup CMP_Exported_Functions CMP exported function prototypes
  * @{
  */


/*********************************************************************************************************//**
 * @brief  Deinitialize the CMP0 and CMP1 peripheral registers to their default reset values.
 * @retval None
 ************************************************************************************************************/
void CMP_DeInit(void)
{
  RSTCU_PeripReset_TypeDef RSTCUReset = {{0}};

  RSTCUReset.Bit.CMP = 1;
  RSTCU_PeripReset(RSTCUReset, ENABLE);
}

/*********************************************************************************************************//**
 * @brief  Unprotect the selected comparator configuration before setting the Comparator Control Register.
 * @param  CMPx: where the CMPx is the selected CMP peripheral, x can be 0 or 1.
 * @retval None
 ************************************************************************************************************/
void CMP_UnprotectConfig(HT_CMP_TypeDef* CMPx)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CMP(CMPx));

  /* Set the unlock code corresponding to selected comparator                                               */
  CMPx->CR = CMP_PROTECT_KEY;

}

/*********************************************************************************************************//**
  * @brief  Initialize the CMP peripheral according to the specified parameters in the CMP_InitStruct.
  * @param  CMPx: where the CMPx is the selected CMP peripheral, x can be 0 or 1.
  * @param  CMP_InitStruct: pointer to a CMP_InitTypeDef structure that contains the configuration
  *         information for the CMP peripheral.
  * @retval None
  ***********************************************************************************************************/
void CMP_Init(HT_CMP_TypeDef* CMPx, CMP_InitTypeDef* CMP_InitStruct)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CMP(CMPx));
  Assert_Param(IS_CMP_Wakeup_Set(CMP_InitStruct->CMP_Wakeup));
  Assert_Param(IS_CMP_OutputSelection(CMP_InitStruct->CMP_OutputSelection));
  Assert_Param(IS_CMP_ScalerSource(CMP_InitStruct->CMP_ScalerSource));
  Assert_Param(IS_CMP_ScalerOutputBuf(CMP_InitStruct->CMP_ScalerOutputBuf));
  Assert_Param(IS_CMP_ScalerEnable(CMP_InitStruct->CMP_ScalerEnable));
  Assert_Param(IS_CMP_CoutSynchronized(CMP_InitStruct->CMP_CoutSync));
  Assert_Param(IS_CMP_OutputPol_Set(CMP_InitStruct->CMP_OutputPol));
  Assert_Param(IS_CMP_InvInputSelection(CMP_InitStruct->CMP_InvInputSelection));
  Assert_Param(IS_CMP_Hysteresis_Set(CMP_InitStruct->CMP_Hysteresis));
  Assert_Param(IS_CMP_Speed_Set(CMP_InitStruct->CMP_Speed));

  CMPx->CR = CMP_InitStruct->CMP_Wakeup | CMP_InitStruct->CMP_OutputSelection | CMP_InitStruct->CMP_ScalerSource | \
             CMP_InitStruct->CMP_ScalerOutputBuf | CMP_InitStruct->CMP_ScalerEnable | CMP_InitStruct->CMP_CoutSync | \
             CMP_InitStruct->CMP_OutputPol | CMP_InitStruct->CMP_InvInputSelection | CMP_InitStruct->CMP_Hysteresis | \
             CMP_InitStruct->CMP_Speed;
}

/*********************************************************************************************************//**
  * @brief  Fill each CMP_InitStruct member with its default value.
  * @param  CMP_InitStruct: pointer to an CMP_InitTypeDef structure which will be initialized.
  * @retval None
  ***********************************************************************************************************/
void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct)
{
  /* CMP_InitStruct members default value                                                                   */
  CMP_InitStruct->CMP_Wakeup = CMP_WUP_DISABLE;
  CMP_InitStruct->CMP_OutputSelection = CMP_TRIG_NONE;
  CMP_InitStruct->CMP_ScalerSource = CMP_SCALER_SRC_VDDA;
  CMP_InitStruct->CMP_ScalerOutputBuf = CMP_SCALER_OBUF_DISABLE;
  CMP_InitStruct->CMP_ScalerEnable = CMP_SCALER_DISABLE;
  CMP_InitStruct->CMP_CoutSync = CMP_ASYBC_OUTPUT;
  CMP_InitStruct->CMP_OutputPol = CMP_NONINV_OUTPUT;
  CMP_InitStruct->CMP_InvInputSelection = CMP_EXTERNAL_CN_IN;
  CMP_InitStruct->CMP_Hysteresis = CMP_NO_HYSTERESIS;
  CMP_InitStruct->CMP_Speed = CMP_LOW_SPEED;
}

/*********************************************************************************************************//**
 * @brief  Enable or Disable the specified CMP peripheral.
 * @param  CMPx: where the CMPx is the selected CMP peripheral, x can be 0 or 1.
 * @param  NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void CMP_Cmd(HT_CMP_TypeDef* CMPx, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CMP(CMPx));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected CMPx peripheral                                                                  */
    CMPx->CR |= CMP_ENABLE;
  }
  else
  {
    /* Disable the selected CMPx peripheral                                                                 */
    CMPx->CR &= ~(u32)CMP_ENABLE;
  }
}

/*********************************************************************************************************//**
 * @brief  Enable or Disable the specified CMP interrupts.
 * @param  CMPx: where the CMPx is the selected CMP peripheral, x can be 0 or 1.
 * @param  CMP_INT: Specify the CMP interrupt sources that is to be enabled or disabled.
 *         This parameter can be any combination of the following values:
 *         @arg CMP_INT_RE      : CMP rising edge interrupt
 *         @arg CMP_INT_FE      : CMP falling edge interrupt
 * @param  NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void CMP_IntConfig(HT_CMP_TypeDef* CMPx, u32 CMP_INT, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CMP(CMPx));
  Assert_Param(IS_CMP_INT(CMP_INT));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    CMPx->IER |= CMP_INT ;
  }
  else
  {
    CMPx->IER &= ~CMP_INT ;
  }
}

/*********************************************************************************************************//**
 * @brief  Enable or Disable the specified CMP edge detection.
 * @param  CMPx: where the CMPx is the selected CMP peripheral, x can be 0 or 1.
 * @param  CMP_EdgeDetect: Specify the CMP edge detection that is to be enabled or disabled.
 *         This parameter can be any combination of the following values:
 *         @arg CMP_RE_Detect      : CMP rising edge detection
 *         @arg CMP_FE_Detect      : CMP falling edge detection
 * @param  NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void CMP_EdgeDetectConfig(HT_CMP_TypeDef* CMPx, u32 CMP_EdgeDetect, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CMP(CMPx));
  Assert_Param(IS_CMP_EdgeDetect(CMP_EdgeDetect));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    CMPx->TFR = (CMPx->TFR | CMP_EdgeDetect) & 0xfffffffc;
  }
  else
  {
    CMPx->TFR = (CMPx->TFR & (~CMP_EdgeDetect)) & 0xfffffffc;
  }
}

/*********************************************************************************************************//**
 * @brief  Check whether the specified CMPx flag has been set.
 * @param  CMPx: where the CMPx is the selected CMP peripheral, x can be 0 or 1.
 * @param  CMP_FLAG: Specify the flag to be checked.
 *         @arg CMP_FLAG_RE      : CMP rising edge flag
 *         @arg CMP_FLAG_FE      : CMP falling edge flag
 * @return The new state of CMP_FLAG (SET or RESET).
 ************************************************************************************************************/
FlagStatus CMP_GetFlagStatus(HT_CMP_TypeDef* CMPx, u32 CMP_FLAG)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CMP(CMPx));
  Assert_Param(IS_CMP_FLAG(CMP_FLAG));

  if ((CMPx->TFR & CMP_FLAG) != 0)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*********************************************************************************************************//**
 * @brief  Clear flags of the CMPx.
 * @param  CMPx: where the CMPx is the selected CMP peripheral, x can be 0 or 1.
 * @param  CMP_FLAG: Specify the flag to be checked.
 *         This parameter can be any combination of the following values:
 *         @arg CMP_FLAG_RE      : CMP rising edge flag
 *         @arg CMP_FLAG_FE      : CMP falling edge flag
 * @retval None
 ************************************************************************************************************/
void CMP_ClearFlag(HT_CMP_TypeDef* CMPx, u32 CMP_FLAG)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CMP(CMPx));
  Assert_Param(IS_CMP_FLAG(CMP_FLAG));

  /* Clear the flags                                                                                        */
  CMPx->TFR = (CMPx->TFR & 0xfffffffc) | CMP_FLAG;

  /*--------------------------------------------------------------------------------------------------------*/
  /* DSB instruction is added in this function to ensure the write operation which is for clearing interrupt*/
  /* flag is actually completed before exiting ISR. It prevents the NVIC from detecting the interrupt again */
  /* since the write register operation may be pended in the internal write buffer of Cortex-M3 when program*/
  /* has exited interrupt routine. This DSB instruction may be masked if this function is called in the     */
  /* beginning of ISR and there are still some instructions before exiting ISR.                             */
  /*--------------------------------------------------------------------------------------------------------*/
  __DSB();
}

/*********************************************************************************************************//**
 * @brief  Get the output status of CMPx.
 * @param  CMPx: where the CMPx is the selected CMP peripheral, x can be 0 or 1.
 * @retval The new state of CMP_Output (SET or RESET).
 ************************************************************************************************************/
FlagStatus CMP_GetOutputStatus(HT_CMP_TypeDef* CMPx)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CMP(CMPx));

  if ((CMPx-> CR & CMP_OUTPUT_HIGH) != 0)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*********************************************************************************************************//**
  * @brief  Sets the specified reference value in the data register of the scaler.
  * @param  CMPx: where the CMPx is the selected CMP peripheral, x can be 0 or 1.
  * @param  Scaler_Value: value to be loaded in the selected data register
  * @retval None
  ***********************************************************************************************************/
void CMP_SetScalerValue(HT_CMP_TypeDef* CMPx, u8 Scaler_Value)
{

  /* Check the parameters                                                                                   */
  Assert_Param(IS_CMP(CMPx));
  Assert_Param(IS_SCALER_VALUE(Scaler_Value));

  /* Set the scaler reference value register                                                                */
  CMPx->VALR = (u32)Scaler_Value;
}

/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */
