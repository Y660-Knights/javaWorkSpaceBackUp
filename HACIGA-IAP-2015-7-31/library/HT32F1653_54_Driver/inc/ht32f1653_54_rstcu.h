/*********************************************************************************************************//**
 * @file    ht32f1653_54_rstcu.h
 * @version $Rev:: 307          $
 * @date    $Date:: 2014-12-31 #$
 * @brief   The header file of the Reset Control Unit library.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F1653_54_RSTCU_H
#define __HT32F1653_54_RSTCU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1653_54.h"

/** @addtogroup HT32F1653_54_Peripheral_Driver HT32F1653/1654 Peripheral Driver
  * @{
  */

/** @addtogroup RSTCU
  * @{
  */


/* Exported types ------------------------------------------------------------------------------------------*/
/** @defgroup RSTCU_Exported_Types RSTCU exported types
  * @{
  */

/**
 * @brief Enumeration of Global reset status.
 */
typedef enum
{
  RSTCU_FLAG_SYSRST = 0,
  RSTCU_FLAG_EXTRST,
  RSTCU_FLAG_WDTRST,
  RSTCU_FLAG_PORST
} RSTCU_RSTF_TypeDef;

/**
 * @brief Definition of initial structure of peripheral reset.
 */
typedef union
{
  struct
  {
    unsigned long PDMA       :1;
    unsigned long            :4;
    unsigned long USBD       :1;
    unsigned long EBI        :1;
    unsigned long CRC        :1;
    unsigned long PA         :1;
    unsigned long PB         :1;
    unsigned long PC         :1;
    unsigned long PD         :1;
    unsigned long REV0       :1;
    unsigned long            :19;

    unsigned long I2C0       :1;
    unsigned long I2C1       :1;
    unsigned long            :2;
    unsigned long SPI0       :1;
    unsigned long SPI1       :1;
    unsigned long            :2;
    unsigned long USART0     :1;
    unsigned long USART1     :1;
    unsigned long UART0      :1;
    unsigned long UART1      :1;
    unsigned long            :2;
    unsigned long AFIO       :1;
    unsigned long EXTI       :1;
    unsigned long            :8;
    unsigned long SCI        :1;
    unsigned long I2S        :1;
    unsigned long            :6;

    unsigned long MCTM0      :1;
    unsigned long MCTM1      :1;
    unsigned long            :2;
    unsigned long WDT        :1;
    unsigned long            :3;
    unsigned long GPTM0      :1;
    unsigned long GPTM1      :1;
    unsigned long            :6;
    unsigned long BFTM0      :1;
    unsigned long BFTM1      :1;
    unsigned long            :4;
    unsigned long CMP        :1;
    unsigned long REV1       :1;
    unsigned long ADC        :1;
    unsigned long            :7;
  } Bit;
  u32 Reg[3];
} RSTCU_PeripReset_TypeDef;

/**
  * @}
  */


/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup RSTCU_Exported_Constants RSTCU exported constants
  * @{
  */

/* Other definitions                                                                                        */
#define IS_RSTCU_FLAG(FLAG)       ((FLAG == RSTCU_FLAG_SYSRST) || \
                                   (FLAG == RSTCU_FLAG_EXTRST) || \
                                   (FLAG == RSTCU_FLAG_WDTRST) || \
                                   (FLAG == RSTCU_FLAG_PORST))

/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup RSTCU_Exported_Functions RSTCU exported functions
  * @{
  */
FlagStatus RSTCU_GetResetFlagStatus(RSTCU_RSTF_TypeDef RSTCU_RSTF);
void RSTCU_ClearResetFlag(RSTCU_RSTF_TypeDef RSTCU_RSTF);
void RSTCU_ClearAllResetFlag(void);
void RSTCU_PeripReset(RSTCU_PeripReset_TypeDef Reset, ControlStatus Cmd);

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
