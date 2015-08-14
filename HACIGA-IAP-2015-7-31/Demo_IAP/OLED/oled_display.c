/*********************************************************************************************************//**
 * @file    oled_display.c
 * @version V1.00
 * @date    24/06/2015
 * @brief   UG-2832TSWEG04-topwin
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2015 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "oled.h"
#include "oled_display.h"

extern bool Open_State;//开机状态
extern bool K_Count;//3S
extern vu32 gTimebaseDelayCounter;//3s关机
u16 bat_time=0;
bool Key_Delay=FALSE;//连续按键延时0.7s
u8 Main_key=0;//检测按键数
vu32 Key_Delaycnt=0;
u8 Key_NUM=1;
u8 Key_NUM2=1;
u8 Key_NUM3=1;
u32 gTimebaseDelayCounter_2s=0;
u32 Add_delay=0;
bool Keep_delay=FALSE;
u32 Sub_delay=0;
bool Keep_Add=FALSE;
//bool One_Add=FALSE;
bool Keep_Sub=FALSE;
//bool One_Sub=FALSE;
extern bool Enter_output;
extern bool Enter_menu;
extern bool Enter_Ajustment;
//extern bool TEST_R;
extern u16 ZKB_L;//pwm1低频占空比
extern uint32_t ZKB;//pwm2高频占空比
extern u32 P_W;//输出功率0.2w
extern bool P_open;//功率开启状态
extern bool PUSH_PWM;//升压
extern u16 UV1;//电池电压
u16 Return_main_time=0;//6s返回主界面
void open_pic(void)
{
	u8 t;
	for(t=1;t<14;t++)
		{
			if(t==9)
			{	delay(8000000);
				write_i(0x81);    /*set charge pump enable*/
				write_i(0x00); //亮度加16、14
				OLED_ShowPic(0,32,8,1);
				OLED_Refresh_Gram();
				delay(6000000);
			}
			else if(t==13)
			{	delay(8000000);
				write_i(0x81);    /*set charge pump enable*/
				write_i(0x00); //亮度加16、14
				OLED_ShowPic(0,32,12,1);
				OLED_Refresh_Gram();
				delay(6000000);
				write_i(0x81);    /*set charge pump enable*/
				write_i(0xFF); //亮度加16、14
			}
			else
			{
				write_i(0x81);    /*set charge pump enable*/
				write_i(0xff); //亮度加16、14
			}
			OLED_ShowPic(0,32,t,1);
