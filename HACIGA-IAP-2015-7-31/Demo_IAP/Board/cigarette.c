/*********************************************************************************************************//**
 * @file    cigarette.c
 * @version V1.00
 * @date    24/06/2015
 * @brief   
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2015 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/


/* Includes ------------------------------------------------------------------------------------------------*/
#include "cigarette.h"
#include "ht32.h"
#include "ht32_board.h"
/* Private constants ---------------------------------------------------------------------------------------*/
#define BEE_NBR           4                     /*!< Number of bee after reset                              */
#define BEE_FREQ          100000                  /*!< Frequency of bee                                       */
#define BEE_ACTIVE_TIME   (BEE_FREQ * 5) / 100  /*!< Active 50mS per bee cycle                              */
#define BEE_INACTIVE_TIME (BEE_FREQ * 5) / 100  /*!< Inactive 50mS per bee cycle                            */

/* Private variables ---------------------------------------------------------------------------------------*/
__IO uint32_t wMctmUpdateDownCounter = 0;   /*!< Used to save the numbers of Update Event occurred of MCTM1 */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  MCTM_TimeBaseInitTypeDef MCTM_TimeBaseInitStructure;
  MCTM_OutputInitTypeDef MCTM_OutputInitStructure;
	MCTM_CHBRKCTRInitTypeDef MCTM_CHBRKCTRInitStructure;
  uint32_t wCRR = 0, wPSCR = 0, wBeeIndex = 0;
u16 UV0=350;
u16 Uout=0;
u16 UV1=0;
u16 OUT2V=250;
u16 OUT1V=350;
uint16_t ZKB= 980;uint16_t CNT= 0;
u16 ZKB_L= 200;
u8 pro=1;
u8 pro_L=1;uint8_t CNT_L= 0;
extern bool PUSH_PWM;
extern bool P_open;
void ADC_Config(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  GPTM_TimeBaseInitTypeDef TimeBaseInit;
  GPTM_OutputInitTypeDef OutInit;

  RETARGET_Configuration();           /* Retarget Related configuration                                     */

  /* Enable the ADC Interrupts                                                                              */
  NVIC_EnableIRQ(ADC_IRQn);

  /* Enable peripheral clocks of ADC, GPTM0                                                                 */
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.GPTM0      = 1;
  CKCUClock.Bit.ADC        = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Configure GPTM0 Channel 3 as PWM output mode used to trigger ADC start of conversion
     every */
  TimeBaseInit.CounterMode = GPTM_CNT_MODE_UP;
  TimeBaseInit.CounterReload = 4999;
  TimeBaseInit.Prescaler = 1799;
  TimeBaseInit.PSCReloadTime = GPTM_PSC_RLD_IMMEDIATE;
  GPTM_TimeBaseInit(HT_GPTM0, &TimeBaseInit);

  OutInit.Channel = GPTM_CH_3;
  OutInit.OutputMode = GPTM_OM_PWM2;
  OutInit.Control = GPTM_CHCTL_ENABLE;
  OutInit.Polarity = GPTM_CHP_NONINVERTED;
  OutInit.Compare = 4999;
  GPTM_OutputInit(HT_GPTM0, &OutInit);

  /* ADCLK frequency is set to 72/64 MHz = 1.125MHz                                                         */
  CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV64);

  /* Configure specify GPIO's AFIO mode as ADC function                                                     */
  AFIO_GPAConfig(AFIO_PIN_5 | AFIO_PIN_6 , AFIO_MODE_2);
  AFIO_GPAConfig(AFIO_PIN_2 | AFIO_PIN_3 , AFIO_MODE_2);
	AFIO_GPAConfig(AFIO_PIN_0 | AFIO_PIN_1 , AFIO_MODE_2);
	AFIO_GPAConfig(AFIO_PIN_4 , AFIO_MODE_2);
  /* One Shot Mode, Length 4, SubLength 1                                                                   */
  ADC_RegularGroupConfig(HT_ADC, ONE_SHOT_MODE, 5, 1);

  /* ADC Channel n, Rank 0, Sampling clock is (1.5 + 10) ADCLK
        Conversion time = (sampling clock + 12.5) / ADCLK = 21.3 uS */
  ADC_RegularChannelConfig(HT_ADC, ADC_CH_3, 0, 10);//OUT2V
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_5, 1, 5);//UV0 
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_6, 2, 6);//UV1
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_2, 3, 10);//OUT1V
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_4, 4, 5);//SCP
//	ADC_RegularChannelConfig(HT_ADC, ADC_CH_1, 5, 5);//NTC3
  /* Use GPTM0 CH3O as ADC trigger source                                                                   */
  ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_GPTM0_CH3O);

  /* Enable ADC cycle end of conversion interrupt,
     The ADC ISR will store the ADC result into global variable gADC_Result and
     set gADC_CycleEndOfConversion as TRUE */
  ADC_IntConfig(HT_ADC, ADC_INT_CYCLE_EOC, ENABLE);

  /* Enable GPTM which will trigger ADC start of conversion every 1 second                                  */
  GPTM_Cmd(HT_GPTM0, ENABLE);

