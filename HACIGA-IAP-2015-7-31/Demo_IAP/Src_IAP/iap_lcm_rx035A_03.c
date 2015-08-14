 /*********************************************************************************************************//**
 * @file    ebi_lcd.c
 * @version V1.0
 * @date    03/12/2012
 * @brief   This file provides a set of functions needed to manage the
 *          communication between EBI peripheral and LCD HX8347-D.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "lcm.h"
#include "lcm_rx035A_03.h"
#include "iap_font.h"

/* Private typedef -----------------------------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------------------------------------*/
#define LCM_EXCHANGE_X_Y  1

#if (LCM_DIRECTION == LCM_NORMAL)
  // ---> Column 480 (MY=0 MX=0 MV=0)
  // | 
  // V
  // Page 800
  #define EC (480)
  #define EP (800)
  #define MADCTR (0x00)
#elif (LCM_DIRECTION == LCM_LEFT_90)
  // Column 800
  // ^
  // |  
  // |--> Page 480   (MY=1 MX=0 MV=1)
  #define EC (800)
  #define EP (480)
  #define MADCTR (0xA0)
#elif (LCM_DIRECTION == LCM_RIGHT_90)
  //   Page 480  <--| (MY=0 MX=1 MV=1)
  //                |
  //                V
  //           Column 800
  #define EC (800)
  #define EP (480)
  #define MADCTR (0x60)
#elif (LCM_DIRECTION == LCM_ROTATE_180)
  //              Page 800
  //                ^ 
  //                |
  //  Column 480 <--- (MY=1 MX=1 MV=0)
  #define EC (480)
  #define EP (800)
  #define MADCTR (0xC0)
#elif (LCM_DIRECTION == LCM_ROTATE_180_INV)
  // Page 800
  //   ^ 
  //   |
  //   ---> Column 480  (MY=1 MX=0 MV=0)
  #define EC (480)
  #define EP (800)
  #define MADCTR (0x80)
#endif

#define	HDP					(u16)479			// Horizontal Display Period
#define	HT					(u16)1000			// Horizontal Total
#define	HPS					(u16)51				// LLINE Pulse Start Position
#define	LPS					(u16)3				// Horizontal Display Period Start Position
#define	HPW					(u16)8				// LLINE Pulse Width
#define	VDP					(u16)479			// Vertical Display Period
#define	VT					(u16)530			// Vertical Total
#define	VPS					(u16)24				// LFRAME Pulse Start Position
#define	FPS					(u16)23				// Vertical Display Period Start Positio
#define	VPW					(u16)3				// LFRAME Pulse Width

#define delay_ms(nTime)     BOARD_DelayXuS((nTime*1000))


/* Private macro -------------------------------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------------------------------------*/

static vu16 Color_Text = 0x0000;  // default: Black
static vu16 Color_Back = 0xFFFF;  // default: White

EBI_InitTypeDef  EBI_InitStructure;

u16 SyncLine = 0;

/* Private function prototypes -----------------------------------------------------------------------------*/

//static void delay_ms(u32 ms);

/* Private functions ---------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
  * @brief  LCM_CMD
  * @retval None
  ***********************************************************************************************************/
void LCM_CMD(u16 cmd)
{
  EBI_LCD->EBI_LCD_REG = cmd;
}

/*********************************************************************************************************//**
  * @brief  LCM_WRITE_DATA
  * @retval None
  ***********************************************************************************************************/
void LCM_WRITE_DATA(u16 data)
{
  EBI_LCD->EBI_LCD_RAM = data;
}

/*********************************************************************************************************//**
  * @brief  LCM_READ_DATA
  * @retval None
  ***********************************************************************************************************/
u16 LCM_READ_DATA(void)
{
  return EBI_LCD->EBI_LCD_RAM;
}

/*********************************************************************************************************//**
  * @brief  Configures the EBI and GPIOs to interface with the SRAM memory.
  * @retval None
  ***********************************************************************************************************/