//		OLED_ShowChar(48,38,t,16,1);//显示ASCII字符	   
			OLED_Refresh_Gram();
			delay(2000000);
	

		}
}
void bat_display(void)
{
		OLED_ShowPic(0,32,15,1);
		OLED_delect(47,42,83,54,0);//删除数据	
		OLED_delect(88,41,125,54,0);//删除数据	
		OLED_Refresh_Gram();
		delay(2600000);
		OLED_Fill(47,42,52,54,1);//電池最低格
		delay(2600000);
		OLED_Fill(54,42,58,54,1);//電池2
		delay(2600000);
		OLED_Fill(60,42,64,54,1);//電池3
		delay(2600000);
		OLED_Fill(66,42,70,54,1);//電池4
		delay(2600000);
		OLED_Fill(72,42,76,54,1);//電池5
		delay(2600000);
		OLED_Fill(78,42,82,54,1);//電池6
		delay(2600000);
//		OLED_Fill(79,42,83,54,1);//電池7
//		delay(2600000);
}
void pc_connect(void)
{
	OLED_ShowPic(0,32,14,1);
	OLED_Refresh_Gram();
}
extern u16 Uout;
extern u16 P_r;//电阻值
void V_mode(void)
{
	u8 R_H=0;u8 R_L=0;u8 R_LL=0;
	u8 V_H=0;u8 V_L=0;u8 V_LL=0;
	u8 P_H=0;u8 P_L=0;u8 P_LL=0;		
	if(Keep_Add&&(!Add_delay))//连加
				{
					Add_delay=100;
					P_W+=10;
					if(P_W>5000)
						P_W=5000;
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
					{	delay(150000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
							{
								Keep_Add=FALSE;gTimebaseDelayCounter_2s=0;Keep_delay=FALSE;Add_delay=100;
							}
					}
				}
	else if(Keep_Sub&&(!Sub_delay))//功率连减
				{
					Sub_delay=100;
					if(P_W>10)
						P_W-=10;
					else
						P_W=0;
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
					{	delay(150000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
							{
								Keep_Sub=FALSE;gTimebaseDelayCounter_2s=0;Keep_delay=FALSE;Sub_delay=100;
							}
					}
				}
	else if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)//功率加
							{		delay(1600000);
								if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
								{	
									Keep_delay=TRUE;
//									while(!GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)&&gTimebaseDelayCounter_2s);
									if((GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0) && (gTimebaseDelayCounter_2s >= 2000))
									{	
										Keep_Add=TRUE;Add_delay=100;gTimebaseDelayCounter_2s=0;Keep_delay=FALSE;
									}		
									else
									{
										P_W+=10;
										if(P_W>5000)
											P_W=5000;
									}
									
								}
							}				
		
				
		else if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)//功率减
							{	delay(1600000);
								if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
								{	
									Keep_delay=TRUE;
//									while(!GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)&&gTimebaseDelayCounter_2s);
									if((GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0) && (gTimebaseDelayCounter_2s >= 2000))
									{	
										Keep_Sub=TRUE;Sub_delay=100;gTimebaseDelayCounter_2s=0;Keep_delay=FALSE;
									}		
									else
									{
										if(P_W>10)
											P_W-=10;
										else
											P_W=0;
										
									}
								
								}
							}											
		else if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)//按键次数检测
			{	delay(200);
				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{		
						if(Key_Delaycnt<700)
						{
							K_Count=TRUE;
							while(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0&&gTimebaseDelayCounter<3000);
							
							Key_Delay=TRUE;
							Main_key++;
							Key_Delaycnt=0;
							if(Main_key==3)
									Enter_menu=TRUE;	
							if(gTimebaseDelayCounter>=3000)//关机
							{
								Open_State=FALSE;
								open_pic();
								OLED_Display_Off();
								PUSH_PWM=FALSE;
								P_W=0;
								Uout=0;
								ZKB_L=500;
								ZKB=1000;
								GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_5);//ENH
								GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_7);//EN12V
							OLED_Display_Off();
						  GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_8);//PB0拉高
								Main_key=0;Key_Delay=FALSE;Key_Delaycnt=0;
							}
								K_Count=FALSE;gTimebaseDelayCounter=0;
						}
						else
						{
							Main_key=0;Key_Delay=FALSE;Key_Delaycnt=0;
						}
					}
			}	
		if(gTimebaseDelayCounter_2s>=2000)
		{
			gTimebaseDelayCounter_2s=0;Keep_delay=FALSE;
		}
										
		
		OLED_ShowPic(0,31,21,1);
//		OLED_Refresh_Gram();
//		OLED_Fill(0,30,32,64,0);//删除数据
		OLED_delect(0,32,15,64,0);//删除数据	
		if(P_r<1000)
		{
			R_H=P_r/100;
			R_L=P_r%100;
			OLED_ShowNum(0,32,R_H,1,16);//hms
			OLED_ShowString(8,32,".");
			OLED_ShowNum(10,32,R_L,2,16);
		}
		else
		{
			R_H=P_r/100;
			R_L=P_r%100;
			R_LL=R_L/10;
			OLED_ShowNum(0,32,R_H,2,16);//hms
			OLED_ShowString(16,32,".");
			OLED_ShowNum(18,32,R_LL,1,16);
		}
		if(Uout<1000)
		{
			V_H=Uout/100;
			V_L=Uout%100;
			OLED_ShowNum(0,47,V_H,1,16);//V
			OLED_ShowString(8,47,".");
			OLED_ShowNum(10,47,V_L,2,16);
		}
		else
		{
			V_H=Uout/100;
			V_L=Uout%100;
			V_LL=V_L/10;
			OLED_ShowNum(0,47,V_H,2,16);//V
			OLED_ShowString(16,47,".");
			OLED_ShowNum(18,47,V_LL,1,16);
		}
		OLED_delect(42,32,110,64,0);//删除数据	
		OLED_delect(116,37,123,59,0);//删除电池数据	
		if(UV1>330)
			OLED_delect(116,53,122,59,1);//填充数据	电池
//		if(bat_time>4000)
//		{
//			bat_time=0;
		if(UV1>340)
			OLED_delect(116,49,122,53,1);//填充数据	电池
		if(UV1>350)
			OLED_delect(116,45,122,49,1);//填充数据	电池
		if(UV1>360)
			OLED_delect(116,41,122,45,1);//填充数据	电池
		if(UV1>370)
			OLED_delect(116,37,122,41,1);//填充数据	电池
