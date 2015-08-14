/*********************************************************************************************************//**
 * @file    spi_flash.c
 * @version V1.00
 * @date    10/01/2013
 * @brief   This file provides a set of functions needed to manage the
 *          communication between SPI peripheral and SPI MX25L1606E FLASH.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2013 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "spi_flash.h"
#include "spi_flash_mx25l12835f.h"

/* Private typedef -----------------------------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------------------------------------*/
#define SPI_FLASH_SEL_ACTIVE()    GPIO_ClearOutBits(GPIO_PORT[FLASH_SPI_SEL_GPIO_ID],\
																													FLASH_SPI_SEL_GPIO_PIN)
#define SPI_FLASH_SEL_INACTIVE()  GPIO_SetOutBits(GPIO_PORT[FLASH_SPI_SEL_GPIO_ID],\
																													FLASH_SPI_SEL_GPIO_PIN)
bool SPI_FLASH_IsDMA_Busy;

/* Private variables ---------------------------------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
  * @brief  Initializes peripherals used by the SPI flash driver.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
 /*  Enable AFIO & SPI SEL pin port & SPI clock                                                            */
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  FLASH_SPI_SEL_CLK(CKCUClock) = 1;
  FLASH_SPI_CLK(CKCUClock)     = 1;
  CKCUClock.Bit.AFIO           = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
//  /*  Enable AFIO & SPI SEL pin port & SPI clock  */
//  CKCU_APBPerip0ClockConfig(CKCU_APBEN0_AFIO | SPI_PCLK(SPI_FLASH_CH), ENABLE);
//  CKCU_AHBPeripClockConfig(CKCU_AHBEN_PDMA | GPIO_PCLK(SPI_FLASH_SEL_GPIO_PORT_ID), ENABLE);

  /*  Configure SPI SCK pin, SPI MISO pin, SPI MOSI pin                                                     */
  HT32F_DVB_GPxConfig(FLASH_SPI_SCK_GPIO_ID, FLASH_SPI_SCK_AFIO_PIN, FLASH_SPI_SCK_AFIO_MODE);
  HT32F_DVB_GPxConfig(FLASH_SPI_MISO_GPIO_ID, FLASH_SPI_MISO_AFIO_PIN, FLASH_SPI_MISO_AFIO_MODE);
  HT32F_DVB_GPxConfig(FLASH_SPI_MOSI_GPIO_ID, FLASH_SPI_MOSI_AFIO_PIN, FLASH_SPI_MOSI_AFIO_MODE);
  /*  Configure SPI SEL pin                                                                                 */
  GPIO_SetOutBits(GPIO_PORT[FLASH_SPI_SEL_GPIO_ID], FLASH_SPI_SEL_GPIO_PIN);
  GPIO_DirectionConfig(GPIO_PORT[FLASH_SPI_SEL_GPIO_ID], FLASH_SPI_SEL_GPIO_PIN, GPIO_DIR_OUT);

  /* SEL output inactive level */
  SPI_FLASH_SEL_INACTIVE();
  //GPIO_DirectionConfig(GPIO_PORT(SPI_FLASH_SEL_GPIO_PORT_ID), GPIO_PIN(SPI_FLASH_SEL_GPIO_PIN_ID), GPIO_DIR_OUT);   

  /*  SPI Configuration */
  SPI_InitStructure.SPI_Mode = SPI_MASTER;
  SPI_InitStructure.SPI_FIFO = SPI_FIFO_ENABLE;
  SPI_InitStructure.SPI_DataLength = SPI_DATALENGTH_8;
  SPI_InitStructure.SPI_SELMode = SPI_SEL_SOFTWARE;
  SPI_InitStructure.SPI_SELPolarity = SPI_SELPOLARITY_LOW;
  SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_HIGH;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_SECOND;
  SPI_InitStructure.SPI_RxFIFOTriggerLevel = 4;
  SPI_InitStructure.SPI_TxFIFOTriggerLevel = 4;
  SPI_InitStructure.SPI_ClockPrescaler = 4; /* 72/2=36MHz, Normal Read frequency */
  SPI_Init(FLASH_SPI, &SPI_InitStructure);

  SPI_SELOutputCmd(FLASH_SPI, ENABLE);
  SPI_Cmd(FLASH_SPI, ENABLE);
  
  SPI_FLASH_IsDMA_Busy = FALSE;

  #if 0
  {
    SPI_FLASH_ChipErase();  
  }
  #endif
  
  #if 0
  {
    u8 buf1[1024];
    u8 buf[1024];
    int i;
    for(i=0; i < 1024; i++)
    {
      buf1[i] = i + 1;
    }
//    SPI_FLASH_SectorErase(0);  
    SPI_FLASH_SectorErase(1024*1024*10);  
    
    SPI_FLASH_BufferRead(buf, 0, 1024);  
    SPI_FLASH_BufferRead(buf, 1024*1024*10, 1024);
    
//    SPI_FLASH_BufferWrite(buf1, 0, 1024);
    SPI_FLASH_BufferWrite(buf1, 1024*1024*10, 1024);
//    SPI_FLASH_BufferRead(buf, 0, 1024);  
    SPI_FLASH_BufferRead(buf, 1024*1024*10, 1024);

    SPI_FLASH_SectorErase(1024*1024*10);  
    SPI_FLASH_BufferRead(buf, 0, 1024);  
    SPI_FLASH_BufferRead(buf, 1024*1024*10, 1024);
     SPI_FLASH_SectorErase(1024*1024*10);     
    while(1);
  }
  #endif
}