void LCD_Init(void)
{

  /*  Enable AFIO clock                                                                                     */
  CKCU_APBPerip0ClockConfig(CKCU_APBEN0_AFIO, ENABLE);
  /*  Enable EBI clock                                                                                      */
  CKCU_AHBPeripClockConfig(CKCU_AHBEN_EBI | GPIO_PCLK(LCM_RST_GPIO_PORT_ID) | CKCU_AHBEN_PB, ENABLE);
 
  CKCU_AHBPeripClockConfig(GPIO_PCLK(GPIOD_ID), ENABLE);
  GPIO_DirectionConfig(GPIO_PORT(GPIOD_ID), GPIO_PIN(15), GPIO_DIR_OUT);
  HT_GPIOD->SRR = GPIO_PIN_15;

  
  /* Configure EBI pins & LCD RESET pin                                                                     */
  BOARD_AFIO_PinConfig(EBI_OE_GPIO_PORT_ID, EBI_OE_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_WE_GPIO_PORT_ID, EBI_WE_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_A0_GPIO_PORT_ID, EBI_A0_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD0_GPIO_PORT_ID, EBI_AD0_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD1_GPIO_PORT_ID, EBI_AD1_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD2_GPIO_PORT_ID, EBI_AD2_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD3_GPIO_PORT_ID, EBI_AD3_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD4_GPIO_PORT_ID, EBI_AD4_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD5_GPIO_PORT_ID, EBI_AD5_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD6_GPIO_PORT_ID, EBI_AD6_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD7_GPIO_PORT_ID, EBI_AD7_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD8_GPIO_PORT_ID, EBI_AD8_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD9_GPIO_PORT_ID, EBI_AD9_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD10_GPIO_PORT_ID, EBI_AD10_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD11_GPIO_PORT_ID, EBI_AD11_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD12_GPIO_PORT_ID, EBI_AD12_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD13_GPIO_PORT_ID, EBI_AD13_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD14_GPIO_PORT_ID, EBI_AD14_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  
  BOARD_AFIO_PinConfig(EBI_AD15_GPIO_PORT_ID, EBI_AD15_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);  

  /*  EBI Configuration                                                                                     */
  #if   (LCM_IF_ID == LCM_IF_EBI_0)
  BOARD_AFIO_PinConfig(EBI_CS0_GPIO_PORT_ID, EBI_CS0_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);
  #elif (LCM_IF_ID == LCM_IF_EBI_1)
  BOARD_AFIO_PinConfig(EBI_CS1_GPIO_PORT_ID, EBI_CS1_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);
  #elif (LCM_IF_ID == LCM_IF_EBI_2)
  BOARD_AFIO_PinConfig(EBI_CS2_GPIO_PORT_ID, EBI_CS2_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);
  #elif (LCM_IF_ID == LCM_IF_EBI_3)
  BOARD_AFIO_PinConfig(EBI_CS3_GPIO_PORT_ID, EBI_CS3_GPIO_PIN_ID, BOARD_AFIO_MODE_ID_EBI);
  #endif

  EBI_InitStructure.EBI_Bank = LCM_EBI_BANK;
  EBI_InitStructure.EBI_Mode = EBI_MODE_D16;
  EBI_InitStructure.EBI_ByteLane = EBI_BYTELANE_DISABLE;
  EBI_InitStructure.EBI_IdleCycle = EBI_IDLECYCLE_DISABLE;
  EBI_InitStructure.EBI_AsynchronousReady = EBI_ASYNCHRONOUSREADY_DISABLE;
  EBI_InitStructure.EBI_ARDYTimeOut = EBI_ARDYTIMEOUT_DISABLE;
  EBI_InitStructure.EBI_ChipSelectPolarity = EBI_CHIPSELECTPOLARITY_LOW;
  EBI_InitStructure.EBI_AddressLatchPolarity = EBI_ADDRESSLATCHPOLARITY_LOW;
  EBI_InitStructure.EBI_WriteEnablePolarity = EBI_WRITEENABLEPOLARITY_LOW;
  EBI_InitStructure.EBI_ReadEnablePolarity = EBI_READENABLEPOLARITY_LOW;
  EBI_InitStructure.EBI_ByteLanePolarity = EBI_BYTELANEPOLARITY_LOW;
  EBI_InitStructure.EBI_ReadySignalPolarity = EBI_READYSIGNALPOLARITY_LOW;
  EBI_InitStructure.EBI_AddressSetupTime = 2;
  EBI_InitStructure.EBI_AddressHoldTime = 1;
  EBI_InitStructure.EBI_WriteSetupTime = 4;
  EBI_InitStructure.EBI_WriteStrobeTime = 2;
  EBI_InitStructure.EBI_WriteHoldTime = 1;
  EBI_InitStructure.EBI_ReadSetupTime = 2;
  EBI_InitStructure.EBI_ReadStrobeTime = 5;
  EBI_InitStructure.EBI_ReadHoldTime = 1;
  EBI_InitStructure.EBI_PageMode = EBI_PAGEMODE_DISABLE;
  EBI_InitStructure.EBI_PageLength = EBI_PAGELENGTH_4;
  EBI_InitStructure.EBI_PageHitMode = EBI_PAGEHITMODE_ADDINC;
  EBI_InitStructure.EBI_PageAccessTime = 0xF;
  EBI_InitStructure.EBI_PageOpenTime = 0xFF;
  EBI_Init(&EBI_InitStructure);

  EBI_Cmd(LCM_EBI_BANK, ENABLE);
  
  LCM_RST_INACTIVE();
  GPIO_DirectionConfig(GPIO_PORT(LCM_RST_GPIO_PORT_ID), GPIO_PIN(LCM_RST_GPIO_PIN_ID), GPIO_DIR_OUT);
  
  CKCU_AHBPeripClockConfig(CKCU_AHBEN_PB, ENABLE);
  GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_7 | GPIO_PIN_8, GPIO_DV_8MA);

  
  /* GT0CH0 PA4 */
  {
    GPTM_TimeBaseInitTypeDef TimeBaseInit;
    GPTM_OutputInitTypeDef OutInit;    
    
    CKCU_APBPerip1ClockConfig(CKCU_APBEN1_GPTM0, ENABLE); 

    BOARD_AFIO_PinConfig(GPIOA_ID, 4, BOARD_AFIO_MODE_ID_TMR); 

    /* Time base configuration */
    TimeBaseInit.CounterMode = GPTM_CNT_MODE_UP;
    TimeBaseInit.CounterReload = 999;
    TimeBaseInit.Prescaler = 71;
    TimeBaseInit.PSCReloadTime = GPTM_PSC_RLD_IMMEDIATE;
    GPTM_TimeBaseInit(HT_GPTM0, &TimeBaseInit);

    OutInit.Channel = GPTM_CH_0;
    OutInit.OutputMode = GPTM_OM_PWM1; 
    OutInit.Control = GPTM_CHCTL_ENABLE;
    OutInit.Polarity = GPTM_CHP_NONINVERTED;
    OutInit.Compare = 0;
    GPTM_OutputInit(HT_GPTM0, &OutInit);
    GPTM_CHCCRPreloadConfig(HT_GPTM0, GPTM_CH_0, ENABLE);

    /* Enable GPTM */
    GPTM_Cmd(HT_GPTM0, ENABLE); 
  }
}

