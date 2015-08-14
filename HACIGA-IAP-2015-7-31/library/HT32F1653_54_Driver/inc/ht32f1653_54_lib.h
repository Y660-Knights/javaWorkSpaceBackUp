/*********************************************************************************************************//**
 * @file    ht32f1653_54_lib.h
 * @version $Rev:: 307          $
 * @date    $Date:: 2014-12-31 #$
 * @brief   The header file includes all the header files of the libraries.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F1653_54_LIB_H
#define __HT32F1653_54_LIB_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "ht32f1653_54_conf.h"

#if (HT32_LIB_DEBUG == 1)
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed.
  *   If expr is true, it returns no value.
  * @retval None
  */
#define Assert_Param(expr) ((expr) ? (void)0 : assert_error((u8 *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- ------------------------------*/
void assert_error(u8* file, u32 line);
#else

#define Assert_Param(expr) ((void)0)

#endif /* DEBUG --------------------------------------------------------------------------------------------*/


#if _USART
  #include "ht32f1653_54_usart.h"
#endif

#if _SPI
  #include "ht32f1653_54_spi.h"
#endif

#if _ADC
  #include "ht32f1653_54_adc.h"
#endif

#if _CMP
  #include "ht32f1653_54_cmp.h"
#endif

#if _GPIO
  #include "ht32f1653_54_gpio.h"
#endif

#if _EXTI
  #include "ht32f1653_54_exti.h"
#endif

#if _MCTM
  #include "ht32f1653_54_mctm.h"
#endif

#if _SCI
  #include "ht32f1653_54_sci.h"
#endif

#if _I2S
  #include "ht32f1653_54_i2s.h"
#endif

#if _I2C
  #include "ht32f1653_54_i2c.h"
#endif

#if _USB
  #include "ht32f1653_54_usbd.h"
#endif

#if _WDT
  #include "ht32f1653_54_wdt.h"
#endif

#if _RTC
  #include "ht32f1653_54_rtc.h"
#endif

#if _PWRCU
  #include "ht32f1653_54_pwrcu.h"
#endif

#if _GPTM
  #include "ht32f1653_54_gptm.h"
#endif

#if _BFTM
  #include "ht32f1653_54_bftm.h"
#endif

#if _FLASH
  #include "ht32f1653_54_flash.h"
#endif

#if _CKCU
  #include "ht32f1653_54_ckcu.h"
#endif

#if _RSTCU
  #include "ht32f1653_54_rstcu.h"
#endif

#if _CRC
  #include "ht32f1653_54_crc.h"
#endif

#if _PDMA
  #include "ht32f1653_54_pdma.h"
#endif

#if _EBI
  #include "ht32f1653_54_ebi.h"
#endif

#if _MISC
  #include "ht32_cm3_misc.h"
#endif

#if (_RETARGET)
  #if defined (__GNUC__)
    #undef getchar
    #define getchar SERIAL_GetChar
  #endif
#endif

#include "ht32_serial.h"

#ifdef __cplusplus
}
#endif

#endif
