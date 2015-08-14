/*********************************************************************************************************//**
 * @file    USBD/STD_GK_Vxx/Src_IAP/iap_main.c
 * @version V1.00
 * @date    6/10/2013
 * @brief   The main program.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/

#include "ht32.h"
#include "ht32_board.h"
#include "iap_handler.h"
#include "iap_config.h"

#include "ht32_usbd_core.h"
#include "iap_ht32_usbd_class.h"
#include "iap_ht32_usbd_descriptor.h"

/* Setting -----------------------------------------------------------------------------------------------*/

#define WDT_EN                0
#define WDT_COUNTER_VALUE    (3000)
#define WDT_TST              (1)

/* Private typedef -----------------------------------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------------------------------------*/
#define TIMER_BASE                    (SystemCoreClock * 1 / 1000)  /* 1 ms  */

#define FLASH_CMD_PROGRAM       ((u32)0x00000004)

#if (WDT_EN != 0)      
#define WDT_RELOAD()  WDT_Restart()
#else
#define WDT_RELOAD(...)
#endif

/* Private function prototypes -----------------------------------------------------------------------------*/

void CKCU_Configuration(void);
void USBD_Configuration(void);
void Suspend(u32 uPara);

/* Private macro -------------------------------------------------------------------------------------------*/
/* Global variables ----------------------------------------------------------------------------------------*/
__ALIGN4 USBDCore_TypeDef gUSBCore;
USBD_Driver_TypeDef gUSBDriver;

#if (IAP_TIMEOUT_EN == 1)
s32 gIAPTimeout = IAP_TIMEOUT;
#endif
#define RSTCU_APBRST1_WDT         ((u32)0x1 << 4)
/* Private variables ---------------------------------------------------------------------------------------*/
/* Global functions ----------------------------------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------------------------------------*/
extern void FLASH_Operation(u32 type, u32 PageAddress, u32 Data);
void RSTCU_APBPerip1Reset(u32 RSTCU_APBP, ControlStatus Cmd);
void WDT_Configuration(void);
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
#if (HT32_LIB_DEBUG == 1)
  debug();
#endif

  CKCU_Configuration();               /* System Related configuration                                       */
#if (WDT_EN != 0)     
	WDT_Configuration();  