#define HS_FPR    (314572)
#define LS_FPR    (0)

/*********************************************************************************************************//**
  * @brief  LCD_GetScanLine.
  * @retval None
  ***********************************************************************************************************/
u16 LCD_GetScanLine(void)
{
  u16 line;
  
  EBI_LCD->EBI_LCD_REG = 0x45; 
  line = EBI_LCD->EBI_LCD_RAM;                // dumy
  line = (EBI_LCD->EBI_LCD_RAM & 0xFF) << 8;
  line |= EBI_LCD->EBI_LCD_RAM & 0xFF; 
  return line;
}

/*********************************************************************************************************//**
  * @brief  LCD_SetSpeed.
  * @retval None
  ***********************************************************************************************************/
void LCD_SetSpeed(u32 sel)
{
  
  if(sel)
  {
//    LCM_CMD(0xB1);        // Frame Rate Control Normal Full Color
//    LCM_WRITE_DATA(0x90); // D[7:4] FRS[3:0] FrameRate 28 30 32 34 36 39 42 46 50 56 62 70   D[1:0] DIVA[1:0]  fOSC fOSC/2 fOSC/4 fOSC/8
//    LCM_WRITE_DATA(0x11); // 1H (line) period 17 clocks 

  }
  else
  {
    u32 timeout = 100000;
//    while(LCD_GetScanLine() < 470);
    while(timeout--)
    {
      if(LCD_GetScanLine() == SyncLine)
      {
        break;   
      }
    }
    
//    LCM_CMD(0xB1);        // Frame Rate Control Normal Full Color
//    LCM_WRITE_DATA(0x80); // D[7:4] FRS[3:0] FrameRate 28 30 32 34 36 39 42 46 50 56 62 70   D[1:0] DIVA[1:0]  fOSC fOSC/2 fOSC/4 fOSC/8
//    LCM_WRITE_DATA(0x11); // 1H (line) period 31 clocks 
  }
}


