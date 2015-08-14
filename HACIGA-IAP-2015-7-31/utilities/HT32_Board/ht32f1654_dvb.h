/*********************************************************************************************************//**
 * @file    ht32f1654_dvb.h
 * @version $Rev:: 307          $
 * @date    $Date:: 2014-12-31 #$
 * @brief   The header file of ht32f1654_dvb.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F1654_DVB_H
#define __HT32F1654_DVB_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "ht32f1653_54.h"
#include "ht32f1653_54.h"
/** @addtogroup Utilities
  * @{
  */

/** @addtogroup HT32_Board
  * @{
  */

/** @addtogroup HT32F1654_DVB
  * @{
  */


/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup HT32F1654_DVB_Exported_Constants HT32F1654 DVB exported constants
  * @{
  */
extern HT_GPIO_TypeDef* const GPIO_PORT[4];

/** @addtogroup HT32F1654_DVB_LED
  * @{
  */
#define LEDn                        (3)

#define LED1_GPIO_ID                (GPIO_PD)
#define LED1_GPIO_PIN               (GPIO_PIN_0)
#define LED1_AFIO_MODE              (AFIO_MODE_DEFAULT)

#define LED2_GPIO_ID                (GPIO_PD)
#define LED2_GPIO_PIN               (GPIO_PIN_2)
#define LED2_AFIO_MODE              (AFIO_MODE_DEFAULT)

#define LED3_GPIO_ID                (GPIO_PB)
#define LED3_GPIO_PIN               (GPIO_PIN_6)
#define LED3_AFIO_MODE              (AFIO_MODE_DEFAULT)

typedef enum
{
  HT_LED1 = 0,
  HT_LED2 = 1,
  HT_LED3 = 2
} LED_TypeDef;
/**
  * @}
  */

/** @addtogroup HT32F1654_DVB_BUTTON
  * @{
  */
#define BUTTONn                     (3)

#define WAKEUP_BUTTON_GPIO_ID       (GPIO_PC)
#define WAKEUP_BUTTON_GPIO_PIN      (GPIO_PIN_15)
#define WAKEUP_BUTTON_AFIO_MODE     (AFIO_FUN_GPIO)
#define WAKEUP_BUTTON_EXTI_CHANNEL  (15)

#define KEY1_BUTTON_GPIO_ID         (GPIO_PD)
#define KEY1_BUTTON_GPIO_PIN        (GPIO_PIN_1)
#define KEY1_BUTTON_AFIO_MODE       (AFIO_MODE_DEFAULT)
#define KEY1_BUTTON_EXTI_CHANNEL    (1)

#define KEY2_BUTTON_GPIO_ID         (GPIO_PB)
#define KEY2_BUTTON_GPIO_PIN        (GPIO_PIN_9)
#define KEY2_BUTTON_AFIO_MODE       (AFIO_MODE_DEFAULT)
#define KEY2_BUTTON_EXTI_CHANNEL    (9)

typedef enum
{
  BUTTON_WAKEUP = 0,
  BUTTON_KEY1   = 1,
  BUTTON_KEY2   = 2
} BUTTON_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} BUTTON_MODE_TypeDef;
/**
  * @}
  */

/** @addtogroup HT32F1654_DVB_BUZZER
  * @{
  */
#define BUZZER_GPIO_ID0              (GPIO_PB)//(GPIO_PA)
#define BUZZER_AFIO_PIN0            (AFIO_PIN_2)//(AFIO_PIN_10)
#define BUZZER_AFIO_MODE0            (AFIO_FUN_MCTM_GPTM)


#define BUZZER_GPIO_ID              (GPIO_PB)//(GPIO_PA)
#define BUZZER_AFIO_PIN             (AFIO_PIN_2)//(AFIO_PIN_10)
#define BUZZER_AFIO_MODE            (AFIO_FUN_MCTM_GPTM)
#define BUZZER_GPIO_CLK(CK)         (CK.Bit.PB)//(CK.Bit.PA)

#define BUZZER_TM_CLK(CK)           (CK.Bit.MCTM0)//(CK.Bit.MCTM1)
#define BUZZER_TM                   (HT_MCTM0)//(HT_MCTM1)
#define BUZZER_TM_CHANNEL           (MCTM_CH_2)//(MCTM_CH_1)
#define BUZZER_IRQn                 (MCTM0UP_IRQn)//(MCTM1UP_IRQn)
/**
  * @}
  */

