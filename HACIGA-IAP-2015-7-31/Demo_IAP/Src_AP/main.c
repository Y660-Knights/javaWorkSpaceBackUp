/*********************************************************************************************************//**
 * @file    Src_AP/main.c
 * @version V1.00
 * @date    6/10/2013
 * @brief   The main program.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
#include "main.h"
#include "cigarette.h"
#include "math.h"
#include "PID.h"
/* Private macro -------------------------------------------------------------------------------------------*/
/* Global variables ----------------------------------------------------------------------------------------*/
__ALIGN4 USBDCore_TypeDef gUSBCore;
USBD_Driver_TypeDef gUSBDriver;

Sleep_off_typedef Auto_off;
bool K_Count=FALSE;
bool Open_State=FALSE;//开机状态
bool Enter_menu=FALSE;//进入菜单
bool PUSH_PWM=FALSE;//升压
bool Enter_output=FALSE;//进入调输出模式
bool Enter_Ajustment;//进入调试模式
bool TEST_R=FALSE;//测试电阻
bool SCP_STATE=FALSE;//短路
bool PC_STATE=FALSE;//USB连接
bool Exit_PC=FALSE;//退出USB连接
bool EN_Charge=FALSE;//充电
u8 Key1_count=0;
//ADC电压值
extern u16 UV0;
extern u16 Uout;
extern u16 UV1;
extern u16 OUT2V;
extern u16 OUT1V;
u16 AD_R=0;
extern u16 ZKB_L;//pwm1低频占空比
extern uint32_t ZKB;//pwm2高频占空比
u8 TEST_R_T=0;//测试电阻的次数
vu32 gTimebaseDelayCounter=0;

u16 P_r=1;//电阻值
u16 P_r_temp=0;//电阻缓存值
u16 P_W=0;//输出功率0w
bool P_open=FALSE;//功率开启状态
extern u32 gADC_Result[6];//ADC转换寄存器
extern volatile bool gADC_CycleEndOfConversion;//ADC转换状态
bool Hints=FALSE;
/* Private variables ---------------------------------------------------------------------------------------*/

/* Global functions ----------------------------------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------------------------------------*/
void WDT_Configuration(void);
void Hint_handle(void);
u8 t=1;
float	V=0.0;
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{

#if (HT32_LIB_DEBUG == 1)
  debug();
#endif
//	begin:
	CKCU_Configuration();//时钟配置
	WDT_Configuration();
	GPIO_Configuration();//IO口
  NVIC_Configuration();
	SYSTICK_Configuration();//系统时钟
	
	ADC_Config();
  AP_Init();
  USBD_Configuration();
	GPIO_ClearOutBits(HT_GPIOA, AFIO_PIN_10);//LCDE
	GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_8);//检测电压除能
//  EXTI_Configuration();
//	EXTI_WakeupEventConfig(WAKEUP_BUTTON_EXTI_CHANNEL, EXTI_WAKEUP_LOW_LEVEL, ENABLE);
	ssd1306();
	IN_OUT_GPIO();//按键IO
	MCTM_Config();//PWM1、PWM2
	SYSTICK_IntConfig(ENABLE); //OPEN TIME BASE; 
//	OLED_Display_Off();
  USB_CHARG_Int();//USB连接或充电
	SCP_Int();//短路初始化定义
	PID_init();
	/* Configure button EXTI Channel5 on low level                                                           */
  
  while(1)
  {	 
//		V=PID_realize(180);
//		OLED_ShowNum(50,48,V,3,16);
//		OLED_Refresh_Gram();
		/* Request to enter Deep Sleep mode 1 (The LDO in low power mode)                                       */
//		EXTI_WakeupEventConfig(WAKEUP_BUTTON_EXTI_CHANNEL, EXTI_WAKEUP_LOW_LEVEL, ENABLE);
//		PWRCU_DeepSleep1(PWRCU_SLEEP_ENTRY_WFI);
		USBDCore_MainRoutine(&gUSBCore); 
		if(TEST_R)
		{
			R_Check();
		}
		else 
		{
			key_contrl();
			W_handle();//功率调试处理
		}	
		SCP_Handle();//短路处理	
		if(Open_State)
		{
			Sleep_handle();//sleep or turn off
			Hint_handle();//警告提示
		}	
		else
			USB_CHARG_Handle();//USB充电或连接处理
		WDT_RELOAD();		
		
  }
}

