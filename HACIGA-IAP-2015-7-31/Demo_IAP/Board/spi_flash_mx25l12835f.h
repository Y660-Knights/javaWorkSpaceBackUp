/*********************************************************************************************************//**
 * @file    spi_flash_mx25l12835f.h
 * @version V1.00
 * @date    12/10/2013
 * @brief   The header file of spi_flash.c module.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2013 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SPI_FLASH_MX25L12835F_H
#define __SPI_FLASH_MX25L12835F_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------------------------------------*/

/** @defgroup SPI_FLASH_Exported_Constants SPI FLASH exported constants
  * @{
  */
#define SPI_FLASH_CMD_WRITE             0x02  /* Program the selected page instruction */
#define SPI_FLASH_CMD_WRSR              0x01  /* Write Status Register instruction */
#define SPI_FLASH_CMD_WREN              0x06  /* Write enable instruction */
#define SPI_FLASH_CMD_WRDI              0x04  /* Write disable instruction */

#define SPI_FLASH_CMD_READ              0x03  /* Read Data instruction */
#define SPI_FLASH_CMD_FREAD             0x0B  /* Read Data at high speed instruction */
#define SPI_FLASH_CMD_DREAD             0x3B  /* Dual Output Mode Read instruction */
#define SPI_FLASH_CMD_RDSR              0x05  /* Read Status Register instruction  */
#define SPI_FLASH_CMD_RDID              0x9F  /* Read JEDEC ID instruction  */
#define SPI_FLASH_CMD_REMS              0x90  /* Read electronic manufacturer & device ID instruction  */
#define SPI_FLASH_CMD_SE                0xD8  /* (64KB BE replace)Sector Erase instruction */
#define SPI_FLASH_CMD_BE                0xD8  /* (64KB BE replace)Block Erase instruction */
#define SPI_FLASH_CMD_CE                0x60  /* Chip Erase instruction */

#define SPI_FLASH_BUSY_FLAG             0x01  /* Write operation in progress */

#define SPI_FLASH_DUMMY_BYTE            0x00

#if (SPI_FLASH_CH == 0)
#define SPI_FLASH_PORT_TX_DMA_CH        PDMA_SPI0_TX
#define SPI_FLASH_PORT_RX_DMA_CH        PDMA_SPI0_RX
#else
#define SPI_FLASH_PORT_TX_DMA_CH        PDMA_SPI1_TX
#define SPI_FLASH_PORT_RX_DMA_CH        PDMA_SPI1_RX
#endif

/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __SPI_FLASH_MX25L12835F_H ------------------------------------------------------------------------*/
