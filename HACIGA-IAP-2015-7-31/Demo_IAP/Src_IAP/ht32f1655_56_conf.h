/*********************************************************************************************************//**
 * @file    USBD/HID_Keyboard/ht32f1655_56_conf.h
 * @version V1.00
 * @date    10/01/2013
 * @brief   Library configuration file.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2013 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
*/
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F1655_56_CONF_H
#define __HT32F1655_56_CONF_H

/* Includes ------------------------------------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/

/* The DEBUG definition to enter debug mode for library                                                     */
/*
//<e> Library Debug mode
//</e>
*/
#define HT32_LIB_DEBUG      0


#define RETARGET_USART0     1
#define RETARGET_USART1     2
#define RETARGET_UART0      3
#define RETARGET_UART1      4
#define RETARGET_ITM        5

/* RETARGET definition to use USART function by C Library                                                   */
/*
//  <e0> Enable Retarget
//  <o1> Retarget port
//      <1=> USART0
//      <2=> USART1
//      <3=> UART0
//      <4=> UART1
//      <5=> ITM
//  </e>
*/
#define _RETARGET            1
#define RETARGET_PORT        3

#if (RETARGET_PORT == RETARGET_USART0)
  #define RETARGET_USART_PORT       HT_USART0
  #define RETARGET_COM_PORT         COM1
#elif (RETARGET_PORT == RETARGET_USART1)
  #define RETARGET_USART_PORT       HT_USART1
  #define RETARGET_COM_PORT         COM3
#elif (RETARGET_PORT == RETARGET_UART0)
  #define RETARGET_USART_PORT       HT_UART0
  #define RETARGET_COM_PORT         COM2
#elif (RETARGET_PORT == RETARGET_UART1)
  #define RETARGET_USART_PORT       HT_UART1
  #define RETARGET_COM_PORT         COM4
#endif


#define ENABLE_CKOUT              0


/* Enable/disable the specific peripheral inclusion                                                         */

/* USART ---------------------------------------------------------------------------------------------------*/
/*
//<e0> USART Library
//  <q1> USART0
//  <q2> USART1
//  <q3> UART0
//  <q4> UART1
//</e>
*/
#define _USART        1
#if _USART
#define _USART0       1
#define _USART1       1
#define _UART0        1
#define _UART1        1
#endif

/* SPI -----------------------------------------------------------------------------------------------------*/
/*
//<e0> SPI Library
//  <q1> SPI0
//  <q2> SPI1
//</e>
*/
#define _SPI          1
#if _SPI
#define _SPI0         1
#define _SPI1         1
#endif

/* ADC -----------------------------------------------------------------------------------------------------*/
/*
//<e0> ADC Library
//</e>
*/
#define _ADC          1

/* Comparator/OPA ------------------------------------------------------------------------------------------*/
/*
//<e0> Comparator Library
//  <q1> CMP_OP0
//  <q2> CMP_OP1
//</e>
*/
#define _CMP_OP       1
#if _CMP_OP
#define _CMP_OP0      1
#define _CMP_OP1      1
#endif

/* GPIO ----------------------------------------------------------------------------------------------------*/
/*
//<e0> GPIO Library
//  <q1> AFIO
//  <q2> GPIO Port A
//  <q3> GPIO Port B
//  <q4> GPIO Port C
//  <q5> GPIO Port D
//  <q6> GPIO Port E
//</e>
*/
#define _GPIO         1
#if _GPIO
#define _AFIO         1
#define _GPIOA        1
#define _GPIOB        1
#define _GPIOC        1
#define _GPIOD        1
#define _GPIOE        1
#endif

/* EXTI ----------------------------------------------------------------------------------------------------*/
/*
//<e0> EXTI Library
//</e>
*/
#define _EXTI         1

/* MCTM ----------------------------------------------------------------------------------------------------*/
/*
//<e0> MCTM Library
//  <q1> MCTM0
//  <q2> MCTM1
//</e>
*/
#define _MCTM         1
#if _MCTM
#define _MCTM0        1
#define _MCTM1        1
#endif

/* SCI  ----------------------------------------------------------------------------------------------------*/
/*
//<e0> SCI Library
//</e>
*/
#define _SCI          1

/* I2S  ----------------------------------------------------------------------------------------------------*/
/*
//<e0> I2S Library
//</e>
*/
#define _I2S          1

/* I2C -----------------------------------------------------------------------------------------------------*/
/*
//<e0> I2C Library
//  <q1> I2C0
//  <q2> I2C1
//</e>
*/
#define _I2C          1
#if _I2C
#define _I2C0         1
#define _I2C1         1
#endif

/* USBD ----------------------------------------------------------------------------------------------------*/
/*
//<e0> USB Library
//</e>
*/
#define _USB          1
#if _USB
#define _USBEP0       1
#define _USBEP1       1
#define _USBEP2       1
#define _USBEP3       1
#define _USBEP4       1
#define _USBEP5       1
#define _USBEP6       1
#define _USBEP7       1
#endif

/* WDT -----------------------------------------------------------------------------------------------------*/
/*
//<e0> WDT Library
//</e>
*/
#define _WDT          1

/* RTC -----------------------------------------------------------------------------------------------------*/
/*
//<e0> RTC Library
//</e>
*/
#define _RTC          1

/* PWRCU ---------------------------------------------------------------------------------------------------*/
/*
//<e0> PWRCU Library
//</e>
*/
#define _PWRCU        1

/* GPTM ----------------------------------------------------------------------------------------------------*/
/*
//<e0> GPTM Library
//  <q1> GPTM0
//  <q2> GPTM1
//</e>
*/
#define _GPTM         1
#if _GPTM
#define _GPTM0        1
#define _GPTM1        1
#endif

/* BFTM ----------------------------------------------------------------------------------------------------*/
/*
//<e0> BFTM Library
//  <q1> BFTM0
//  <q2> BFTM1
//</e>
*/
#define _BFTM         1
#if _BFTM
#define _BFTM0        1
#define _BFTM1        1
#endif

/* Flash control -------------------------------------------------------------------------------------------*/
/*
//<e0> Flash Control Library
//</e>
*/
#define _FLASH        1

/* Clock Control -------------------------------------------------------------------------------------------*/
/*
//<e0> Clock Control Library
//</e>
*/
#define _CKCU         1

/* RSTCU ---------------------------------------------------------------------------------------------------*/
/*
//<e0> RSTCU Library
//</e>
*/
#define _RSTCU        1

/* CRC -----------------------------------------------------------------------------------------------------*/
/*
//<e0> CRC Library
//</e>
*/
#define _CRC          1

/* PDMA ----------------------------------------------------------------------------------------------------*/
/*
//<e0> PDMA Library
//</e>
*/
#define _PDMA         1

/* EBI -----------------------------------------------------------------------------------------------------*/
/*
//<e0> EBI Library
//</e>
*/
#define _EBI          1

/* Misc ----------------------------------------------------------------------------------------------------*/
/*
//<e0> Misc Library
//</e>
*/
#define _MISC         1


/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/

#endif /* __HT32F1655_56_CONF_H ----------------------------------------------------------------------------*/