void Hint_handle(void)
{
	if(P_r>300&&Hints==FALSE)//3ohms
			{
				Hints=TRUE;
				OLED_ShowString(12,42,"Ohms too High");//雾化器阻值过大
				OLED_Refresh_Gram();
			}
			else if(P_r<30&&Hints==FALSE)//0.3ohms
			{
				Hints=TRUE;
				OLED_ShowString(12,42,"Ohms too Low");//雾化器阻值过小		
				OLED_Refresh_Gram();				
			}
			if(UV1<320&&Hints==FALSE)
			{
				Hints=TRUE;
				OLED_ShowString(12,42,"Battery Low");//电池电量过低	
				OLED_Refresh_Gram();	
			}
			else if(UV1>430&&Hints==FALSE)
			{
				Hints=TRUE;
				OLED_ShowString(12,42,"Battery High");//电池电量过高
				OLED_Refresh_Gram();	
			}
#if 0//尚未加入的提示
	OLED_ShowString(32,48,"Check Atomizer");//检查雾化器

	OLED_ShowString(32,48,"Mech mode");//机械模式
#endif
}

void Sleep_handle(void)
{
	if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0||GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0
		||GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
	{	
		Auto_off.key_free=FALSE;Auto_off.sleep=FALSE;
		Hints=FALSE;OLED_Display_On();
	}
	else
		Auto_off.key_free=TRUE;
	if(Auto_off.sleep_time>30000)//30s
	{
		Auto_off.sleep_time=0;
		Auto_off.sleep=TRUE;
	}
	if(Auto_off.sleep==TRUE)
	{
		OLED_Display_Off();
		if(Auto_off.off_time>600000)//30s
		{
			Auto_off.off_time=0;
			Auto_off.turn_off=TRUE;
			//关机
			Open_State=FALSE;
			PUSH_PWM=FALSE;
			P_W=0;
			Uout=0;
			ZKB_L=500;
			ZKB=1000;
			GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_5);//ENH
			GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_7);//EN12V
			OLED_Display_Off();
		  GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_8);//PB0拉高
		}
	}
	else
	{
		OLED_Display_On();
	}
}
void W_handle(void)
{
	if(P_open&&Auto_off.sleep==FALSE)
				{
					BFTM_EnaCmd(HT_BFTM0, ENABLE);
					GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_9);//不做电阻检测
					if(Uout>=UV1)
						{
							PUSH_PWM=TRUE;
						}
				if(Uout<UV1)
						{
							PUSH_PWM=FALSE;
						}
					Uout=sqrt(P_W*P_r);//应该输出的电压
				
				}
	else
	{
		ZKB_L=500;//Q9管常闭，没电压输出
		ZKB=1000;//Q5管常通，Q4管截止，不做升压操作
		PUSH_PWM=FALSE;
		Uout=0;
		EXTI_IntConfig(EXTI_CHANNEL_14, DISABLE);//关短路中断
	}
//	P_W=Uout*Uout/P_r;
	
	if (gADC_CycleEndOfConversion)
    {
			UV1=((gADC_Result[2]*300)/4095)*2;//电池电压
			AD_R=(gADC_Result[4]*300)/4095;//短路电流对应的电压
			
			//OUT1V=(gADC_Result[3]*310)/4095*4;
			UV0=((gADC_Result[1]*300)/4095)*4;//升压后电压，本应该乘3，后来陈工调完后得乘4才能比较接近实际值，参照电路调整
//			if(Uout<=390)
//				OUT1V=UV0*(500-ZKB_L)/500+20;
//			else
				OUT1V=UV0*(500-ZKB_L)/500+10;
      gADC_CycleEndOfConversion = FALSE;
    }
		#if 0//关键电压显示，调试显示用
			OLED_ShowString(0,32,"UV1:");
			OLED_ShowNum(30,32,UV1,3,16);
			OLED_ShowString(60,32,"OUT1:");
			OLED_ShowNum(100,32,OUT1V,3,16);
			OLED_ShowString(0,48,"UV0:");
			OLED_ShowNum(30,48,UV0,3,16);