//  while (1)
//  {
//    if (gADC_CycleEndOfConversion)
//    {
//      /* Output gADC_Result if needed.                                                                      */
//      printf("ADC Result Rank0:%4u Rank1:%4u Rank2:%4u\r\n",
//        (int)gADC_Result[0], (int)gADC_Result[1], (int)gADC_Result[2]);
//      gADC_CycleEndOfConversion = FALSE;
//    }
//  }
}	
void MCTM_Config(void)
{
	

  /* Enable PCLK of BUZZER and AFIO                                                                         */
  BUZZER_TM_CLK(CKCUClock) = 1;
  CKCUClock.Bit.AFIO       = 1;
	CKCUClock.Bit.BFTM0      = 1;
//  CKCUClock.Bit.BFTM1      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Configure the BUZZER_GPIO_PIN as MCTM channel output AFIO function                                     */
  HT32F_DVB_GPxConfig(BUZZER_GPIO_ID, BUZZER_AFIO_PIN, BUZZER_AFIO_MODE);
	
  /* Compute CRR and PSCR value                                                                             */
  wCRR = (SystemCoreClock / BEE_FREQ) - 1;
  while ((wCRR / (wPSCR + 1)) > 0xFFFF)
  {
    wPSCR++;
  }
  wCRR = wCRR / (wPSCR + 1);

  /* Init BUZZER MCTM time-base                                                                             */
  MCTM_TimeBaseInitStructure.CounterReload = wCRR;
  MCTM_TimeBaseInitStructure.Prescaler = wPSCR;
  MCTM_TimeBaseInitStructure.RepetitionCounter = 0;
  MCTM_TimeBaseInitStructure.CounterMode = MCTM_CNT_MODE_UP;
  MCTM_TimeBaseInitStructure.PSCReloadTime = MCTM_PSC_RLD_IMMEDIATE;
  MCTM_TimeBaseInit(BUZZER_TM, &MCTM_TimeBaseInitStructure);

  /* Clear Update Event Interrupt flag                                                                      */
  MCTM_ClearFlag(BUZZER_TM, MCTM_FLAG_UEV1);

  /* Init BUZZER MCTM Channel x to output PWM waveform with 33% duty                                        */
  MCTM_OutputInitStructure.Channel = MCTM_CH_2;
  MCTM_OutputInitStructure.OutputMode = MCTM_OM_PWM2;
  MCTM_OutputInitStructure.Control = MCTM_CHCTL_ENABLE;
  MCTM_OutputInitStructure.ControlN = MCTM_CHCTL_DISABLE;//MCTM_CHCTL_ENABLE;
  MCTM_OutputInitStructure.Polarity = MCTM_CHP_NONINVERTED;
  MCTM_OutputInitStructure.PolarityN = MCTM_CHP_NONINVERTED;
  MCTM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
  MCTM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
  MCTM_OutputInitStructure.Compare = ((wCRR + 1) * (1000 - ZKB)) / 1000;
  MCTM_OutputInit(BUZZER_TM, &MCTM_OutputInitStructure);
	

  NVIC_EnableIRQ(BFTM0_IRQn);
	BFTM_SetCompare(HT_BFTM0, SystemCoreClock/15000 );
  BFTM_SetCounter(HT_BFTM0, 0);
  BFTM_IntConfig(HT_BFTM0, ENABLE);
  BFTM_EnaCmd(HT_BFTM0, DISABLE);
  /* Enable interrupt of BUZZER MCTM update event                                                           */
  NVIC_EnableIRQ(BUZZER_IRQn);
  MCTM_IntConfig(BUZZER_TM, MCTM_INT_UEV1, ENABLE);

  /* BUZZER MCTM Channel Main Output enable                                                                 */
  MCTM_CHMOECmd(BUZZER_TM, DISABLE);
  /* BUZZER MCTM counter enable                                                                             */
  MCTM_Cmd(BUZZER_TM, ENABLE);
	
  /* Output Bee voice after reset                                                                           */
  for (wBeeIndex = 0; wBeeIndex < BEE_NBR; wBeeIndex++)
  {
    MCTM_ChannelConfig(BUZZER_TM, BUZZER_TM_CHANNEL, MCTM_CHCTL_ENABLE);
    wMctmUpdateDownCounter = BEE_ACTIVE_TIME;
    while (wMctmUpdateDownCounter);

//    MCTM_ChannelConfig(BUZZER_TM, BUZZER_TM_CHANNEL, MCTM_CHCTL_DISABLE);
//    wMctmUpdateDownCounter = BEE_INACTIVE_TIME;
//    while (wMctmUpdateDownCounter);
  }
}	
void delay1(u32 i)
{
    while(i>0)
    {
        i--;
    }
}
void PWM_Control(void)
{
//		if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
//			{	delay1(2600000);
//				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
//				{
//					Uout+=5;
//				}
//			}
//		if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
//			{	delay1(2600000);
//				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
//				{
//						Uout-=5;
//					
//				}
//			}
	if(PUSH_PWM)
	{
	CNT++;
		if(CNT>100*pro)
		{CNT=0;
			if(Uout>UV0)
			{
				if(Uout>=UV0*3)
						{
								pro=1;
								ZKB-=50;
						}
					else if(Uout>=UV0*2&&Uout<UV0*3)
						{
								ZKB-=25;pro=1;
						}
					else if(Uout>=(UV0+3)&&Uout<UV0*2)
					{
						if(Uout>=(UV0+30))
						{
							ZKB-=10;
							pro=1;
						}	
						else if(Uout>=(UV0+20))
						{
							ZKB-=5;
							pro=1;
						}	
						else if(Uout>=(UV0+10))
						{
							ZKB-=3;
							pro=2;
						}	
						else if(Uout>=(UV0+3))
						{
							ZKB--;
							pro=3;
						}	
					}
			}
			if(Uout<UV0)
			{
				if((Uout*3)<=UV0)
					{
						ZKB+=50;pro=1;
					}
				else if((Uout*2)<=UV0&&UV0<=(Uout*3))
					{
							ZKB+=25;pro=1;
					}
				else if((Uout+3)<=UV0)
					{
						if((Uout+30)<=UV0)
						{
							ZKB+=10;
							pro=1;
						}	
						else if((Uout+20)<=UV0)
						{
							ZKB+=5;
							pro=1;
						}	
						else if((Uout+10)<=UV0)
						{
							ZKB+=3;
							pro=2;
						}	
						else if((Uout+3)<=UV0)
						{
							ZKB++;
							pro=3;
						}	
					}
			}
			
		if(ZKB>1000)
		{	ZKB=1000;
		}
		if(ZKB<1)
		{	ZKB=0;
		}
	}
	}
	else
	{
		ZKB=1000;
	}
	if(!P_open)
	{
		ZKB=1000;
	}
	 MCTM_OutputInitStructure.Compare = ((wCRR + 1) * (1000 - ZKB)) / 1000;
	 MCTM_OutputInit(BUZZER_TM, &MCTM_OutputInitStructure);
		
			
}
void PWM2_Control(void)
{
//			if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
//			{	delay1(2600000);
//				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
//				{
//					Uout+=5;
//				}
//			}
//		if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
//			{	delay1(2600000);
//				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
//				{
//						Uout-=5;
//					
//				}
//			}
//	if((Uout+5)>OUT1V)
//	{
//		ZKB_L--;
//		if(ZKB_L==0)
//			ZKB_L=500;
//	}
//		if((Uout-5)<OUT1V)
//	{
//		ZKB_L++;
//		if(ZKB_L==500)
//			ZKB_L=0;
//	}
	if(!PUSH_PWM)
	{
		CNT_L++;
		if(CNT_L>pro_L)
		{CNT_L=0;
		if(Uout>OUT1V)
				{
					if(Uout>=OUT1V*3)
							{
									pro_L=1;
									ZKB_L-=50;
							}
						else if(Uout>=OUT1V*2&&Uout<OUT1V*3)
							{
									ZKB_L-=25;pro_L=1;
							}
						else if(Uout>=(OUT1V+3)&&Uout<OUT1V*2)
						{
							if(Uout>=(OUT1V+30))
							{
								ZKB_L-=10;
								pro_L=1;
							}	
							else if(Uout>=(OUT1V+20))
							{
								ZKB_L-=5;
								pro_L=1;
							}	
							else if(Uout>=(OUT1V+10))
							{
								ZKB_L-=3;
								pro_L=2;
							}	
							else if(Uout>=(OUT1V+3))
							{
								ZKB_L--;
								pro_L=3;
							}	
						}
						if(ZKB_L<1)
							ZKB_L=1;
						if(ZKB_L>500)
							ZKB_L=499;
				}
				if(Uout<OUT1V)
				{
					if((Uout*3)<=OUT1V)
						{
							ZKB_L+=50;pro_L=1;
						}
					else if((Uout*2)<=OUT1V&&OUT1V<=(Uout*3))
					{
							ZKB_L+=25;pro_L=1;
					}
					else if((Uout+3)<=OUT1V)
					{
							if((Uout+30)<=OUT1V)
							{
								ZKB_L+=10;
								pro_L=1;
							}	
							else if((Uout+20)<=OUT1V)
							{
								ZKB_L+=5;
								pro_L=1;
							}	
							else if((Uout+10)<=OUT1V)
							{
								ZKB_L+=3;
								pro_L=2;
							}	
							else if((Uout+3)<=OUT1V)
							{
								ZKB_L++;
								pro_L=3;
							}	
					}
					if(ZKB_L>499)
							ZKB_L=500;
					if(ZKB_L<1)
							ZKB_L=0;
				}
			}
		}
	else
	{
		ZKB_L=0;
	}
	if(!P_open)
	{
		ZKB_L=500;
	}
}

