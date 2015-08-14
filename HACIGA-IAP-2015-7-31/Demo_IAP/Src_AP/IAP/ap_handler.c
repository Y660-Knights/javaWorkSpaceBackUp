/*********************************************************************************************************//**
 * @file    USBD/STD_GK/Src_AP/ap_handler.c
 * @version V1.00
 * @date    11/09/2012
 * @brief   This file contains AP handler.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"

#include "ht32_usbd_core.h"

#include "ap_handler.h"
#include "ap_config.h"
#include "ap_buffer.h"
#include "ap_crc16.h"

/* Private typedef -----------------------------------------------------------------------------------------*/
typedef struct {
  vu32 MDID;                             /* offset  0x180 Flash Manufacturer and Device ID Register (MDID)  */
  vu32 PNSR;                             /* offset  0x184 Flash Page Number Status Register (PNSR)          */
  vu32 PSSR;                             /* offset  0x188 Flash Page Size Status Register (PSSR)            */
} FMCInfo_TypeDef;

typedef u32 (*pFunction)();

/* Private constants ---------------------------------------------------------------------------------------*/
#define __DBG_IAPPrintf(...)

#if (AP_DEBUG_MODE == 1)
  #undef __DBG_APPrintf
  #define __DBG_APPrintf printf
  static _AP_DumpData(u32 uSatrt, u32 uLength);
  #warning "================================= AP Debug Mode Warning ================================"
  #warning " 1. AP debug mode has been enable which degrade the performance.                        "
  #warning " 2. Note that print debug message too much may cause the AP command not synchronized.   "
  #warning " 4. After all debug operation is finished, please remember to turn off AP debug mode.   "
  #warning "========================================================================================"
#endif

#define MAX_CMD_LEN             (64)
#define MAX_TOKENS              (3)
#define CMD_COUNT               (6)

#define CMD_SUCCESS             ('O')
#define CMD_FAILED              ('F')

#define FMCINFO_BASE            (0x40080180)
#define FMCINFO                 ((FMCInfo_TypeDef*) FMCINFO_BASE)

#define FLASH_CMD_PROGRAM       ((u32)0x00000004)
#define FLASH_SEND_MAIN         ((u32)0x00000014)

/* Private function prototypes -----------------------------------------------------------------------------*/
static u32 _AP_CMD0(u32 type, u32 saddr, u32 eaddr);

/* Private macro -------------------------------------------------------------------------------------------*/
#define WriteByte(data)         Buffer_WriteByte(&gTx, data)
#define ReadByte(data)          Buffer_ReadByte(&gRx, data)

/* Global variables ----------------------------------------------------------------------------------------*/
Buffer_TypeDef gTx;
Buffer_TypeDef gRx;

/* Private variables ---------------------------------------------------------------------------------------*/
__ALIGN4 static u8 guRxBuffer[RX_BUFFER_SIZE];
__ALIGN4 static u8 guTxBuffer[TX_BUFFER_SIZE];
__ALIGN4 static u8 gu8CmdBuffer[MAX_CMD_LEN];
static u32 u32BufferIndex;

static const pFunction pFComHandlerTable[CMD_COUNT] =
{
  (pFunction)_AP_CMD0,
  (pFunction)_AP_CMD0,
  (pFunction)_AP_CMD0,
  (pFunction)_AP_CMD0,
  (pFunction)AP_Reset,
  (pFunction)_AP_CMD0
};

/* Global functions ----------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
  * @brief  AP mode initialization.
  * @retval None
  ***********************************************************************************************************/
void AP_Init(void)
{
  Buffer_Init(&gTx, guTxBuffer, sizeof(guTxBuffer));
  Buffer_Init(&gRx, guRxBuffer, sizeof(guRxBuffer));
}

/*********************************************************************************************************//**
  * @brief  AP mode handler.
  * @retval None
  ***********************************************************************************************************/
void AP_Handler(void)
{
  u32 u32Parameter[MAX_TOKENS];
  u32 u32CommandExecuteResult;
  u16 crc;
  u16 crcValue;

  {
    u32BufferIndex = 0;
    while (u32BufferIndex < MAX_CMD_LEN)
    {
      ReadByte(&gu8CmdBuffer[u32BufferIndex++]);
    }
    /*------------------------------------------------------------------------------------------------------*/
    /* Check CRC value of command packet                                                                    */
    /*------------------------------------------------------------------------------------------------------*/
    crc = gu8CmdBuffer[2] | ((u16)gu8CmdBuffer[3] << 8);
    gu8CmdBuffer[2] = gu8CmdBuffer[3] = 0;
    crcValue = CRC16(0, (u8 *)(&gu8CmdBuffer[0]), 64);

    /*------------------------------------------------------------------------------------------------------*/
    /* Check command is valid and CRC is correct                                                            */
    /*------------------------------------------------------------------------------------------------------*/
    if (gu8CmdBuffer[0] > CMD_COUNT || crc != crcValue)
    {
      /*----------------------------------------------------------------------------------------------------*/
      /* Command invalid or CRC error. Return 'F' and clear command buffer                                  */
      /*----------------------------------------------------------------------------------------------------*/
      u32CommandExecuteResult = CMD_FAILED;
      Buffer_Discard(&gRx);
    }
    else
    {
      /*----------------------------------------------------------------------------------------------------*/
      /* Command is OK, stop SysTick to prevent timeout reset                                               */
      /*----------------------------------------------------------------------------------------------------*/
      SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
      /*----------------------------------------------------------------------------------------------------*/
      /* Prepare parameter and execution command                                                            */
      /*----------------------------------------------------------------------------------------------------*/
      u32Parameter[0] = gu8CmdBuffer[1];
      u32Parameter[1] = *((u32 *)(&(gu8CmdBuffer[4])));
      u32Parameter[2] = *((u32 *)(&(gu8CmdBuffer[8])));

      u32CommandExecuteResult = (*pFComHandlerTable[gu8CmdBuffer[0]])(u32Parameter[0],
                                                                      u32Parameter[1],
                                                                      u32Parameter[2]
                                                                      );
    }

    /*------------------------------------------------------------------------------------------------------*/
    /* Send Result to Host                                                                                  */
    /*------------------------------------------------------------------------------------------------------*/
    WriteByte(u32CommandExecuteResult);

  }
}

/*********************************************************************************************************//**
  * @brief  Reset Command.
  * @param  uMode: Mode after reset
  * @retval FALSE or TRUE
  ***********************************************************************************************************/
u32 AP_Reset(u32 uMode)
{
  /*--------------------------------------------------------------------------------------------------------*/
  /* Disconnect USB and wait for PC aware it                                                                */
  /*--------------------------------------------------------------------------------------------------------*/
  NVIC_DisableIRQ(USB_IRQn);
  HT32F_DVB_USBDisConnect();//BOARD_USBDisConnect();
  HT_FLASH->SBVT[1] = BOOT_MODE_IAP;
  //BOARD_DelayXuS(30000);

  NVIC_SystemReset();

  return TRUE;
}

/* Private functions ---------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  _AP_CMD0.
  * @param  type: TBD
  *         @arg TBD
  *         @arg TBD
  * @param  saddr: Start address
  * @param  eaddr: End address
  * @retval CMD_SUCCESS or CMD_FAILED
  ***********************************************************************************************************/
static u32 _AP_CMD0(u32 type, u32 saddr, u32 eaddr)
{
  //return CMD_FAILED;
  return CMD_SUCCESS;
}