/*********************************************************************************************************//**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();
  /* Send "Sector Erase" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_SE);
  /* Send high byte address of SectorAddr */
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* Send medium byte address of SectorAddr */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* Send low byte address of SectorAddr */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();

  /* Wait the write opertaion has completed */
  SPI_FLASH_WaitForWriteEnd();
}

/*********************************************************************************************************//**
  * @brief  Erases the specified FLASH block.
  * @param  SectorAddr: address of the block to erase.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_BlockErase(u32 BlockAddr)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();
  /* Send "Block Erase" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_BE);
  /* Send high byte address of BlockAddr */
  SPI_FLASH_SendByte((BlockAddr & 0xFF0000) >> 16);
  /* Send medium byte address of BlockAddr */
  SPI_FLASH_SendByte((BlockAddr & 0xFF00) >> 8);
  /* Send low byte address of BlockAddr */
  SPI_FLASH_SendByte(BlockAddr & 0xFF);
  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();

  /* Wait the write opertaion has completed */
  SPI_FLASH_WaitForWriteEnd();
}

/*********************************************************************************************************//**
  * @brief  Erases the entire FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_ChipErase(void)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();
  /* Send "Chip Erase" instruction  */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_CE);
  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();

  /* Wait the write opertaion has completed */
  SPI_FLASH_WaitForWriteEnd();
}