#endif
  /*--------------------------------------------------------------------------------------------------------*/
  /* Use BOOT0 (PC8) to decide start user application or IAP mode.                                          */
  /* Modify it if you want to use another GPIO pin.                                                         */
  /*--------------------------------------------------------------------------------------------------------*/
  #if 1
  if((HT_FLASH->VMCR & 0x1) == 0x1)   /* The value of BOOT0 will be sampled to the VMCR                     */
                                      /* register of FMC after reset.                                       */
  #else
  /*--------------------------------------------------------------------------------------------------------*/
  /* Example that using Key1 to decide start user application or IAP mode.                                  */
  /* Key1 = Release: User application, Key1 = Pressed: IAP mode,                                            */
  /*--------------------------------------------------------------------------------------------------------*/
  CKCU_APBPerip0ClockConfig(DVB_GpioClock[KEY1_BUTTON_GPIO_ID], ENABLE);
  GPIO_DirectionConfig(KEY1_BUTTON_GPIO_PORT, KEY1_BUTTON_GPIO_PIN, GPIO_DIR_IN);
  GPIO_InputConfig(KEY1_BUTTON_GPIO_PORT, KEY1_BUTTON_GPIO_PIN, ENABLE);
  if (GPIO_ReadInBit(KEY1_BUTTON_GPIO_PORT , KEY1_BUTTON_GPIO_PIN))
  #endif
  {
    #if (IAP_TIMEOUT_EN == 1)
    {
      if (HT_FLASH->SBVT[1] == BOOT_MODE_APP)
      {
        if ((IAP_isAPValid() == TRUE) && (IAP_isVersionValid() == TRUE) )
        {
          /*--------------------------------------------------------------------------------------------------*/
          /* Start user application when                                                                      */
          /*   1. IO = 1 and                                                                                  */
          /*   2. SBVT1 == BOOT_MODE_APP (Default IAP) and                                                    */
          /*   3. SP and PC of user's application is in range                                                 */
          /*--------------------------------------------------------------------------------------------------*/
          IAP_GoCMD(IAP_APFALSH_START);
        }
      }
      else if (gIAPTimeout != IAP_TIMEOUT_OFF && HT_FLASH->SBVT[1] != BOOT_MODE_IAP)
      {
        SYSTICK_ClockSourceConfig(SYSTICK_SRC_FCLK);
        SYSTICK_SetReloadValue(TIMER_BASE);
        SYSTICK_IntConfig(ENABLE);
        SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
      }
    }
    #else
    {
      WDT_RELOAD();      
      if (HT_FLASH->SBVT[1] != BOOT_MODE_IAP)
      {
        if ((IAP_isAPValid() == TRUE) && (IAP_isVersionValid() == TRUE) )      
				{
          /*--------------------------------------------------------------------------------------------------*/
          /* Start user application when                                                                      */
          /*   1. IO = 1 and                                                                                  */
          /*   2. SBVT1 != BOOT_MODE_IAP (Default AP) and                                                     */
          /*   3. SP and PC of user's application is in range                                                 */
          /*--------------------------------------------------------------------------------------------------*/
          //RSTCU_APBPerip1Reset(RSTCU_APBRST1_WDT, ENABLE);
          IAP_GoCMD(IAP_APFALSH_START);
        }
      }
    }
    #endif
  }
  HT_FLASH->SBVT[1] = BOOT_MODE_APP;

  /* Initialize SPI Flase Driver */
  SPI_FLASH_Init();   
  
  /*--------------------------------------------------------------------------------------------------------*/
  /* Start IAP mode                                                                                         */
  /*   1. IO = 0 or                                                                                         */
  /*   2. SBVT1 = BOOT_MODE_IAP                                                                             */
  /*--------------------------------------------------------------------------------------------------------*/
  IAP_Init();

  USBD_Configuration();               /* USB device configuration                                           */

  while (1)
  {
    USBDCore_MainRoutine(&gUSBCore);
    WDT_RELOAD();  
  }
}
void RSTCU_APBPerip1Reset(u32 RSTCU_APBP, ControlStatus Cmd)
{
  /* Check the parameters */
  Assert_Param(IS_RSTCU_APBRST1(RSTCU_APBP));
  Assert_Param(IS_CONTROL_STATUS(Cmd));

  if (Cmd == ENABLE)
  {
    HT_RSTCU->APBPRST1 |= RSTCU_APBP;
  }
  else
  {
    HT_RSTCU->APBPRST1 &= ~RSTCU_APBP;
  }
}
/*********************************************************************************************************//**
  * @brief  Suspend call back function which enter DeepSleep1
  * @param  uPara: Parameter for Call back function
  * @retval None
  ***********************************************************************************************************/
#if 0
void Suspend(u32 uPara)
{
  while (1)
  {
    __DBG_USBPrintf("%06ld >DEEPSLEEP\n\r", ++__DBG_USBCount);

    PWRCU_DeepSleep1(PWRCU_SLEEP_ENTRY_WFE);

    __DBG_USBPrintf("%06ld <DEEPSLEEP\n\r", ++__DBG_USBCount);

    if (USBDCore_IsSuspend(&gUSBCore) == FALSE)
    {
      break;
    }
  }
  return;
}
#endif

/*********************************************************************************************************//**
  * @brief  Configures the system clocks.
  * @retval None
  ***********************************************************************************************************/
