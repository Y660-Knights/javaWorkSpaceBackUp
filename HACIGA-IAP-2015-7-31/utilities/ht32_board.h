/*********************************************************************************************************//**
 * @file    ht32_board.h
 * @version $Rev:: 343          $
 * @date    $Date:: 2015-03-07 #$
 * @brief   HT32 target board definition file.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32_BOARD_H
#define __HT32_BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#ifdef USE_HT32F1755_2755_DVB
  #include "HT32_board/ht32f1765_dvb.h"
#elif defined USE_HT32F1656_DVB
  #include "HT32_board/ht32f1656_dvb.h"
#elif defined USE_HT32F1654_DVB
  #include "HT32_board/ht32f1654_dvb.h"
#elif defined USE_HT32F1765_SK
  #include "HT32_board/ht32f1765_sk.h"
#elif defined USE_HT32F1656_SK
  #include "HT32_board/ht32f1656_sk.h"
#elif defined USE_HT32F1654_SK
  #include "HT32_board/ht32f1654_sk.h"
#else
 #error "Please select board by define USE_HT32FXXXX_XXX in project."
#endif

#include "common/i2c_eeprom.h"
//#include "common/spi_flash.h"
#include "common/ebi_lcd.h"
#include "spi_flash.h"
#ifdef __cplusplus
}
#endif

#endif /* __HT32_BOARD_H                                                                                    */