/*********************************************************************************************************//**
  * @brief  Writes block of data to the FLASH with a single WRITE cycle.
  * @param  WriteBuffer : pointer to the buffer  containing the data to be written to the FLASH.
  * @param  WriteAddr : FLASH's internal address to write to.
  * @param  NumOfWriteByte : number of bytes to write to the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_PageWrite(u8* WriteBuffer, u32 WriteAddr, u16 NumOfWriteByte)
{
  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();
  /* Send "Page Program" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_WRITE);
  /* Send high byte address of WriteAddr */
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /* Send medium byte address of WriteAddr */
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /* Send low byte address of WriteAddr */
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  while(NumOfWriteByte--)
  {
    /* Send the current byte */
    SPI_FLASH_SendByte(*WriteBuffer);
    /* Point on the next byte to be written */
    WriteBuffer++;
  }

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*********************************************************************************************************//**
  * @brief  Writes block of data to the FLASH.
  * @param  WriteBuffer : pointer to the buffer  containing the data to be written to the FLASH.
  * @param  WriteAddr : FLASH's internal address to write to.
  * @param  NumOfWriteByte : number of bytes to write to the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_BufferWrite(u8* WriteBuffer, u32 WriteAddr, u32 NumOfWriteByte)
{
  u8 NumOfSingle = 0, Addr = 0, Count = 0, tmp = 0;
  u16 NumOfPage = 0;

  Addr = WriteAddr % SPI_FLASH_PAGESIZE;
  Count = SPI_FLASH_PAGESIZE - Addr;
  NumOfPage =  NumOfWriteByte / SPI_FLASH_PAGESIZE;
  NumOfSingle = NumOfWriteByte % SPI_FLASH_PAGESIZE;

  if(Addr == 0)
  {
    if(NumOfPage == 0)
    {
      SPI_FLASH_PageWrite(WriteBuffer, WriteAddr, NumOfWriteByte);
    }
    else
    {
      while(NumOfPage--)
      {
        SPI_FLASH_PageWrite(WriteBuffer, WriteAddr, SPI_FLASH_PAGESIZE);
        WriteAddr +=  SPI_FLASH_PAGESIZE;
        WriteBuffer += SPI_FLASH_PAGESIZE;
      }
      if(NumOfSingle)
      {
        SPI_FLASH_PageWrite(WriteBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
  else
  {
    if(NumOfPage== 0)
    {
      if(NumOfSingle > Count)
      {
        tmp = NumOfSingle - Count;

        SPI_FLASH_PageWrite(WriteBuffer, WriteAddr, Count);
        WriteAddr +=  Count;  // 2014/09/12 Chijen Fix Bug
        WriteBuffer += Count; // 2014/09/12 Chijen Fix Bug

        SPI_FLASH_PageWrite(WriteBuffer, WriteAddr, tmp);
      }
      else
      {
        SPI_FLASH_PageWrite(WriteBuffer, WriteAddr, NumOfWriteByte);
      }
    }
    else
    {
      NumOfWriteByte -= Count;
      NumOfPage =  NumOfWriteByte / SPI_FLASH_PAGESIZE;
      NumOfSingle = NumOfWriteByte % SPI_FLASH_PAGESIZE;

      SPI_FLASH_PageWrite(WriteBuffer, WriteAddr, Count);
      WriteAddr +=  Count;
      WriteBuffer += Count;

      while(NumOfPage--)
      {
        SPI_FLASH_PageWrite(WriteBuffer, WriteAddr, SPI_FLASH_PAGESIZE);
        WriteAddr +=  SPI_FLASH_PAGESIZE;
        WriteBuffer += SPI_FLASH_PAGESIZE;
      }

      if(NumOfSingle)
      {
        SPI_FLASH_PageWrite(WriteBuffer, WriteAddr, NumOfSingle);
      }
     }
   }
}

/*********************************************************************************************************//**
  * @brief  Reads a block of data from the FLASH.
  * @param  ReadBuffer : pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr : FLASH's internal address to read from.
  * @param  NumOfReadByte : number of bytes to read from the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_BufferRead(u8* ReadBuffer, u32 ReadAddr, u16 NumOfReadByte)
{
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_READ);
  /* Send high byte address of ReadAddr */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send medium byte address of ReadAddr */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send low byte address of ReadAddr */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while(NumOfReadByte--)
  {
    /* Read a byte from the FLASH */
    *ReadBuffer = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
    /* Point to the next location where the byte read will be saved */
    ReadBuffer++;
  }

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}

/*********************************************************************************************************//**
  * @brief  Reads a block of data from the FLASH using Dual Output Mode.
  * @param  ReadBuffer : pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr : FLASH's internal address to read from.
  * @param  NumOfReadByte : number of half words to read from the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_DualOutputBufferRead(u16* ReadBuffer, u32 ReadAddr, u16 NumOfReadByte)
{
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_DREAD);
  /* Send high byte address of ReadAddr */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send medium byte address of ReadAddr */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send low byte address of ReadAddr */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
  /* Send dummy byte */
  SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);

  /* Enable Dual Port */
  SPI_DUALCmd(FLASH_SPI, ENABLE);

  while(NumOfReadByte--)
  {
    /* Read a byte from the FLASH */
    *ReadBuffer = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
    /* Point to the next location where the byte read will be saved */
    ReadBuffer++;
  }

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();

  /* Disable Dual Port */
  SPI_DUALCmd(FLASH_SPI, DISABLE);
}

/*********************************************************************************************************//**
  * @brief  Reads JEDEC ID.
  * @retval JEDEC ID value.
  ***********************************************************************************************************/
u32 SPI_FLASH_ReadJEDECID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "JEDEC Read-ID" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_RDID);

  /* Read a byte from the FLASH */
  Temp0 = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);

  /* Read a byte from the FLASH */
  Temp1 = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);

  /* Read a byte from the FLASH */
  Temp2 = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;

}

