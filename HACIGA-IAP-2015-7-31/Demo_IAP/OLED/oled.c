/*********************************************************************************************************//**
 * @file    oled.c
 * @version V1.00
 * @date    24/06/2015
 * @brief   UG-2832TSWEG04-topwin
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2015 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "OLED.h"
#include "oledfont.h"
#include "cartoon2.h"
u8 OLED_GRAM[128][8];

#if 1
void  ssd1306()
{
       GPIO_ClearOutBits(HT_GPIOA, AFIO_PIN_15);//RESET=0
       delay(1000);
       GPIO_SetOutBits(HT_GPIOA, AFIO_PIN_15);//RESET=1
       write_i(0xAE);    /*display off*/


       write_i(0x00);    /*set lower column address*/
       write_i(0x10);    /*set higher column address*/

       write_i(0x00);    /*set display start line*/

       write_i(0xB0);    /*set page address*/

       write_i(0x81);    /*contract control*/
       write_i(0xFF);    /*128*///ff對比度

       write_i(0xA1);    /*set segment remap*///A0左右開始坐標A1 127
	
       write_i(0xA6);    /*normal / reverse*///A7黑字點反向A6

       write_i(0xA8);    /*multiplex ratio*/
       write_i(0x1F);    /*duty = 1/32*/

       write_i(0xC7);    /*Com scan direction*///c8掃描方向C7

       write_i(0xD3);    /*set display offset*/
       write_i(0x00);

       write_i(0xD5);    /*set osc division*/
       write_i(0x80);

       write_i(0xD9);    /*set pre-charge period*/
       write_i(0x04);

       write_i(0xDA);    /*set COM pins*/
       write_i(0x00);

        write_i(0xdb);    /*set vcomh*/
       write_i(0x30);

        write_i(0x8d);    /*set charge pump enable*/
       write_i(0x14); //亮度加16、14
				
       write_i(0xAF);    /*display ON*/
       OLED_Clear();
                
}
#endif
void write_i(unsigned char  ins)
{
  unsigned char  m,da;
  unsigned int j;
    //DC=0;
     GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_1);//GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_1);
    //CS=0;
     GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_3);
            GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_4);
     da=ins;
  for(j=0;j<8;j++)
    {
      m=da;
      //SCL=0;
            GPIO_ClearOutBits(HT_GPIOD, AFIO_PIN_1);
      m=m&0x80;
      if(m==0x80)
        {
           //SDA=1;
                    GPIO_SetOutBits(HT_GPIOD, AFIO_PIN_2);
         }
       else
         {
             //SDA=0;
                     GPIO_ClearOutBits(HT_GPIOD, AFIO_PIN_2);
         }

        da=da<<1;
        //SCL=1;
                 GPIO_SetOutBits(HT_GPIOD, AFIO_PIN_1);
      }
  //CS=1;
            GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_3);
            GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_4);
}

void write_d(unsigned char  dat)
{
  unsigned char  m,da;
  unsigned int j;
     //DC=1;
        GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_1);
     //CS=0;
        GPIO_ClearOutBits(HT_GPIOB, AFIO_PIN_3);
     da=dat;
  for(j=0;j<8;j++)
    {
      m=da;
      //SCL=0;
            GPIO_ClearOutBits(HT_GPIOD, AFIO_PIN_1);
      m=m&0x80;
      if(m==0x80)
        {
           //SDA=1;
                    GPIO_SetOutBits(HT_GPIOD, AFIO_PIN_2);
         }
       else
         {
             //SDA=0;
                     GPIO_ClearOutBits(HT_GPIOD, AFIO_PIN_2);
         }

        da=da<<1;
        //SCL=1;
                 GPIO_SetOutBits(HT_GPIOD, AFIO_PIN_1);
      }
  //CS=1;
            GPIO_SetOutBits(HT_GPIOB, AFIO_PIN_3);
}

void delay(u32 i)
{
    while(i>0)
    {
        i--;
    }
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    u8 i,n;
    for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;
    OLED_Refresh_Gram();//更新显示
}