//			OLED_ShowString(60,48,"Uout:");
//			OLED_ShowNum(100,48,Uout,3,16);
			OLED_ShowString(60,48,"P_r:");
			OLED_ShowNum(100,48,P_r,3,16);
			OLED_Refresh_Gram();
		#endif
}
void SCP_Int(void)
{	
	EXTI_InitTypeDef EXTI_InitStruct;
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
  /* Enable EXTI & PE APB clock                                                                             */
  CKCUClock.Bit.PA         = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.EXTI       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
 
  /* Enable GPIO Input Function                                                                             */
  GPIO_InputConfig(HT_GPIOA, GPIO_PIN_14, ENABLE);

  /* Select Port as EXTI Trigger Source                                                                     */
  AFIO_EXTISourceConfig(AFIO_EXTI_CH_14, AFIO_ESS_PA);
		EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_14;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_ENABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0xFF;                   
    EXTI_InitStruct.EXTI_IntType = EXTI_HIGH_LEVEL;
//    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
    EXTI_Init(&EXTI_InitStruct);  
  
	EXTI_ClearEdgeFlag(EXTI_CHANNEL_14);
  /* Enable EXTI line 14 Interrupt                                                                          */
  EXTI_IntConfig(EXTI_CHANNEL_14, DISABLE);
/* Disable EXTI_CHANNEL_14 NVIC Interrupt Channel                                                          */
  NVIC_EnableIRQ(EXTI14_IRQn);
//  /* Activate the corresponding EXTI interrupt                                                              */
//  EXTI_SWIntCmd(EXTI_CHANNEL_14, ENABLE);
}
void SCP_Handle(void)
{
	if(AD_R>200||SCP_STATE)//中断产生或者通过检测0 .002R的电流大于20A
	{
		SCP_STATE=FALSE;
		//短路
		P_open=FALSE;	
		//PWM2
		PUSH_PWM=FALSE;
		ZKB_L=500;
		ZKB=1000;
		P_W=0;
		GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_5);//ENH
		GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_7);//EN12V
		//	GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_0);//PWM1
		BFTM_EnaCmd(HT_BFTM0, ENABLE);
		/* BUZZER MCTM Channel Main Output enable                                                                 */
		MCTM_CHMOECmd(BUZZER_TM, ENABLE);
		if(Hints==FALSE)
		{
		OLED_ShowString(12,42,"Atomizer Short");//雾化器短路
		OLED_Refresh_Gram();
		}
//		NVIC_SystemReset();//重启
	}
	
}
void USB_CHARG_Int(void)
{
		/* Configure DONE CHRG pins as the input function                                                */

  /* Configure AFIO mode of input pins                                                                      */
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_10, AFIO_MODE_1);//CHRG
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_11, AFIO_MODE_1);//DONE

  /* Configure GPIO direction of input pins                                                                 */
  GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_10, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_11, GPIO_DIR_IN);

  /* Configure GPIO pull resistor of input pins                                                             */
  GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_10, GPIO_PR_DOWN);
  GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_11, GPIO_PR_UP);

  GPIO_InputConfig(HT_GPIOB, GPIO_PIN_10, ENABLE);
  GPIO_InputConfig(HT_GPIOB, GPIO_PIN_11, ENABLE);
}  
void USB_CHARG_Handle(void)
{
	if(GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_10)==1)
	{
		
		GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_10);//清零
		Exit_PC=FALSE;
		EN_Charge=TRUE;
		OLED_Clear();
			if(GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_11)==0)//充满
			{
				
			OLED_ShowPic(0,32,15,1);
			OLED_Refresh_Gram();
			}
			else
				bat_display();
	if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0||GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0
		||GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
		{	
			Auto_off.key_free=FALSE;Auto_off.sleep=FALSE;OLED_Display_On();
		}
	else
		Auto_off.key_free=TRUE;
	if(Auto_off.sleep_time>30000)//30s
		{
			Auto_off.sleep_time=0;
			Auto_off.sleep=TRUE;
		}
	if(Auto_off.sleep==TRUE)
		{
			OLED_Display_Off();
		}
	else
		{
			OLED_Display_On();
		}		
	}

	else 
		{
			if(!Exit_PC)
			{
				Exit_PC=TRUE;OLED_Clear();

			}		
		}
}
void R_Check(void)
{
		PUSH_PWM=FALSE;
		ZKB_L=500;
		ZKB=1000;
		BFTM_EnaCmd(HT_BFTM0, ENABLE);
		//GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_0);//PWM1
		GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_9);//TEST
		GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_8);//测电压
	if (gADC_CycleEndOfConversion)
    {
			UV1=((gADC_Result[2]*300)/4095)*2;
			//OUT1V=(gADC_Result[3]*310)/4095*4;
			UV0=((gADC_Result[1]*300)/4095)*4;
//			if(Uout<=400)
//				OUT1V=UV0*(500-ZKB_L)/500+20;
//			else
				OUT1V=UV0*(500-ZKB_L)/500;
			OUT2V=(gADC_Result[0]*300)/4095;
			
			TEST_R_T++;
//			OLED_ShowString(60,48,"P_r:");
//			OLED_ShowNum(100,48,P_r,3,16);
//			OLED_Refresh_Gram();
			if(TEST_R_T>3)
			{
				P_r_temp=OUT2V*10*100/(UV1-OUT2V);
				P_r=P_r_temp;
				TEST_R_T=0;
				TEST_R=FALSE;
				GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_9);//R除能
			}
			gADC_CycleEndOfConversion = FALSE;