/*********************************************************************************************************//**
  * @brief  Writes the new value to the Status Register.
  * @param  Value : the new value to be written to the Status Register.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_WriteStatus(u8 Value)
{
  SPI_FLASH_WriteEnable();

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Write Status Register" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_WRSR);
  /* Writes to the Status Register */
  SPI_FLASH_SendByte(Value);

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}

/*********************************************************************************************************//**
  * @brief  Sends a byte through the SPI interface and return the byte received from the SPI bus.
  * @param  byte : byte to send.
  * @retval The value of the received byte.
  ***********************************************************************************************************/
u16 SPI_FLASH_SendByte(u8 byte)
{
  /* Wait until TX FIFO empty                                                                               */
  while (SPI_GetFIFOStatus(FLASH_SPI, SPI_FIFO_TX) != 0);
 
  /* Clear all RX data                                                                                      */
  while (SPI_GetFIFOStatus(FLASH_SPI, SPI_FIFO_RX) != 0)
  {
    SPI_ReceiveData(FLASH_SPI);
  }
  
  /* Send	byte through the SPI_FLASH_PORT	peripheral */
  SPI_SendData(FLASH_SPI,	byte);
  
  /* Loop	while	Rx is	not	empty	*/
  while (SPI_GetFIFOStatus(FLASH_SPI, SPI_FIFO_RX) == 0);
  
  /* Return	the	byte read	from the SPI */
  return SPI_ReceiveData(FLASH_SPI);
}

/*********************************************************************************************************//**
  * @brief  Enables the write access to the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Write Enable" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_WREN);

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}

/*********************************************************************************************************//**
  * @brief  Disables the write access to the FLASH.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_WriteDisable(void)
{
  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Write Disable" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_WRDI);

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}

/*********************************************************************************************************//**
  * @brief  Polls the status of the BUSY flag in the FLASH's status register and waits for the end of Flash
            writing.
  * @retval None
  ***********************************************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* Select the SPI FLASH */
  SPI_FLASH_SEL_ACTIVE();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(SPI_FLASH_CMD_RDSR);

  /* Loop as long as the busy flag is set */
  do
  {
    /* Send a dummy byte to generate the clock to read the value of the status register */
    FLASH_Status = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);

  } while((FLASH_Status & SPI_FLASH_BUSY_FLAG) == SET);

  /* Deselect the SPI FLASH */
  SPI_FLASH_SEL_INACTIVE();
}


/*********************************************************************************************************//**
	*	@brief	SPI_FLASH_ReadDMA
  * @param  ReadAddr :  
  * @param  DstAddr :  
  * @param  DataSize : 
  * @param  DstAddrMode : 
  *   @arg : SPI_FLASH_DMA_ADR_FIX_MODE
  *   @arg : SPI_FLASH_DMA_ADR_INC_MODE
	*	@retval	None
	***********************************************************************************************************/