/*********************************************************************************************************//**
  * @brief  Configures the EBI and GPIOs to interface with the SRAM memory.
  * @retval None
  ***********************************************************************************************************/
void LCD_Config(void)
{
  /* RESET LCD                                                                                              */
  delay_ms(120);
  LCM_RST_ACTIVE();
  delay_ms(1);
  LCM_RST_INACTIVE();
  delay_ms(120);

	LCM_CMD(0xC0);        // Power Control 1
	LCM_WRITE_DATA(0x05);
	LCM_WRITE_DATA(0x05);

	LCM_CMD(0xC1);        // Power Control 2
	LCM_WRITE_DATA(0x41);

	LCM_CMD(0xC5);        // VCOM Control 1
	LCM_WRITE_DATA(0x00); 
	LCM_WRITE_DATA(0x06);

	LCM_CMD(0xB1);        // Frame Rate Control Normal Full Color
	LCM_WRITE_DATA(0x80); // D[7:4] FRS[3:0] FrameRate 28 30 32 34 36 39 42 46 50 56 62 70   D[1:0] DIVA[1:0]  fOSC fOSC/2 fOSC/4 fOSC/8
	//LCM_WRITE_DATA(0x03); // D[7:4] FRS[3:0] FrameRate 28 30 32 34 36 39 42 46 50 56 62 70   D[1:0] DIVA[1:0]  fOSC fOSC/2 fOSC/4 fOSC/8
	LCM_WRITE_DATA(0x11); // 1H (line) period 17 clocks 

	LCM_CMD(0xB4);        // Display Inversion Control
	LCM_WRITE_DATA(0x02);

	LCM_CMD(0xB7);        // Entry Mode Set
	LCM_WRITE_DATA(0x86); 

	LCM_CMD(0xE0);        // Positive Gamma Control
	LCM_WRITE_DATA(0x0F);
	LCM_WRITE_DATA(0x1B);
	LCM_WRITE_DATA(0x19);
	LCM_WRITE_DATA(0x0C);
	LCM_WRITE_DATA(0x0E);
	LCM_WRITE_DATA(0x07);
	LCM_WRITE_DATA(0x44);
	LCM_WRITE_DATA(0xA8);
	LCM_WRITE_DATA(0x32);
	LCM_WRITE_DATA(0x0A);
	LCM_WRITE_DATA(0x12);
	LCM_WRITE_DATA(0x05);
	LCM_WRITE_DATA(0x0A);
	LCM_WRITE_DATA(0x06);
	LCM_WRITE_DATA(0x00); 

	LCM_CMD(0xE1);        // Negative Gamma Control
	LCM_WRITE_DATA(0x0F);
	LCM_WRITE_DATA(0x39);
	LCM_WRITE_DATA(0x35);
	LCM_WRITE_DATA(0x0A);
	LCM_WRITE_DATA(0x0D);
	LCM_WRITE_DATA(0x05);
	LCM_WRITE_DATA(0x4D);
	LCM_WRITE_DATA(0x75);
	LCM_WRITE_DATA(0x3B);
	LCM_WRITE_DATA(0x08);
	LCM_WRITE_DATA(0x11);
	LCM_WRITE_DATA(0x03);
	LCM_WRITE_DATA(0x26);
	LCM_WRITE_DATA(0x24);
	LCM_WRITE_DATA(0x00); 

	LCM_CMD(0xF2);
	LCM_WRITE_DATA(0x18); 
	LCM_WRITE_DATA(0xA3); 
	LCM_WRITE_DATA(0x12); 
	LCM_WRITE_DATA(0x02); 
	LCM_WRITE_DATA(0xB2); 
	LCM_WRITE_DATA(0x12); 
	LCM_WRITE_DATA(0xFF); 
	LCM_WRITE_DATA(0x10); 
	LCM_WRITE_DATA(0x00);  

	LCM_CMD(0xF1);
	LCM_WRITE_DATA(0x36); 
	LCM_WRITE_DATA(0x04); 
	LCM_WRITE_DATA(0x00); 
	LCM_WRITE_DATA(0x3C); 
	LCM_WRITE_DATA(0x0F); 
	LCM_WRITE_DATA(0x8F); 					   

	LCM_CMD(0xF8);
	LCM_WRITE_DATA(0x21); 
	LCM_WRITE_DATA(0x04); 

	LCM_CMD(0x36);        // Memory Access Control 
	LCM_WRITE_DATA(0xD8); // MY MX MV ML BGR MH X X  

	LCM_CMD(0x3A);        // Interface Pixel Format 
	LCM_WRITE_DATA(0x55); //  RGB Interface Format 16 bits/pixel and CPU Interface Format 16 bits/pixel 

	LCM_CMD(0xF9);
	LCM_WRITE_DATA(0x00); 
	LCM_WRITE_DATA(0x08);  

	LCM_CMD(0x11);        // Sleep Out
	delay_ms(120);

	LCM_CMD(0x29);        // Display ON 
	delay_ms(10);
  
  /* Show Direction */
  LCD_Clear(Black);

  delay_ms(100);
//  LCM_BL_DUTY_SET(LCM_BL_PWM_DEFAULT_DUTY);
  LCM_BL_DUTY_SET(0);

#if 0
  while(1)
  {
//    int i;
//    LCD_SetDisplayArea(10, 20, 50, 50);
//    LCD_WriteRAMPrior();
//    i = 50*50;
//    while(1)
//    {
//      EBI_LCD->EBI_LCD_RAM = 0x1;
//      EBI_LCD->EBI_LCD_RAM = 0x0;
//    }
//    LCD_Clear(0x0400);
//    delay_ms(1000*3);
//    LCD_Clear(0x0200);
//    delay_ms(1000*3);
//    LCD_Clear(0x0100);
//    delay_ms(1000*3);
//    LCD_Clear(0x0080);
//    delay_ms(1000*3);
//    LCD_Clear(0x0400);
//    delay_ms(1000*3);
//    LCD_Clear(0x0200);
//    delay_ms(1000*3);
//    
//    LCD_Clear(0x8000);
//    delay_ms(1000*3);
//    LCD_Clear(0x4000);
//    delay_ms(1000*3);
//    LCD_Clear(0x2000);
//    delay_ms(1000*3);
//    LCD_Clear(0x1000);
//    delay_ms(1000*3);
//    LCD_Clear(0x0800);
//    delay_ms(1000*3);


    
    LCD_Clear(Red);
    delay_ms(1000*3);
    LCD_Clear(Green);
    delay_ms(1000*3);
    LCD_Clear(Blue);
    delay_ms(1000*3);
  }
#endif
}