//更新显存到LCD
void OLED_Refresh_Gram(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
                 write_i(0xb0+i);    //设置页地址（0~7）
                 write_i(0x00);   //设置显示位置—列低地址
                 write_i(0x10);     //设置显示位置—列高地址
        for(n=0;n<128;n++)
                 write_d(OLED_GRAM[n][i]);//OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
    }
}
//开启OLED显示    
void OLED_Display_On(void)
{
	write_i(0X8D);  //SET DCDC命令
	write_i(0X14);  //DCDC ON
	write_i(0XAF);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	write_i(0X8D);  //SET DCDC命令
	write_i(0X10);  //DCDC OFF
	write_i(0XAE);  //DISPLAY OFF
}		   			 
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)
		OLED_GRAM[x][pos]|=temp;
	else 
		OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_delect(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
}
////在指定位置显示一个字符,包括部分字符
////x:0~127
////y:0~63
////mode:0,反白显示;1,正常显示				 
////size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	chr=chr-' ';//得到偏移后的值				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1608[chr][t];  //调用1206字体
		else temp=oled_asc2_1608[chr][t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
////在指定位置显示一个字符,包括部分字符
////x:0~127
////y:0~63
////mode:0,反白显示;1,正常显示				 
////size:选择字体 19 
//横向扫描
void OLED_ShowChar_big(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 x0=x;
	chr=chr-' ';//得到偏移后的值				   
    for(t=0;t<size;t++)
    {   
			temp=oled_asc2_3316[chr][t];  //调用3316字体          
        for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)OLED_DrawPoint(x,y,mode);
				else OLED_DrawPoint(x,y,!mode);
				temp<<=1;
				x++;
				if((x-x0)==16)
					{
						x=x0;
						y++;
						break;
					}
			}  	 
    }          
}
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum_big(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar_big(x+(size/4)*t-3,y,'0',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar_big(x+(size/4)*t-3,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用19字体
void OLED_ShowString_big(u8 x,u8 y,const u8 *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=33;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar_big(x,y,*p,66,1);	 
        x+=16;
        p++;
    }  
}	   
void OLED_ShowChar_select(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	chr=chr-' ';//得到偏移后的值				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1608[chr][t];  //调用1206字体
		else temp=oled_asc2_1608[chr][t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,!mode);
			else OLED_DrawPoint(x,y,mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
////在指定位置显示一个字符,包括部分字符
////x:0~127
////y:0~63
////mode:0,反白显示;1,正常显示				 
////size:选择字体 16/12 
#define Image_ADDR  ((vu32*)(40*1024+1024*11+11+4*9));//((vu32*)0xAC25);
 
void OLED_ShowPic(u8 x,u8 y,u8 size,u8 mode)
{      			   
	vu8 *p_address;
	u8 tem=0,t1=0;u16 t;
	u8 x0=x;		
	p_address=(u8*)Image_ADDR;
    for(t=0;t<513;t++)
    {
			switch (size)
			{
				case 1:
				{
					tem=*(p_address+t);
//					tem=gImage_v1[t];	
					break;
				}
				case 2:
				{	
					tem=*(p_address+512+t);
//					tem=gImage_v2[t];	
					break;
				} 
				case 3:
				{tem=*(p_address+512*2+t);
//					tem=gImage_v3[t];	
					break;
				}
				case 4:
				{tem=*(p_address+512*3+t);
//					tem=gImage_v4[t];	
					break;
				}
				case 5:
				{	tem=*(p_address+512*4+t);
//					tem=gImage_v5[t];	
					break;
				} 
				case 6:
				{
					tem=*(p_address+512*5+t);
//					tem=gImage_v6[t];	
					break;
				}				
				case 7:
				{
					tem=*(p_address+512*6+t);
//					tem=gImage_v7[t];	
					break;
				}
				case 8:
				{	tem=*(p_address+512*7+t);
//					tem=gImage_v8[t];	
					break;
				} 
				case 12:
				{
					tem=*(p_address+512*8+t);
//					tem=gImage_v12[t];
					break;
				}

				case 14:
				{
					tem=gImage_pc[t];	
					break;
				}
				case 15:
				{
					tem=gImage_bat[t];	
					break;
				}
					case 16:
				{
					tem=gImage_51[t];	
					break;
				}
				case 17:
				{
					tem=gImage_52[t];	
					break;
				}
				case 18:
				{
					tem=gImage_53[t];	
					break;
				}
				case 19:
				{
					tem=gImage_54[t];	
					break;
				}
				case 20:
				{
					tem=gImage_55[t];	
					break;
				}
				case 21:
				{
					tem=gImage_1[t];	
					break;
				}
				case 22:
				{
					tem=gImage_31[t];	
					break;
				}
				case 23:
				{
					tem=gImage_32[t];	
					break;
				}
				case 24:
				{
					tem=gImage_33[t];	
					break;
				}
				case 25:
				{
					tem=gImage_61[t];	
					break;
				}
				case 26:
				{
					tem=gImage_62[t];	
					break;
				}
				case 27:
				{
					tem=gImage_63[t];	
					break;
				}
				case 28:
				{
					tem=gImage_64[t];	
					break;
				}
				case 29:
				{
					tem=gImage_65[t];	
					break;
				}
				case 30:
				{
					tem=gImage_66[t];	
					break;
				}
				default:	break;
			}
			for(t1=0;t1<8;t1++)
			{
				if(tem&0x80)
					OLED_DrawPoint(x,y,mode);
				else 
					OLED_DrawPoint(x,y,!mode);
				tem<<=1;
				x++;
				if((x-x0)==128)
				{
					x=x0;
					y++;
					break;
				}
			}  	 
    }          
}
////在指定位置显示一个字符,包括部分字符
////x:0~127
////y:0~63
////mode:0,反白显示;1,正常显示				 
////size:选择字体 16/12 
void OLED_ShowPic_dian6(u8 x,u8 y,u8 size,u8 mode)
{      			    
	u8 tem=0,t1;u16 t;
	u8 x0=x;		
    for(t=0;t<25;t++)
    {   
			switch (size)
			{
				case 1:
				{
					tem=gImage_61[t];	
					break;
				}
				default:	break;
			}
			for(t1=0;t1<8;t1++)
			{
				if(tem&0x80)
					OLED_DrawPoint(x,y,mode);
				else 
					OLED_DrawPoint(x,y,!mode);
				tem<<=1;
				x++;
				if((x-x0)==49)
				{
					x=x0;
					y++;
					break;
				}
			}  	 
    }          
}

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,'0',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void OLED_ShowString(u8 x,u8 y,const u8 *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}	   
void OLED_ShowString_select(u8 x,u8 y,const u8 *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar_select(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}	   

