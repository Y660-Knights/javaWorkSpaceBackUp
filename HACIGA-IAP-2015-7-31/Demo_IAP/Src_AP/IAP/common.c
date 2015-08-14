
#include "common.h"

/*********************************************************************************************************//**
  * @brief  Ascii2Hex.
  * @param  TBD
  * @retval None
  ***********************************************************************************************************/
u32 Ascii2Hex(u8 *pStr, u8 size)
{
  const u32 TenPower[10] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
  u32 val = 0;
  if(size > 0 && size <= 10)
  {
    u8 i;    
    for(i=0;i<size;i++)
    {
      val += (u32)(pStr[i] - '0') * TenPower[size - i - 1];
    }
  }
  return val;
}

/*********************************************************************************************************//**
  * @brief  HexAscii2Hex.
  * @param  TBD
  * @retval None
  ***********************************************************************************************************/
u32 HexAscii2Hex(char *pStr, u8 size)
{
  u32 val = 0;
  u8 i;    
  for(i=0;i<size;i++)
  {
    char c = pStr[i];
    if(c >= 'A')
    {
      val |= (u32)((pStr[i] - 'A') + 10) << ((size-i-1)*4);
    }
    else
    {
      val |= (u32)(pStr[i] - '0') << ((size-i-1)*4);
    }
  }

  return val;
}

/*********************************************************************************************************//**
  * @brief  hex data to bcd data.
  * @param  u32 hex_data
  * @param  digit
  * @retval bcd data
  ***********************************************************************************************************/
u32 Hex2Bcd(u32 hex)
{
  u32 bcd = 0;
  u8 digit = 8;
  u32 div = 10000000;  
  while(digit--)
  {
    bcd <<= 4;
    bcd |= (hex / div);
    hex %= div;
    div /= 10;
  }  
	return bcd;
}

/*********************************************************************************************************//**
  * @brief  MemCpy.
  * @param  TBD
  * @retval None
  ***********************************************************************************************************/
bool MemCpy(void* pDes, void* pSrc, u16 size)
{
  u8* des = (u8*)pDes;
  u8* src = (u8*)pSrc;
  
  while(size--)
  {
    *des = *src;
    des++;
    src++;
  }
  return TRUE;
}

/*********************************************************************************************************//**
  * @brief  MemCmp.
  * @param  TBD
  * @retval None
  ***********************************************************************************************************/
bool MemCmp(void* pSrc, void* pCmp, int size)
{
  u8* cmp = (u8*)pCmp;
  u8* src = (u8*)pSrc;
 
  while(size--)
  {
    if(*src != *cmp)
    {
      return FALSE;
    }
    src++;
    cmp++;
  }
  return TRUE;
}

/*********************************************************************************************************//**
  * @brief  StrLen.
  * @param  TBD
  * @retval None
  ***********************************************************************************************************/
int StrLen(STR_T pStr)
{
  int Len = 0;
  while(pStr[Len] != 0)
  {
    Len++;
    if(Len > STR_MAX_LEN)
    {
      break;
    }
  }
  return Len;
}

/*********************************************************************************************************//**
  * @brief  StrCmp
  * @param  Src
  * @param  Cmp
  * @retval None
  ***********************************************************************************************************/
bool StrCmp(STR_T Src, STR_T Cmp)
{
  int len = StrLen(Cmp);

  if(len > STR_MAX_LEN)
    return FALSE;
    
  return MemCmp(Src, Cmp, len+1);
}

/*********************************************************************************************************//**
  * @brief  MemCmpStr
  * @param  TBD
  * @retval None
  ***********************************************************************************************************/
bool MemCmpStr(char* Src, char* Cmp)
{
  int len = StrLen(Cmp);
  if(len == 0 || len > STR_MAX_LEN)
    return FALSE;
    
  return MemCmp(Src, Cmp, len);
}

/*********************************************************************************************************//**
  * @brief  MemCpyStr
  * @param  TBD
  * @retval None
  ***********************************************************************************************************/
bool MemCpyStr(char* pDes, char* pSrc)
{
  int len = StrLen(pSrc);
  if(len > STR_MAX_LEN)
    return FALSE;
    
  return MemCpy(pDes, pSrc, len);
}

/*********************************************************************************************************//**
  * @brief  StrCpy.
  * @param  TBD
  * @retval None
  ***********************************************************************************************************/
bool StrCpy(char* pDes, char* pSrc)
{
  int len = StrLen(pSrc);

  if(len > STR_MAX_LEN)
    return FALSE;

  return MemCpy(pDes,pSrc,len+1);
}

/*********************************************************************************************************//**
  * @brief  StrSplitBySign.
  * @param  TBD
  * @retval None
  ***********************************************************************************************************/
u8 StrSplitBySign(u8 max, STR_T pStr, char sign, u8 *pPos)
{
  int len,index;
  u8 size;    
  
  len = StrLen(pStr);
  
  if(len == 0 || len > STR_MAX_LEN)
    return 0;
  
  size = 0;
  index = 0;
  while(len--)
  {
    if(pStr[index] == sign)
    {
      pPos[size++] = index;
      if(size == max)
      {
        return size;
      }
    }
    index++;
  }
  pPos[size++] = index;
  return size;
}

/*********************************************************************************************************//**
  * @brief  RoughDelayXuS.
  * @param  TBD
  * @retval None
  ***********************************************************************************************************/
#pragma push
#pragma Ospace
#pragma O2
#ifndef HCLK
#define HCLK  (72000000)
#endif
void RoughDelayXuS(u32 X)
{
  #if (HCLK > 48000000ul)
  X *= ((HCLK/1000000)/5);
  #elif (HCLK > 24000000ul)
  X *= ((HCLK/1000000)/3);
  #else
  X *= ((HCLK/1000000)/2);
  #endif
  while(X--)
  {
  }
}
#pragma pop





