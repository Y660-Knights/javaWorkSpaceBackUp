/*********************************************************************************************************//**
 * @file   main.h
 * @version V1.00
 * @date    03/04/2014
 * @brief   The header file of main.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "ap_handler.h"
#include "ap_config.h"
#include "spi_flash.h"
#include "ht32_usbd_core.h"
#include "ht32_usbd_class.h"
#include "ht32_usbd_descriptor.h"

#include "common.h"
#include "oled.h"
#include "oled_display.h"
/* Settings ------------------------------------------------------------------------------------------------*/
#define IAP_VERSION_PAGE              (1)
#define IAP_VERSION_LENGTH            (16)
#define IAP_CODE_PAGE                 (10)
#define IAP_CODE_SIZE                 (1024 * IAP_CODE_PAGE)    /* IAP code size in Bytes                  */

/* Private typedef -----------------------------------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------------------------------------*/
#define IAP_APFALSH_START             (IAP_CODE_SIZE + (IAP_VERSION_PAGE * 1024))
#define BOOT_MODE_IAP                 (0x55AAFAF5)


/* Private function prototypes -----------------------------------------------------------------------------*/



/* Setting -----------------------------------------------------------------------------------------------*/
#define WDT_EN                1
#define WDT_COUNTER_VALUE    (2500) // Unit : 4mS
#define WDT_TST              (1)

#if (WDT_EN != 0)      
#define WDT_RELOAD()  WDT_Restart()
#else
#define WDT_RELOAD(...)
#endif
//systick_setting
#define HCLK  72000000
#define TIMEBASE_US             (1000)//1MS
#define SYSTICK_SRC_SEL         SYSTICK_SRC_FCLK    /* FCLK=72MHz, STCLK=FCLK/8=9MHz */
#define SYSTICK_1US_MUL         (HCLK/1000000)                /* 72MHz/1MHz=72 */
#define SYSTICK_RELOAD_SET      ((TIMEBASE_US*SYSTICK_1US_MUL)-1)

typedef struct
{
	bool sleep;
	bool turn_off;
	bool key_free;
	u32 sleep_time;
	u32 off_time;
}Sleep_off_typedef;
/* Exported constants --------------------------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
void CKCU_Configuration(void);
void NVIC_Configuration(void);
void USBD_Configuration(void);
void GPIO_Configuration(void);
void key_contrl(void);
void IN_OUT_GPIO(void);
void SYSTICK_Configuration(void);
void R_Check(void);
void Sleep_handle(void);
void W_handle(void);
void EXTI_Configuration(void);
void SCP_Int(void);
void SCP_Handle(void);
void USB_CHARG_Int(void);
void USB_CHARG_Handle(void);
void Sleep_handle(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H -----------------------------------------------------------------------------------*/