/** @addtogroup HT32F1654_DVB_COM
  * @{
  */
#define COMn                        (1)

#define COM1_CLK(CK)                (CK.Bit.USART0)
#define COM1_PORT                   (HT_USART0)
#define COM1_IRQn                   (USART0_IRQn)

#define COM1_TX_GPIO_ID             (GPIO_PA)
#define COM1_TX_AFIO_PIN            (AFIO_PIN_2)
#define COM1_TX_AFIO_MODE           (AFIO_FUN_USART_UART)

#define COM1_RX_GPIO_ID             (GPIO_PA)
#define COM1_RX_AFIO_PIN            (AFIO_PIN_3)
#define COM1_RX_AFIO_MODE           (AFIO_FUN_USART_UART)

typedef enum
{
  COM1 = 0
} COM_TypeDef;
/**
  * @}
  */


/** @addtogroup HT32F1654_DVB_SPI_FLASH
  * @{
  */
#define FLASH_SPI_CLK(CK)           (CK.Bit.SPI1)
#define FLASH_SPI                   (HT_SPI1)

#define FLASH_SPI_SCK_GPIO_ID       (GPIO_PC)
#define FLASH_SPI_SCK_AFIO_PIN      (AFIO_PIN_10)
#define FLASH_SPI_SCK_AFIO_MODE     (AFIO_FUN_SPI)

#define FLASH_SPI_MOSI_GPIO_ID      (GPIO_PC)
#define FLASH_SPI_MOSI_AFIO_PIN     (AFIO_PIN_11)
#define FLASH_SPI_MOSI_AFIO_MODE    (AFIO_FUN_SPI)

#define FLASH_SPI_MISO_GPIO_ID      (GPIO_PC)
#define FLASH_SPI_MISO_AFIO_PIN     (AFIO_PIN_12)
#define FLASH_SPI_MISO_AFIO_MODE    (AFIO_FUN_SPI)

#define FLASH_SPI_SEL_GPIO_ID       (GPIO_PA)
#define FLASH_SPI_SEL_GPIO_PIN      (GPIO_PIN_8)
#define FLASH_SPI_SEL_AFIO_MODE     (AFIO_MODE_DEFAULT)
#define FLASH_SPI_SEL_CLK(CK)       (CK.Bit.PA)

/**
  * @}
  */

/** @addtogroup HT32F1654_DVB_EBI_LCD
  * @{
  */
#define LCD_EBI                     (HT_EBI)

#define LCD_EBI_CS_GPIO_ID          (GPIO_PB)
#define LCD_EBI_CS_AFIO_PIN         (AFIO_PIN_7)
#define LCD_EBI_CS_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_OE_GPIO_ID          (GPIO_PB)
#define LCD_EBI_OE_AFIO_PIN         (AFIO_PIN_6)
#define LCD_EBI_OE_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_WE_GPIO_ID          (GPIO_PB)
#define LCD_EBI_WE_AFIO_PIN         (AFIO_PIN_8)
#define LCD_EBI_WE_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_RS_GPIO_ID          (GPIO_PA)
#define LCD_EBI_RS_AFIO_PIN         (AFIO_PIN_11)
#define LCD_EBI_RS_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_AD0_GPIO_ID         (GPIO_PA)
#define LCD_EBI_AD0_AFIO_PIN        (AFIO_PIN_14)
#define LCD_EBI_AD0_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD1_GPIO_ID         (GPIO_PA)
#define LCD_EBI_AD1_AFIO_PIN        (AFIO_PIN_15)
#define LCD_EBI_AD1_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD2_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD2_AFIO_PIN        (AFIO_PIN_0)
#define LCD_EBI_AD2_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD3_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD3_AFIO_PIN        (AFIO_PIN_1)
#define LCD_EBI_AD3_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD4_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD4_AFIO_PIN        (AFIO_PIN_2)
#define LCD_EBI_AD4_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD5_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD5_AFIO_PIN        (AFIO_PIN_3)
#define LCD_EBI_AD5_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD6_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD6_AFIO_PIN        (AFIO_PIN_4)
#define LCD_EBI_AD6_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD7_GPIO_ID         (GPIO_PB)
#define LCD_EBI_AD7_AFIO_PIN        (AFIO_PIN_5)
#define LCD_EBI_AD7_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD8_GPIO_ID         (GPIO_PC)
#define LCD_EBI_AD8_AFIO_PIN        (AFIO_PIN_7)
#define LCD_EBI_AD8_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD9_GPIO_ID         (GPIO_PC)
#define LCD_EBI_AD9_AFIO_PIN        (AFIO_PIN_8)
#define LCD_EBI_AD9_AFIO_MODE       (AFIO_FUN_EBI)

