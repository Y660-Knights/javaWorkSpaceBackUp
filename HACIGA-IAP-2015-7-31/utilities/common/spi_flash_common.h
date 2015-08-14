/*********************************************************************************************************//**
 * @file    spi_flash_common.h
 * @version $Rev:: 307          $
 * @date    $Date:: 2014-12-31 #$
 * @brief   The header file of common function for SPI Flash.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SPI_FLASH_COMMON_H
#define __SPI_FLASH_COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @addtogroup SPI_FLASH_AUTO SPI_FLASH_AUTO
  * @brief The SPI Flash Driver with auto detection function.
  * @{
  */


/* Settings ------------------------------------------------------------------------------------------------*/
/** @defgroup SPI_FLASH_AUTO_Settings SPI_FLASH_AUTO settings
  * @{
  */
#define SPI_FALSH_NUM                   (2)
#define SPI_READ_CMD_NUM                (1)
/**
  * @}
  */

/* Exported variables --------------------------------------------------------------------------------------*/
/** @defgroup SPI_FLASH_AUTO_Exported_Variables SPI_FLASH_AUTO exported variables
  * @{
  */
extern s32 sSPI_FLASH_Index;
extern s32 sSPI_FLASH_Size;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup SPI_FLASH_AUTO_Exported_Functions SPI_FLASH_AUTO exported functions
  * @{
  */
u32  SPI_FLASH_ReadJEDECID_0(void);
void SPI_FLASH_ChipErase_0(void);
void SPI_FLASH_SectorErase_0(u32 SectorAddr);
void SPI_FLASH_BufferWrite_0(u8* WriteBuffer, u32 WriteAddr, u32 NumOfWriteByte);
void SPI_FLASH_BufferRead_0(u8* ReadBuffer, u32 ReadAddr, u32 NumOfReadByte);
void SPI_FLASH_BufferDualRead_0(u16* ReadBuffer, u32 ReadAddr, u32 NumOfReadByte);
void SPI_FLASH_WriteStatus_0(u8 Value);
void SPI_FLASH_WriteEnable_0(void);
void SPI_FLASH_WriteDisable_0(void);
void SPI_FLASH_WaitForWriteEnd_0(void);
u16  SPI_FLASH_SendByte_0(u8 byte);
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

#ifdef __cplusplus
}
#endif

#endif /* __SPI_FLASH_COMMON_H -----------------------------------------------------------------------------*/
