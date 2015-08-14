/*********************************************************************************************************//**
 * @file    USBD/Mass_Storage_IAP/Src_AP/ht32f165x_it.c
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
#include "ht32_board.h"
#include "ht32_usbd_core.h"
#include "cigarette.h"
#include "main.h"
u32 gADC_Result[6];
volatile bool gADC_CycleEndOfConversion;
u16 counter_bftm=0;
extern u16 ZKB_L;
extern bool SCP_STATE;//短路
extern bool Keep_delay;
extern bool Keep_Add;
extern bool Keep_Sub;
extern Sleep_off_typedef Auto_off;
extern u16 Return_main_time;
//extern bool ReturnToMain_Conter;
/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup USBD_Examples USBD
  * @{
  */

/** @addtogroup USBD_Mass_Storage_IAP USBD Mass Storage IAP
  * @{
  */

/** @addtogroup Mass_Storage_AP AP
  * @{
  */


/* Private variables ---------------------------------------------------------------------------------------*/
//static u32 LedFlag = 0;

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
  extern vu32 gTimebaseDelayCounter;
	extern bool K_Count;
	extern bool Key_Delay;
	extern vu32 Key_Delaycnt;
	extern u32 gTimebaseDelayCounter_2s;
	extern u16 Add_delay;
	extern u16 Sub_delay;
//	extern u16 bat_time;
//	bat_time++;//电量变化间隔时间
//	if(bat_time>6000)
//		bat_time=0;
	if(K_Count)//开关机3s
		gTimebaseDelayCounter++;
	if(Key_Delay)//连续按3次和5次的延时0.7S
		Key_Delaycnt++;
	if(Keep_Add)//0.1秒功率加1
	{
		if(Add_delay)
			Add_delay--;
	}
	if(Keep_Sub)//0.1秒功率减1
	{
		if(Sub_delay)
			Sub_delay--;
	}
	if(Keep_delay)//两秒进入功率常加常减
	{
		gTimebaseDelayCounter_2s++;
	}
	if(Auto_off.key_free==FALSE)//按键按下
	{
		Auto_off.sleep_time=0;
		Return_main_time=0;
	}
	else
	{
		Auto_off.sleep_time++;//睡眠时间
		Return_main_time++;//6s返回主菜单
	}
		
	if(Auto_off.sleep==TRUE)//睡眠
		Auto_off.off_time++;//关机时间
	else
		Auto_off.off_time=0;
	PWM_Control();//升压控制程序
//	if(gTimebaseDelayCounter==3000)
//		gTimebaseDelayCounter=0;
}
/*********************************************************************************************************//**
 * @brief   This function handles USB interrupt.
 * @retval  None
 ************************************************************************************************************/
extern bool PC_STATE;
void USB_IRQHandler(void)
{
  __ALIGN4 extern USBDCore_TypeDef gUSBCore;
  USBDCore_IRQHandler(&gUSBCore);
	OLED_Display_On();
	pc_connect();
	PC_STATE=TRUE;
}
/*********************************************************************************************************//**
 * @brief   This function handles ADC interrupt.
 * @retval  None
 ************************************************************************************************************/
void ADC_IRQHandler(void)
{
  ADC_ClearIntPendingBit(HT_ADC, ADC_FLAG_CYCLE_EOC);
  gADC_CycleEndOfConversion = TRUE;
  gADC_Result[0] = HT_ADC->DR[0] & 0x0FFF;
  gADC_Result[1] = HT_ADC->DR[1] & 0x0FFF;
  gADC_Result[2] = HT_ADC->DR[2] & 0x0FFF;
	gADC_Result[3] = HT_ADC->DR[3] & 0x0FFF;
   	gADC_Result[4] = HT_ADC->DR[4] & 0x0FFF;
}
/*********************************************************************************************************//**
 * @brief   This function handles MCTM1 UP interrupt.
 * @retval  None
 ************************************************************************************************************/
void MCTM0UP_IRQHandler(void)
{
  extern __IO uint32_t wMctmUpdateDownCounter;
  MCTM_ClearFlag(BUZZER_TM, MCTM_INT_UEV1);
  if (wMctmUpdateDownCounter)
    wMctmUpdateDownCounter--;
}
/*********************************************************************************************************//**
 * @brief   This function handles BFTM0 interrupt.
 * @retval  None
 ************************************************************************************************************/
u8 EXTI_delay=0;
void BFTM0_IRQHandler(void)
{
	
	counter_bftm++;
//	if(GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_0)==0)
//		GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_0);//pwm1
//	else
	
	if(counter_bftm>=500)
	{
		PWM2_Control();//降压操作
		counter_bftm=0;
	}
	if(counter_bftm<ZKB_L)
	{
		GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_0);//pwm1
	}
	else
	{
		GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_0);//PWM1
		EXTI_delay++;
		if(EXTI_delay>10)
		{
			EXTI_IntConfig(EXTI_CHANNEL_14, ENABLE);//开短路中断
			EXTI_delay=0;
		}
	  
	}
		
  BFTM_ClearFlag(HT_BFTM0);
}
/*********************************************************************************************************//**
 * @brief   This function handles EXTI14 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI14_IRQHandler(void)
{
	EXTI_ClearEdgeFlag(EXTI_CHANNEL_14);
	SCP_STATE=TRUE;//输出端短路
}
#if 0
/*********************************************************************************************************//**
 * @brief   This function handles EXTI Events Wakeup interrupt.
 * @retval  None
 * @details The EXTI Event Wakeup interrupt handler as following:
 *    - If the EXTI Channel wakeup event occurred
 *      - Disable button EXTI Channel wakeup event since it's active on level.
 *      - Clear the Key Button EXTI channel event wakeup flag.
 *      - Toggle LED2.
 *    - If the RTC compare match event occurred
 *      - Toggle LED3.
 * @note The RTC_SR is clear by read action.
 ************************************************************************************************************/
void EVWUP_IRQHandler(void)
{
  if (EXTI_GetWakeupFlagStatus(WAKEUP_BUTTON_EXTI_CHANNEL) )
  {
    /* Disable button EXTI Channel wakeup event to avoid re-entry                                           */
    EXTI_WakeupEventConfig(WAKEUP_BUTTON_EXTI_CHANNEL, EXTI_WAKEUP_LOW_LEVEL, DISABLE);

    /* Clear the Key Button EXTI channel event wakeup flag                                                  */
    EXTI_ClearWakeupFlag(WAKEUP_BUTTON_EXTI_CHANNEL);
  }
}
#endif
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
