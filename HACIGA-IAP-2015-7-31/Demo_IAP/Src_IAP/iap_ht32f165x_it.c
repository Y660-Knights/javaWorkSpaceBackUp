/*********************************************************************************************************//**
 * @file    USBD/Mass_Storage_IAP/Src_IAP/iap_ht32f165x_it.c
 * @version $Rev:: 307          $
 * @date    $Date:: 2014-12-31 #$
 * @brief   This file provides all interrupt service routine.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_usbd_core.h"
#include "ht32_board.h"
/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup USBD_Examples USBD
  * @{
  */

/** @addtogroup USBD_Mass_Storage_IAP USBD Mass Storage IAP
  * @{
  */

/** @addtogroup Mass_Storage_IAP IAP
  * @{
  */
/*********************************************************************************************************//**
 * @brief  This function handles SysTick Handler.
 * @retval None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
  #if (IAP_TIMEOUT_EN == 1)
  extern s32 gIAPTimeout;
  if (gIAPTimeout > 0)
  {
    gIAPTimeout--;
    if (gIAPTimeout == 0)
    {
      /* Wait IAP command timeout. Reset and start application                                              */
      IAP_Reset(0);
    }
  }
  #endif
}

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles USB interrupt.
 * @retval  None
 ************************************************************************************************************/
void USB_IRQHandler(void)
{
  __ALIGN4 extern USBDCore_TypeDef gUSBCore;
  USBDCore_IRQHandler(&gUSBCore);
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

/**
  * @}
  */
