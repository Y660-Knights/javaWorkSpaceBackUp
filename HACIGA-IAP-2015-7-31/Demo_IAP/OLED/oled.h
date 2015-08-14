#include "ht32.h"
#include "ht32_board.h"


	
void  ssd1306(void);
void write_i(unsigned char  ins);
void write_d(unsigned char  dat);
void OLED_Refresh_Gram(void);
void OLED_Clear(void);
void delay(u32 i);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_delect(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);  
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowChar_select(u8 x,u8 y,u8 chr,u8 size,u8 mode);
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);
void OLED_ShowChar_big(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum_big(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString_big(u8 x,u8 y,const u8 *p);
void OLED_ShowString_select(u8 x,u8 y,const u8 *p);
void OLED_ShowPic(u8 x,u8 y,u8 size,u8 mode);
void OLED_ShowPic_dian6(u8 x,u8 y,u8 size,u8 mode);