//			UV1=sqrt(P_W*P_r);
		}
}
#if 0
u8 Press_3_valid;
void Three_Press_detect(void)
{
	static u8 PressValidCnt,PressTimes,isPress;
	static u16 PressReleaseCnt;
	if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
	{
		if(PressValidCnt++>30)
		{
			if((isPress==1)&&(PressReleaseCnt<700))
			{
				PressReleaseCnt=0;
				PressTimes++;
				if(PressTimes==2)
				{
					PressTimes=0;
					Press_3_valid=1;
				}
			}
			isPress=1;			
		}
	}
	else
	{
		PressValidCnt=0;
		if(isPress)
		{
			if(PressReleaseCnt++>700)
			{
				PressReleaseCnt=0;
				PressTimes=0;
				isPress=0;
			}		
		}
	}
}
#endif


void key_contrl(void)
{
	if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
	{	delay(1000);
		if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
			K_Count=TRUE;
			if(gTimebaseDelayCounter>350)
			{
				while(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0&&gTimebaseDelayCounter<3000);
				K_Count=FALSE;
				gTimebaseDelayCounter=0;
				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
				{
					OLED_Display_On();
					open_pic();
					if(Open_State==FALSE)
					{
						
						Open_State=TRUE;
						TEST_R=TRUE;
						GPIO_SetOutBits(HT_GPIOA, AFIO_PIN_10);//LCDE
						GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_8);//PB0拉低測電壓
					}
					else
					{
						Open_State=FALSE;
							PUSH_PWM=FALSE;
							P_W=0;
							Uout=0;
							ZKB_L=500;
							ZKB=1000;
							GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_5);//ENH
							GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_7);//EN12V
							OLED_Display_Off();
						  GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_8);//PB0拉高
							GPIO_ClearOutBits(HT_GPIOA, AFIO_PIN_10);//LCDE release
					}
				}
		}
	}
	else
	{
		K_Count=FALSE;
		gTimebaseDelayCounter=0;
	}
	if(Open_State==TRUE)
	{
		
		if(Enter_menu==TRUE)
		{
			main_menu();
		}
		else if(Enter_output==TRUE)
		{
			output_menu();
		}
		else if(Enter_Ajustment==TRUE)
		{
			Ajustment_menu();
		}
		else
		{
			V_mode();
//			bat_display();
		}
	}
}