void CKCU_Configuration(void)
{
	#if 1
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.PDMA       = 1;
  CKCUClock.Bit.USBD       = 1;
  CKCUClock.Bit.CKREF      = 0;
  CKCUClock.Bit.EBI        = 0;
  CKCUClock.Bit.CRC        = 0;
  CKCUClock.Bit.PA         = 0;
  CKCUClock.Bit.PB         = 0;
  CKCUClock.Bit.PC         = 0;
  CKCUClock.Bit.PD         = 0;
  CKCUClock.Bit.REV0       = 0;
  CKCUClock.Bit.I2C0       = 0;
  CKCUClock.Bit.I2C1       = 0;
  CKCUClock.Bit.SPI0       = 0;
  CKCUClock.Bit.SPI1       = 0;
  CKCUClock.Bit.USART0     = 0;
  CKCUClock.Bit.USART1     = 0;
  CKCUClock.Bit.UART0      = 0;
  CKCUClock.Bit.UART1      = 0;
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.EXTI       = 0;
  CKCUClock.Bit.SCI        = 0;
  CKCUClock.Bit.I2S        = 0;
  CKCUClock.Bit.MCTM0      = 0;
  CKCUClock.Bit.MCTM1      = 0;
  CKCUClock.Bit.WDT        = 1;
  CKCUClock.Bit.BKP        = 0;
  CKCUClock.Bit.GPTM0      = 0;
  CKCUClock.Bit.GPTM1      = 0;
  CKCUClock.Bit.BFTM0      = 0;
  CKCUClock.Bit.BFTM1      = 0;
  CKCUClock.Bit.CMP        = 0;
  CKCUClock.Bit.REV1       = 0;
  CKCUClock.Bit.ADC        = 0;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
#endif
#if 0
  CKCU_AHBPeripClockConfig(CKCU_AHBEN_USB, ENABLE);
  
  /*--------------------------------------------------------------------------------------------------------*/
  /* APB Peripheral 0 Enable                                                                                */
  /* - CKCU_APBEN0_I2C0,   CKCU_APBEN0_I2C1, CKCU_APBEN0_SPI0, CKCU_APBEN0_SPI1, CKCU_APBEN0_USART0         */
  /* - CKCU_APBEN0_USART1, CKCU_APBEN0_AFIO, CKCU_APBEN0_EXTI, CKCU_APBEN0_PA,   CKCU_APBEN0_PB             */
  /* - CKCU_APBEN0_PC,     CKCU_APBEN0_PD,   CKCU_APBEN0_PE,   CKCU_APBEN0_SCI                              */
  /*--------------------------------------------------------------------------------------------------------*/
  CKCU_APBPerip0ClockConfig(CKCU_APBEN0_AFIO, ENABLE);

  /*--------------------------------------------------------------------------------------------------------*/
  /* APB Peripheral 1 Enable                                                                                */
  /* - CKCU_APBEN1_MCTM, CKCU_APBEN1_WDT,   CKCU_APBEN1_RTC,   CKCU_APBEN1_GPTM0, CKCU_APBEN1_GPTM1         */
  /* - CKCU_APBEN1_USB,  CKCU_APBEN1_BFTM0, CKCU_APBEN1_BFTM1, CKCU_APBEN1_OPA0,  CKCU_APBEN1_OPA1          */
  /* - CKCU_APBEN1_ADC                                                                                      */
  /*--------------------------------------------------------------------------------------------------------*/
  CKCU_APBPerip1ClockConfig(CKCU_APBEN1_RTC, ENABLE);
#endif
  /*--------------------------------------------------------------------------------------------------------*/
  /* Set USB Clock as PLL / 3                                                                               */
  /*--------------------------------------------------------------------------------------------------------*/
  CKCU_SetUSBPrescaler(CKCU_USBPRE_DIV3);
}

/*********************************************************************************************************//**
  * @brief  Configures the USB device.
  * @retval None
  ***********************************************************************************************************/
void USBD_Configuration(void)
{
  gUSBCore.pDriver = (u32 *)&gUSBDriver;
  //gUSBCore.Power.CallBack_Suspend.func  = Suspend;
  //gUSBCore.Power.CallBack_Suspend.uPara = (u32)NULL;

  USBDDesc_Init(&gUSBCore.Device.Desc);
  USBDClass_Init(&gUSBCore.Class);
  USBDCore_Init(&gUSBCore);

  NVIC_EnableIRQ(USB_IRQn);

  HT32F_DVB_USBConnect();
}

/*********************************************************************************************************//**
  * @brief  Configures the Watchdog Timer in startup_ht32f175x_275x.s.
  * @retval None
  ***********************************************************************************************************/
void WDT_Configuration(void)
{
#if (WDT_EN != 0)  
	WDT_ResetCmd(ENABLE);
  /*----------------------------- WatchDog configuration ---------------------------------------------------*/
//  WDT_IntConfig(ENABLE);             // Enable WDT Interrupt
  WDT_SetPrescaler(WDT_PRESCALER_64); // Set Prescaler Value as 2
  WDT_SetReloadValue(0xEFF);         // Set Reload Value as 0xEFF
  WDT_SetDeltaValue(0xEFF);          // Set Delta Value as 0xA00
  WDT_Cmd(ENABLE);

  WDT_Restart();                     // Reload Counter as WDTV Value
#endif  
}

#if (HT32_LIB_DEBUG == 1)
/*********************************************************************************************************//**
  * @brief  Reports both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
  */

  while (1)
  {
  }
}
#endif