/*********************************************************************************************************//**
  * @brief  Write value to the selected LCD register.
  * @param  LCD_Reg_Index: address of the selected register.
  * @param  LCD_Reg_Value: value of to write the selected register.
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteReg(u8 LCD_Reg_Index, u16 LCD_Reg_Value)
{
  EBI_LCD->EBI_LCD_REG = LCD_Reg_Index;
  EBI_LCD->EBI_LCD_RAM = LCD_Reg_Value;
}

/*********************************************************************************************************//**
  * @brief  Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteRAMPrior(void)
{
  EBI_LCD->EBI_LCD_REG = 0x2C;
}

/*********************************************************************************************************//**
  * @brief  Writes to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  ***********************************************************************************************************/
void LCD_WriteRAM(u16 RGB_Set)
{
  EBI_LCD->EBI_LCD_RAM = RGB_Set;
}

/*********************************************************************************************************//**
  * @brief  Set the cursor position.
  * @param  X_Location: specify the X position.
  * @param  Y_Location: specify the Y position.
  * @retval None
  ***********************************************************************************************************/
void LCD_StarterSet(u16 X_Location, u16 Y_Location)
{
  EBI_LCD->EBI_LCD_REG = 0x2A;
  EBI_LCD->EBI_LCD_RAM = X_Location >> 8;
  EBI_LCD->EBI_LCD_RAM = X_Location & 0xff;
  EBI_LCD->EBI_LCD_RAM = 319 >> 8;
  EBI_LCD->EBI_LCD_RAM = 319 & 0xff;

  EBI_LCD->EBI_LCD_REG = 0x2B;
  EBI_LCD->EBI_LCD_RAM = Y_Location >> 8;
  EBI_LCD->EBI_LCD_RAM = Y_Location & 0xff;
  EBI_LCD->EBI_LCD_RAM = 479 >> 8;
  EBI_LCD->EBI_LCD_RAM = 479 & 0xff;
}

