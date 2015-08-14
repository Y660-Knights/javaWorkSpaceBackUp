/*********************************************************************************************************//**
 * @file    spi_flash_SST25VF016B.h
 * @version $Rev:: 307          $
 * @date    $Date:: 2014-12-31 #$
 * @brief   The header file of SST25V016B.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SPI_FLASH_SST25V016B_H
#define __SPI_FLASH_SST25V016B_H

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


/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup SPI_FLASH_AUTO_Exported_Constants SPI_FLASH_AUTO exported constants
  * @{
  */
#define SST25V016B_CMD_JEDECID           (0x9F)  /*!< Read JEDEC ID instruction                             */

#define SST25V016B_CMD_CE                (0x60)  /*!< Chip Erase instruction                                */
#define SST25V016B_CMD_SE                (0x20)  /*!< Sector Erase instruction                              */
#define SST25V016B_CMD_READ              (0x0B)  /*!< Read Data instruction                                 */
#define SST25V016B_CMD_WRSR              (0x01)  /*!< Write Status Register instruction                     */
#define SST25V016B_CMD_WREN              (0x06)  /*!< Write enable instruction                              */
#define SST25V016B_CMD_WRDI              (0x04)  /*!< Write disable instruction                             */
#define SST25V016B_CMD_RDSR              (0x05)  /*!< Read Status Register instruction                      */

#define SST25V016B_CMD_BP                (0x02)  /*!< Program one data byte instruction                     */
#define SST25V016B_CMD_AAI               (0xAD)  /*!< Auto Address Increment Program instruction            */

#define SST25V016B_JEDEC_ID              (0xBF2541)
#define SST25V016B_JEDEC_IDMASK          (0xFFFFFF)
/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup SPI_FLASH_AUTO_Exported_Functions SPI_FLASH_AUTO exported functions
  * @{
  */
void SST25VF016B_ByteWrite(u8 DataByte, u32 WriteAddr);
void SST25VF016B_AAIWrite(u8* WriteBuffer, u32 WriteAddr, u32 NumOfWriteByte);
void SST25VF016B_BufferWrite(u8* WriteBuffer, u32 WriteAddr, u32 NumOfWriteByte);
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

#endif /* __SPI_FLASH_SST25V016B_H -------------------------------------------------------------------------*/