#define LCD_EBI_AD10_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD10_AFIO_PIN       (AFIO_PIN_4)
#define LCD_EBI_AD10_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_AD11_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD11_AFIO_PIN       (AFIO_PIN_5)
#define LCD_EBI_AD11_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_AD12_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD12_AFIO_PIN       (AFIO_PIN_6)
#define LCD_EBI_AD12_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_AD13_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD13_AFIO_PIN       (AFIO_PIN_0)
#define LCD_EBI_AD13_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_AD14_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD14_AFIO_PIN       (AFIO_PIN_1)
#define LCD_EBI_AD14_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_AD15_GPIO_ID        (GPIO_PC)
#define LCD_EBI_AD15_AFIO_PIN       (AFIO_PIN_2)
#define LCD_EBI_AD15_AFIO_MODE      (AFIO_FUN_EBI)

#define LCD_EBI_RST_GPIO_ID         (GPIO_PC)
#define LCD_EBI_RST_GPIO_PIN        (GPIO_PIN_3)
#define LCD_EBI_RST_AFIO_MODE       (AFIO_MODE_DEFAULT)
#define LCD_EBI_RST_CLK(CK)         (CK.Bit.PC)
/**
  * @}
  */

/** @addtogroup HT32F1654_DVB_I2C_EEPROM
  * @{
  */
#define EEPROM_I2C_CLK(CK)          (CK.Bit.I2C1)
#define EEPROM_I2C                  (HT_I2C1)

#define EEPROM_I2C_SCL_GPIO_ID      (GPIO_PA)
#define EEPROM_I2C_SCL_AFIO_PIN     (AFIO_PIN_0)
#define EEPROM_I2C_SCL_AFIO_MODE    (AFIO_FUN_I2C)

#define EEPROM_I2C_SDA_GPIO_ID      (GPIO_PA)
#define EEPROM_I2C_SDA_AFIO_PIN     (AFIO_PIN_1)
#define EEPROM_I2C_SDA_AFIO_MODE    (AFIO_FUN_I2C)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup HT32F1654_DVB_Exported_Functions HT32F1654 DVB exported functions
  * @{
  */
#define HT32F_DVB_USBConnect()      USBD_DPpullupCmd(ENABLE)
#define HT32F_DVB_USBDisConnect()   USBD_DPpullupCmd(DISABLE)
#define HT32F_DVB_GPxConfig         AFIO_GPxConfig

#define HT32F_DVB_BuzzerInit(hFreq, bDuty)    HT32F_DVB_BuzzerFun(hFreq, bDuty, 0);
#define HT32F_DVB_BuzzerConfig(hFreq, bDuty)  HT32F_DVB_BuzzerFun(hFreq, bDuty, 1);


void HT32F_DVB_LEDInit(LED_TypeDef Led);
void HT32F_DVB_LEDOn(LED_TypeDef Led);
void HT32F_DVB_LEDOff(LED_TypeDef Led);
void HT32F_DVB_LEDToggle(LED_TypeDef Led);

void HT32F_DVB_PBInit(BUTTON_TypeDef Button, BUTTON_MODE_TypeDef Button_Mode);
u32 HT32F_DVB_PBGetState(BUTTON_TypeDef Button);

//void HT32F_DVB_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);

void HT32F_DVB_BuzzerFun(u16 hFreq, u8 bDuty, u32 mode);
void HT32F_DVB_BuzzerOutputCmd(ControlStatus Control);
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

#ifdef __cplusplus
}
#endif

#endif /* __HT32F1654_DVB_H --------------------------------------------------------------------------------*/
