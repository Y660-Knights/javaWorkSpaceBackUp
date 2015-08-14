/*********************************************************************************************************//**
 * @file    ht32f1653_54_rstcu.c
 * @version $Rev:: 307          $
 * @date    $Date:: 2014-12-31 #$
 * @brief   This file provides all the Reset Control Unit firmware functions.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1653_54_rstcu.h"

/** @addtogroup HT32F1653_54_Peripheral_Driver HT32F1653/1654 Peripheral Driver
  * @{
  */

/** @defgroup RSTCU RSTCU
  * @brief RSTCU driver modules
  * @{
  */


/* Global functions ----------------------------------------------------------------------------------------*/
/** @defgroup RSTCU_Exported_Functions RSTCU exported functions
  * @{
  */
/*********************************************************************************************************//**
 * @brief Check whether the specific global reset flag is set or not.
 * @param RSTCU_RSTF: specify the reset flag.
 *        This parameter can be:
 *        @arg RSTCU_FLAG_SYSRST: Get system reset flag
 *        @arg RSTCU_FLAG_EXTRST: Get external pin reset flag
 *        @arg RSTCU_FLAG_WDTRST: Get WDT reset flag
 *        @arg RSTCU_FLAG_PORST : Get power on reset flag
 * @retval SET or RESET
 ************************************************************************************************************/
FlagStatus RSTCU_GetResetFlagStatus(RSTCU_RSTF_TypeDef RSTCU_RSTF)
{
  u32 tmp;

  /* Check the parameters                                                                                   */
  Assert_Param(IS_RSTCU_FLAG(RSTCU_RSTF));

  tmp = (HT_RSTCU->GRSR & ((u32)0x1 << RSTCU_RSTF));
  if (tmp != RESET)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*********************************************************************************************************//**
 * @brief Clear the specific global reset flag.
 * @param RSTCU_RSTF: specify the reset flag.
 *        This parameter can be:
 *        @arg RSTCU_FLAG_SYSRST: Clear system reset flag
 *        @arg RSTCU_FLAG_EXTRST: Clear external pin reset flag
 *        @arg RSTCU_FLAG_WDTRST: Clear WDT reset flag
 *        @arg RSTCU_FLAG_PORST : Clear power on reset flag
 * @retval None
 ************************************************************************************************************/
void RSTCU_ClearResetFlag(RSTCU_RSTF_TypeDef RSTCU_RSTF)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_RSTCU_FLAG(RSTCU_RSTF));

  HT_RSTCU->GRSR = (u32)0x1 << RSTCU_RSTF;  /* Write 1 to clear                                             */
}

/*********************************************************************************************************//**
 * @brief Clear all of the global reset flag.
 * @retval None
 ************************************************************************************************************/
void RSTCU_ClearAllResetFlag(void)
{
  HT_RSTCU->GRSR = (vu32)0xF;   /* Write 1 to clear                                                         */
}

/*********************************************************************************************************//**
 * @brief Peripheral reset function.
 * @param Reset: specify the peripheral clock enable bits.
 * @param Cmd: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void RSTCU_PeripReset(RSTCU_PeripReset_TypeDef Reset, ControlStatus Cmd)
{
  u32 uAHBPRST;
  u32 uAPBPRST0;
  u32 uAPBPRST1;

  uAHBPRST  = HT_RSTCU->AHBPRST;
  uAPBPRST0 = HT_RSTCU->APBPRST0;
  uAPBPRST1 = HT_RSTCU->APBPRST1;

  uAHBPRST  &= ~(Reset.Reg[0]);
  uAPBPRST0 &= ~(Reset.Reg[1]);
  uAPBPRST1 &= ~(Reset.Reg[2]);

  if (Cmd != DISABLE)
  {
    uAHBPRST  |= Reset.Reg[0];
    uAPBPRST0 |= Reset.Reg[1];
    uAPBPRST1 |= Reset.Reg[2];
  }

  HT_RSTCU->AHBPRST  = uAHBPRST;
  HT_RSTCU->APBPRST0 = uAPBPRST0;
  HT_RSTCU->APBPRST1 = uAPBPRST1;
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