void LCD_SetDisplayArea(u16 Column, u16 Page, u16 Height, u16 Width)
{
  EBI_LCD->EBI_LCD_REG = 0x2A;
  EBI_LCD->EBI_LCD_RAM = Column >> 8;
  EBI_LCD->EBI_LCD_RAM = Column & 0xff;
  EBI_LCD->EBI_LCD_RAM = (Column+Width-1) >> 8;
  EBI_LCD->EBI_LCD_RAM = (Column+Width-1) & 0xff;

  EBI_LCD->EBI_LCD_REG = 0x2B;
  EBI_LCD->EBI_LCD_RAM = Page >> 8;
  EBI_LCD->EBI_LCD_RAM = Page & 0xff;
  EBI_LCD->EBI_LCD_RAM = (Page+Height-1) >> 8;
  EBI_LCD->EBI_LCD_RAM = (Page+Height-1) & 0xff;
}

void LCD_SetPartialDisplayArea(u16 Start, u16 End)
{
}


void LCD_NormalDisplay(void)
{
}

void LCD_PartialDisplayOn(void)
{
}

/*********************************************************************************************************//**
  * @brief  Clear the whole LCD.
  * @param  Color: the color to be the background.
  * @retval None
  ***********************************************************************************************************/
void LCD_Clear(u16 Color)
{
  u32 i = ((VDP+1) * (HDP+1));
  
  //LCD_SetSpeed(0);
  
  LCD_StarterSet(0, 0);
  
  /* Prepare to write GRAM */
  LCD_WriteRAMPrior();
  
  while(i--)
  {
    EBI_LCD->EBI_LCD_RAM = Color;
    //BOARD_DelayXuS(1000);
  }

  //LCD_SetSpeed(1);
}

/*********************************************************************************************************//**
  * @brief  Set the background color.
  * @param  Color: background color of RGB 5-6-5.
  * @retval None
  ***********************************************************************************************************/
void LCD_BackColorSet(u16 Color)
{
  Color_Back = Color;
}

/*********************************************************************************************************//**
  * @brief  Set the background color.
  * @param  Color: background color of RGB 5-6-5.
  * @retval None
  ***********************************************************************************************************/
u16 LCD_BackColorGet(void)
{
  return Color_Back;
}

/*********************************************************************************************************//**
  * @brief  Set the text color.
  * @param  Color: text color of RGB 5-6-5.
  * @retval None
  ***********************************************************************************************************/
void LCD_TextColorSet(u16 Color)
{
  Color_Text = Color;
}

/*********************************************************************************************************//**
  * @brief  Set the text color.
  * @param  Color: text color of RGB 5-6-5.
  * @retval None
  ***********************************************************************************************************/
u16 LCD_TextColorGet(void)
{
  return Color_Text;
}

/*********************************************************************************************************//**
  * @brief  Draw a point.
  * @param  u16 Xpos,u16 Ypos,u16 Color.
  * @retval None
  ***********************************************************************************************************/
