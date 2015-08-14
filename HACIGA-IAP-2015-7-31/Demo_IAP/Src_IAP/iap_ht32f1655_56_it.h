/*********************************************************************************************************//**
 * @file    USBD/HID_Keyboard/ht32f1655_56_it.h
 * @version V1.00
 * @date    10/01/2013
 * @brief   The header file of interrupt handlers.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2013 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F1655_56_IT_H
#define __HT32F1655_56_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1655_56.h"

/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void CKRDY_IRQHandler(void);
void LVD_IRQHandler(void);
void BOD_IRQHandler(void);
void WDT_IRQHandler(void);
void RTC_IRQHandler(void);
void FLASH_IRQHandler(void);
void EVWUP_IRQHandler(void);
void LPWUP_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI5_IRQHandler(void);
void EXTI6_IRQHandler(void);
void EXTI7_IRQHandler(void);
void EXTI8_IRQHandler(void);
void EXTI9_IRQHandler(void);
void EXTI10_IRQHandler(void);
void EXTI11_IRQHandler(void);
void EXTI12_IRQHandler(void);
void EXTI13_IRQHandler(void);
void EXTI14_IRQHandler(void);
void EXTI15_IRQHandler(void);
void COMP_IRQHandler(void);
void ADC_IRQHandler(void);
void MCTM0BRK_IRQHandler(void);
void MCTM0UP_IRQHandler(void);
void MCTM0TR_IRQHandler(void);
void MCTM0CC_IRQHandler(void);
void MCTM1BRK_IRQHandler(void);
void MCTM1UP_IRQHandler(void);
void MCTM1TR_IRQHandler(void);
void MCTM1CC_IRQHandler(void);
void GPTM0_IRQHandler(void);
void GPTM1_IRQHandler(void);
void BFTM0_IRQHandler(void);
void BFTM1_IRQHandler(void);
void I2C0_IRQHandler(void);
void I2C1_IRQHandler(void);
void SPI0_IRQHandler(void);
void SPI1_IRQHandler(void);
void USART0_IRQHandler(void);
void USART1_IRQHandler(void);
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);
void SCI_IRQHandler(void);
void I2S_IRQHandler(void);
void USB_IRQHandler(void);
void PDMA_CH0_IRQHandler(void);
void PDMA_CH1_IRQHandler(void);
void PDMA_CH2_IRQHandler(void);
void PDMA_CH3_IRQHandler(void);
void PDMA_CH4_IRQHandler(void);
void PDMA_CH5_IRQHandler(void);
void PDMA_CH6_IRQHandler(void);
void PDMA_CH7_IRQHandler(void);
void EBI_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __HT32F1655_56_IT_H ---------------------------------------------------------------------------------*/