void SPI_FLASH_ReadDMA(u32 ReadAddr,u32 DstAddr, u32 DataSize, u8 DstAddrMode)
{
  PDMACH_InitTypeDef PDMACH_InitStructure;
  u32 blkcnt = 0;

  /* Wait until TX FIFO empty                                                                               */
  while (SPI_GetFIFOStatus(FLASH_SPI, SPI_FIFO_TX) > 0);

  SPI_FLASH_SEL_ACTIVE();
  SPI_SendData(FLASH_SPI, SPI_FLASH_CMD_DREAD);    
  SPI_SendData(FLASH_SPI, (ReadAddr & 0xFF0000) >> 16);  
  SPI_SendData(FLASH_SPI, (ReadAddr& 0xFF00) >> 8);  
  SPI_SendData(FLASH_SPI, ReadAddr & 0xFF);

  SPI_SendData(FLASH_SPI, SPI_FLASH_DUMMY_BYTE);  
  
  while (SPI_GetFIFOStatus(FLASH_SPI, SPI_FIFO_TX) != 0);
  while(SPI_GetFIFOStatus(FLASH_SPI, SPI_FIFO_RX))
  {
    FLASH_SPI->DR;
  } 
  
  /* DMA Setting: BlkLen=4, DataSize=2Byte */
  SPI_DUALCmd(FLASH_SPI, ENABLE);
  blkcnt = DataSize / 8;  
	if ( (DataSize % 8) != 0)
	  blkcnt += 1;
  
  SPI_SendData(FLASH_SPI, SPI_FLASH_DUMMY_BYTE);  
  SPI_SendData(FLASH_SPI, SPI_FLASH_DUMMY_BYTE);  
  SPI_SendData(FLASH_SPI, SPI_FLASH_DUMMY_BYTE);  
  SPI_SendData(FLASH_SPI, SPI_FLASH_DUMMY_BYTE);  
  SPI_SendData(FLASH_SPI, SPI_FLASH_DUMMY_BYTE);  
  SPI_SendData(FLASH_SPI, SPI_FLASH_DUMMY_BYTE);  
  SPI_SendData(FLASH_SPI, SPI_FLASH_DUMMY_BYTE);  
  
  /* SPIx Tx  PDMA_SPI0_TX */  
  PDMACH_InitStructure.PDMACH_SrcAddr = 0; 
  PDMACH_InitStructure.PDMACH_DstAddr = (u32)&FLASH_SPI->DR;
  PDMACH_InitStructure.PDMACH_BlkCnt = blkcnt - 1;
  PDMACH_InitStructure.PDMACH_BlkLen = 4;
  PDMACH_InitStructure.PDMACH_DataSize = WIDTH_16BIT;
  PDMACH_InitStructure.PDMACH_Priority = L_PRIO;
  PDMACH_InitStructure.PDMACH_AdrMod  = SRC_ADR_FIX | DST_ADR_FIX;
  PDMA_Config(SPI_FLASH_PORT_TX_DMA_CH, &PDMACH_InitStructure); 
  
  /* SPIx Rx */
  PDMACH_InitStructure.PDMACH_SrcAddr = (u32)&FLASH_SPI->DR; 
  PDMACH_InitStructure.PDMACH_BlkCnt = blkcnt;
  PDMACH_InitStructure.PDMACH_DstAddr = (u32)DstAddr;
  PDMACH_InitStructure.PDMACH_Priority = H_PRIO;
  if(DstAddrMode == SPI_FLASH_DMA_ADR_INC_MODE)
  {
    PDMACH_InitStructure.PDMACH_AdrMod  = SRC_ADR_FIX | DST_ADR_LIN_INC;
  }
  PDMA_Config(SPI_FLASH_PORT_RX_DMA_CH, &PDMACH_InitStructure); 
 
  PDMA_IntConig(SPI_FLASH_PORT_TX_DMA_CH, PDMA_INT_TC | PDMA_INT_GE, ENABLE);
  PDMA_IntConig(SPI_FLASH_PORT_RX_DMA_CH, PDMA_INT_TC | PDMA_INT_GE, ENABLE);
  PDMA_EnaCmd(SPI_FLASH_PORT_RX_DMA_CH, ENABLE);
  PDMA_EnaCmd(SPI_FLASH_PORT_TX_DMA_CH, ENABLE); 
  
  SPI_PDMACmd(FLASH_SPI, SPI_PDMAREQ_TX | SPI_PDMAREQ_RX, ENABLE);
  
  SPI_FLASH_IsDMA_Busy = TRUE;
}

/*********************************************************************************************************//**
 * @brief  : WaitsSPI_FLASH_DMA_Finished
 * @param  :  
 * @param  : 
 * @retval None 
 ************************************************************************************************************/
void SPI_FLASH_WaitDMA_Finished(void)
{
  while(SPI_FLASH_CheckDMA_Finished() == FALSE);
}

/*********************************************************************************************************//**
 * @brief  : SPI_FLASH_CheckDMA_Finished
 * @param  :  
 * @param  : 
 * @retval None 
 ************************************************************************************************************/
bool SPI_FLASH_CheckDMA_Finished(void)
{
  if(SPI_FLASH_IsDMA_Busy)
  {
    if(PDMA_GetFlagStatus(SPI_FLASH_PORT_RX_DMA_CH, PDMA_FLAG_TC))
    {
      PDMA_ClearFlag(SPI_FLASH_PORT_RX_DMA_CH, PDMA_FLAG_TC);
      SPI_FLASH_SEL_INACTIVE();  
      SPI_PDMACmd(FLASH_SPI, SPI_PDMAREQ_TX | SPI_PDMAREQ_RX, DISABLE);
      SPI_DUALCmd(FLASH_SPI, DISABLE);
      SPI_FLASH_IsDMA_Busy = FALSE;
    }
  }
  return (bool)(!SPI_FLASH_IsDMA_Busy);
}