void LCD_DrawPoint(u16 Xpos, u16 Ypos, u16 Color)
{
  EBI_LCD->EBI_LCD_REG = 0x2A;
  EBI_LCD->EBI_LCD_RAM = Xpos >> 8;
  EBI_LCD->EBI_LCD_RAM = Xpos & 0xff;
  EBI_LCD->EBI_LCD_RAM = HDP >> 8;
  EBI_LCD->EBI_LCD_RAM = HDP & 0xff;
  
  EBI_LCD->EBI_LCD_REG = 0x2B;
  EBI_LCD->EBI_LCD_RAM = Ypos >> 8;
  EBI_LCD->EBI_LCD_RAM = Ypos & 0xff;
  EBI_LCD->EBI_LCD_RAM = VDP >> 8;
  EBI_LCD->EBI_LCD_RAM = VDP & 0xff;  

  EBI_LCD->EBI_LCD_REG = 0x2C;
  EBI_LCD->EBI_LCD_RAM = Color;
}  

/*********************************************************************************************************//**
  * @brief  Display the characters on line.
  * @param  u16 Xpos, u16 Ypos,  u16 Height, u16 Width, u8 Mode:0(非叠加方式).
  * @param  uc8 *Sptr: point to the string data.
  * @retval None
  ***********************************************************************************************************/
void LCD_ShowString(u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, uc8 *Sptr)
{         
	while(*Sptr != '\0')
	{       
		if(Xpos > HDP)
		{
			Xpos = 0;
			Ypos += Height;
		}
		if(Ypos > VDP)
		{
			Ypos = Xpos = 0;
		}
    
    LCD_CharDraw(Xpos, Ypos, Height, Width, Mode, *Sptr);
		Xpos += Width;
		Sptr++;
	}  
}

/*********************************************************************************************************//**
  * @brief  Display a character.
  * @param  u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, u8 Ascii
  * @retval None
  ***********************************************************************************************************/
void LCD_CharDraw(u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 Mode, u8 Ascii)
{
	u32 temp=0;
	int x, y;
  u16 i;

//	if(Xpos > HDP || Ypos > VDP)
//		return;

  // Not Need   LCD_SetDisplayArea(Xpos, Ypos, Height, Width);

  i = Height - 1;
	Ascii = Ascii - 32;	//-' ' 得到偏移后的值
	for(y = 0; y < Height; y++)
	{
//		if(Height == 12)
			temp = ASCII_1206[Ascii][i--];				//调用1206字体
//		else if (Height == 16) 
//			temp = ASCII_1608[Ascii][i--]; 				//调用1608字体
//		else if(Height == 24)
//			temp = ASCII_2416[Ascii * 24 + i--];	//调用2416字体
		
		for(x = 0; x < Width; x++)
		{                 
			if(temp & 0x01)
        LCD_DrawPoint(Xpos+x, Ypos+y, Color_Text);
			else
			{
				if(!Mode)//非叠加方式				
          LCD_DrawPoint(Xpos+x, Ypos+y, Color_Back);   
			}					
			temp >>= 1; 
		}
	}	
} 

/*********************************************************************************************************//**
  * @brief  Display one character (16 dots width, 24 dots height).
  * @param  Line_Num: Where to display the character, should be LINE0 ~ LINE9.
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  ***********************************************************************************************************/
/*void LCD_CharDisplay(u8 Line_Num, u16 Column, u8 Ascii)
{
  Ascii -= 32;
  LCD_CharDraw(Line_Num, Column, &ASCII_Font_Table[Ascii * 24]);
}*/

/*********************************************************************************************************//**
  * @brief  Display the maximum 20 characters on one line.
  * @param  Line_Num: Where to display the character, should be LINE0 ~ LINE9.
  * @param  Sptr: point to the string data.
  * @retval None
  ***********************************************************************************************************/
#if 0  // ... 
void LCD_StringLineDisplay(u8 Line_Num, char *Sptr)
{
  u32 i = 0;
  u16 reference_column = 0;

  /* Send character by character on LCD */
  while((*Sptr != 0) & (i < 20))
  {
    LCD_CharDisplay(Line_Num, reference_column, *Sptr);
    /* Increment the column position by 16, because character is size of 16x24 */
    reference_column += 16;
    /* Point to the next character */
    Sptr ++;

    /* Increase the character counter */
    i ++;
  }
}
#endif

/*********************************************************************************************************//**
  * @brief  Draw a picture.
  * @param  Pptr: point to pixel data of picture.
  * @retval None
  ***********************************************************************************************************/
void LCD_PicDraw(u8 X_Location, u16 Y_Location, u8 Height, u16 Width, uc8 *Pptr)
{
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