void IN_OUT_GPIO(void)
{
	/* Configure WEAKUP, KEY1, KEY2 pins as the input function                                                */

  /* Configure AFIO mode of input pins                                                                      */
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_15, AFIO_MODE_1);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_13, AFIO_MODE_1);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_14, AFIO_MODE_1);

  /* Configure GPIO direction of input pins                                                                 */
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_15, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_13, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_14, GPIO_DIR_IN);

  /* Configure GPIO pull resistor of input pins                                                             */
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_15, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_13, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_14, GPIO_PR_UP);

  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_15, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_13, ENABLE);
  GPIO_InputConfig(HT_GPIOC, GPIO_PIN_14, ENABLE);
	#if 0
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.PC         = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
#endif
	
}
#if 0
void EXTI_Configuration(void)
{
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_15, AFIO_MODE_1);

  /* Connect Button EXTI Channel to Button GPIO Pin                                                         */
  AFIO_EXTISourceConfig((AFIO_EXTI_CH_Enum)WAKEUP_BUTTON_EXTI_CHANNEL, (AFIO_ESS_Enum)WAKEUP_BUTTON_GPIO_ID);

  /* Enable and set EXTI Event Wakeup interrupt to the lowest priority                                      */
  NVIC_SetPriority(EVWUP_IRQn, 0xF);
  NVIC_EnableIRQ(EVWUP_IRQn);
  EXTI_WakeupEventIntConfig(ENABLE);
}
#endif
void SYSTICK_Configuration(void)
{
  /* SYSTICK configuration */
  SYSTICK_ClockSourceConfig(SYSTICK_SRC_SEL);
  SYSTICK_SetReloadValue(SYSTICK_RELOAD_SET);                  
  SYSTICK_IntConfig(DISABLE);

  /* Enable the SysTick Counter                                                                             */
  SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
}
void GPIO_Configuration(void)
{
	/*OLED屏引腳配置*/
	/* Configure AFIO mode of input pins                                                                      */
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_1, AFIO_MODE_DEFAULT);//scl
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_2, AFIO_MODE_DEFAULT);//sda
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_1, AFIO_MODE_DEFAULT);//dc
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_3, AFIO_MODE_DEFAULT);//cs
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_0, AFIO_MODE_DEFAULT);//pwm1
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_4, AFIO_MODE_DEFAULT);//CS備用
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_10, AFIO_MODE_DEFAULT);//LCDE
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_11, AFIO_MODE_1);//RES
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_MODE_DEFAULT);//RES備用
  /* Configure GPIO direction of input pins                                                                 */
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_1, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_2, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_1, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_3, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_4, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_0, GPIO_DIR_OUT);//PWM1
	GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_0);//PWM1
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_10, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_11, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_OUT);
	//PB
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_8, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_8, GPIO_DIR_OUT);
	//EN12V
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_7, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_7, GPIO_DIR_OUT);
	//ENH
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_5, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_5, GPIO_DIR_OUT);
//	//短路腳
//	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_14, AFIO_MODE_DEFAULT);
//	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_14, GPIO_DIR_OUT);
//	GPIO_ClearOutBits(HT_GPIOA, AFIO_PIN_14);//常態是低電平
	//HO高电平
//	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_2, AFIO_MODE_DEFAULT);
//	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_2, GPIO_DIR_OUT);
//	GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_2);//HO
	//r_check  test
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_9, AFIO_MODE_DEFAULT);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_9, GPIO_DIR_OUT);
	GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_9);//R
}
/*********************************************************************************************************//**
  * @brief  Configures the NVIC priority.
  * @retval None
  ***********************************************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, IAP_APFALSH_START);     /* Set the Vector Table Offset          */

#if 0
  #define NVIC_PRIORITY_GROUP_3          3 /* Preemption: 4 bits / Subpriority: 0 bits */
  #define NVIC_PRIORITY_GROUP_4          4 /* Preemption: 3 bits / Subpriority: 1 bits */
  #define NVIC_PRIORITY_GROUP_5          5 /* Preemption: 2 bits / Subpriority: 2 bits */
  #define NVIC_PRIORITY_GROUP_6          6 /* Preemption: 1 bits / Subpriority: 3 bits */
  #define NVIC_PRIORITY_GROUP_7          7 /* Preemption: 0 bits / Subpriority: 4 bits */

  /* Configure 4 bits for preemption priority 0 bits for subpriority */
  NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP_3);
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_3, 0, 0));
  NVIC_SetPriority(USB_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_3, 0, 0));
#endif  
}
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
  CKCUClock.Bit.PA         = 1;
  CKCUClock.Bit.PB         = 1;
  CKCUClock.Bit.PC         = 1;
  CKCUClock.Bit.PD         = 1;
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
  CKCUClock.Bit.EXTI       = 1;
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