//		}
		if(P_W<1000)
		{
			P_H=P_W/100;
			P_L=P_W%100;
			OLED_ShowNum_big(44,32,P_H,1,66);//P  _big
			OLED_ShowString_big(57,32,".");//_big
			OLED_ShowNum_big(67,32,P_L,2,66);//_big
			OLED_ShowString_big(96,32,"w");//_big
		}
		else
		{
			P_H=P_W/100;
			P_L=P_W%100;
			P_LL=P_L/10;
			OLED_ShowNum_big(44,32,P_H,2,66);//P _big
			OLED_ShowString_big(73,32,".");//_big
			OLED_ShowNum_big(83,32,P_LL,1,66);//_big
			OLED_ShowString_big(96,32,"w");//_big
		}
		OLED_Refresh_Gram();
}
void P_Select(void)
{
		OLED_ShowString_select(33,38,"F5");  
		OLED_ShowString(48,38,"50.0W");
		OLED_ShowPic(0,59,16,1);
		OLED_Refresh_Gram();
}
bool overturn=FALSE;
void main_menu(void)
{

	if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
			{	delay(2600000);
				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
				{
					Key_NUM--;
					if(Key_NUM==0)
						Key_NUM=5;
				}
			}
		if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
			{	delay(2600000);
				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
				{
					Key_NUM++;
					if(Key_NUM==6)
						Key_NUM=1;
					
				}
			}

		if(Return_main_time>6000)
				{
					Return_main_time=0;
					Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;//返回主界面
				}
		switch (Key_NUM)
			{
				case 1:
				{  
					
						OLED_delect(0,32,128,64,0);//clear
						OLED_ShowString(23,38,"Output Mode");
						OLED_ShowPic(0,59,16,1);
						OLED_Refresh_Gram();
					
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							Enter_output=TRUE;Enter_menu=FALSE;
						}
					}
					break;
				}
				case 2:
				{	
					
						OLED_delect(0,32,128,64,0);//clear
						OLED_ShowString(9,38,"Ajustment Mode");
						OLED_ShowPic(0,59,17,1);
						OLED_Refresh_Gram();
					
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							Enter_Ajustment=TRUE;Enter_menu=FALSE;
						}
					}
					break;
				} 
				case 3:
				{	
					OLED_delect(0,32,128,64,0);//clear
					OLED_ShowString(29,38,"Left&Right");
					OLED_ShowPic(0,59,18,1);
					OLED_Refresh_Gram();
						
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							if(overturn)
							{
								overturn=FALSE;
								write_i(0xA1);    /*set segment remap*///A0左右開始坐標A1 127
								write_i(0xC7);    /*Com scan direction*///c8掃描方向C7
							}
							else
							{
								overturn=TRUE;
								write_i(0xA0);    /*set segment remap*///A0左右開始坐標A1 127
								write_i(0xC8);    /*Com scan direction*///c8掃描方向C7
							}
							Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;
						}
					}
					break;
				}case 4:
				{ 
					OLED_delect(0,32,128,64,0);//clear
					OLED_ShowString(43,38,"Exit");
					OLED_ShowPic(0,59,19,1);
					OLED_Refresh_Gram();
					
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;
						}
					}
					break;
				}
				case 5:
				{
					OLED_delect(0,32,128,64,0);//clear
					OLED_ShowString(33,38,"Shutdown");
					OLED_ShowPic(0,59,20,1);
					OLED_Refresh_Gram();
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							P_open=FALSE;	
							//PWM2
							PUSH_PWM=FALSE;
							P_W=0;
							Uout=0;
							ZKB_L=500;
							ZKB=1000;
							GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_5);//ENH
							GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_7);//EN12V
						//	GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_0);//PWM1
							BFTM_EnaCmd(HT_BFTM0, ENABLE);
							/* BUZZER MCTM Channel Main Output enable                                                                 */
							MCTM_CHMOECmd(BUZZER_TM, ENABLE);
							Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;
						}
					}
							
					break;
				} 
				
				default:	break;
			}
}
void output_menu(void)
{

	if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
			{	delay(2600000);
				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
				{
					Key_NUM2--;
					if(Key_NUM2==0)
						Key_NUM2=3;
				}
			}
		if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
			{	delay(2600000);
				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
				{
					Key_NUM2++;
					if(Key_NUM2==4)
						Key_NUM2=1;
					
				}
			}
		if(Return_main_time>6000)
				{
					Return_main_time=0;
					Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;//返回主界面
				}
		switch (Key_NUM2)
			{
				case 1:
				{  
					
						OLED_delect(0,32,128,64,0);//clear
						OLED_ShowString(27,38,"Power Mode");
						OLED_ShowPic(0,59,22,1);
						OLED_Refresh_Gram();
					
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							if(UV1>320)
							{
							P_open=TRUE;	
							PUSH_PWM=FALSE;			
							//PWM2
							GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_7);//EN12V
						//	GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_0);//PWM1
							GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_5);//ENH
							BFTM_EnaCmd(HT_BFTM0, ENABLE);
							/* BUZZER MCTM Channel Main Output enable                                                                 */
							MCTM_CHMOECmd(BUZZER_TM, ENABLE);
							Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;
							OLED_Clear();
							OLED_ShowString(12,42,"Wattage mode");//功率模式
							}
							else
							{
								OLED_Clear();
								OLED_ShowString(32,48,"Weak Battery");//电压弱，小于3.2
							}
							OLED_Refresh_Gram();
							delay(10000000);
						}
					}
					break;
				}
				case 2:
				{	
					
						OLED_delect(0,32,128,64,0);//clear
						OLED_ShowString(23,30,"Mechanical");
						OLED_ShowPic(0,60,23,1);
						OLED_ShowString(43,44,"Mode");
						OLED_Refresh_Gram();
					
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							Enter_menu=TRUE;Enter_output=FALSE;
							OLED_Clear();
							OLED_ShowString(18,42,"Undeveloped");//未开发
							OLED_Refresh_Gram();
							delay(10000000);
						}
					}
					break;
				} 
				case 3:
				{	
						OLED_delect(0,32,128,64,0);//clear
						OLED_ShowString(23,30,"Temperature");
						OLED_ShowPic(0,60,24,1);
						OLED_ShowString(20,44,"Control Mode");
						OLED_Refresh_Gram();
					
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							Enter_menu=TRUE;Enter_output=FALSE;
							OLED_Clear();
							OLED_ShowString(18,42,"Undeveloped");//未开发
							OLED_Refresh_Gram();
							delay(10000000);
						}
					}
					break;
				}
				default:	break;
			}
}
#define P_ADDR  ((vu32*)(40*1024+1024*11));
u16 P[5]={0};u8 P_i=0;
u8 Pi_H=0;u8 Pi_L=0;u8 Pi_LL=0;	
void Ajustment_menu(void)
{
	
	vu16 *p_add_P;
	p_add_P=(u16*)P_ADDR;	
	for(P_i=0;P_i<5;P_i++)
		{
			P[P_i]=*(p_add_P+P_i);
			if(P[P_i]>5000)//防止功率过大
				P[P_i]=0;
		}
	if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
			{	delay(2600000);
				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_14)==0)
				{
					Key_NUM3--;
					if(Key_NUM3==0)
						Key_NUM3=6;
				}
			}
		if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
			{	delay(2600000);
				if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_13)==0)
				{
					Key_NUM3++;
					if(Key_NUM3==7)
						Key_NUM3=1;
					
				}
			}
		if(Return_main_time>6000)
				{
					Return_main_time=0;
					Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;//返回主界面
				}
		switch (Key_NUM3)
			{
				case 1:
				{  
					
						OLED_delect(0,32,128,64,0);//clear
						OLED_ShowString_select(33,38,"F1");  
						if(P[0]<1000)
							{
								Pi_H=P[0]/100;
								Pi_L=P[0]%100;
								OLED_ShowNum(51,38,Pi_H,1,16);//
								OLED_ShowString(58,38,".");//
								OLED_ShowNum(62,38,Pi_L,2,16);//
								OLED_ShowString(80,38,"w");//
							}
							else
							{
								Pi_H=P[0]/100;
								Pi_L=P[0]%100;
								Pi_LL=Pi_L/10;
								OLED_ShowNum(51,38,Pi_H,2,16);//
								OLED_ShowString(66,38,".");//
								OLED_ShowNum(71,38,Pi_LL,1,16);//
								OLED_ShowString(80,38,"w");//
							}
						OLED_ShowPic(0,59,25,1);
						OLED_Refresh_Gram();
					
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							P_W=P[0];
							Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;
						}
					}
					
					break;
				}
				case 2:
				{	
					
						OLED_delect(0,32,128,64,0);//clear
						OLED_ShowString_select(33,38,"F2");  
						if(P[1]<1000)
							{
								Pi_H=P[1]/100;
								Pi_L=P[1]%100;
								OLED_ShowNum(51,38,Pi_H,1,16);//
								OLED_ShowString(58,38,".");//
								OLED_ShowNum(62,38,Pi_L,2,16);//
								OLED_ShowString(80,38,"w");//
							}
							else
							{
								Pi_H=P[1]/100;
								Pi_L=P[1]%100;
								Pi_LL=Pi_L/10;
								OLED_ShowNum(51,38,Pi_H,2,16);//
								OLED_ShowString(66,38,".");//
								OLED_ShowNum(71,38,Pi_LL,1,16);//
								OLED_ShowString(80,38,"w");//
							}
						OLED_ShowPic(0,59,26,1);
						OLED_Refresh_Gram();
					
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							P_W=P[1];
							Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;
						}
					}
					break;
				} 
				case 3:
				{	
						OLED_delect(0,32,128,64,0);//clear
						OLED_ShowString_select(33,38,"F3");  
						if(P[2]<1000)
							{
								Pi_H=P[2]/100;
								Pi_L=P[2]%100;
								OLED_ShowNum(51,38,Pi_H,1,16);//
								OLED_ShowString(58,38,".");//
								OLED_ShowNum(62,38,Pi_L,2,16);//
								OLED_ShowString(80,38,"w");//
							}
							else
							{
								Pi_H=P[2]/100;
								Pi_L=P[2]%100;
								Pi_LL=Pi_L/10;
								OLED_ShowNum(51,38,Pi_H,2,16);//
								OLED_ShowString(66,38,".");//
								OLED_ShowNum(71,38,Pi_LL,1,16);//
								OLED_ShowString(80,38,"w");//
							}
						OLED_ShowPic(0,59,27,1);
						OLED_Refresh_Gram();
					
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							P_W=P[2];
							Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;
						}
					}
					break;
				}case 4:
				{ 
						OLED_delect(0,32,128,64,0);//clear
						OLED_ShowString_select(33,38,"F4");  
						if(P[3]<1000)
							{
								Pi_H=P[3]/100;
								Pi_L=P[3]%100;
								OLED_ShowNum(51,38,Pi_H,1,16);//
								OLED_ShowString(58,38,".");//
								OLED_ShowNum(62,38,Pi_L,2,16);//
								OLED_ShowString(80,38,"w");//
							}
							else
							{
								Pi_H=P[3]/100;
								Pi_L=P[3]%100;
								Pi_LL=Pi_L/10;
								OLED_ShowNum(51,38,Pi_H,2,16);//
								OLED_ShowString(66,38,".");//
								OLED_ShowNum(71,38,Pi_LL,1,16);//
								OLED_ShowString(80,38,"w");//
							}
						OLED_ShowPic(0,59,28,1);
						OLED_Refresh_Gram();
				
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							P_W=P[3];
							Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;
						}
					}
					break;
				}
				case 5:
				{	
						OLED_delect(0,32,128,64,0);//clear
						OLED_ShowString_select(33,38,"F5");  
						if(P[4]<1000)
							{
								Pi_H=P[4]/100;
								Pi_L=P[4]%100;
								OLED_ShowNum(51,38,Pi_H,1,16);//
								OLED_ShowString(58,38,".");//
								OLED_ShowNum(62,38,Pi_L,2,16);//
								OLED_ShowString(80,38,"w");//
							}
							else
							{
								Pi_H=P[4]/100;
								Pi_L=P[4]%100;
								Pi_LL=Pi_L/10;
								OLED_ShowNum(51,38,Pi_H,2,16);//
								OLED_ShowString(66,38,".");//
								OLED_ShowNum(71,38,Pi_LL,1,16);//
								OLED_ShowString(80,38,"w");//
							}
						OLED_ShowPic(0,59,29,1);
						OLED_Refresh_Gram();
					
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							P_W=P[4];
							Enter_Ajustment=FALSE;Enter_menu=FALSE;Enter_output=FALSE;
						}
					}
					break;
				} 
				case 6:
				{	
						OLED_delect(0,32,128,64,0);//clear
						OLED_ShowString(35,38,"Return");
						OLED_ShowPic(0,59,30,1);
						OLED_Refresh_Gram();
					
					if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
					{	delay(2600000);
						if(GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)==0)
						{
							Enter_menu=TRUE;Enter_Ajustment=FALSE;
						}
					}
					break;
				} 
				
				default:	break;
			}
}

