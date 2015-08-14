/*********************************************************************************************************//**
 * @file    ht32_board.c
 * @version $Rev:: 343          $
 * @date    $Date:: 2015-03-07 #$
 * @brief   HT32 target board related file.
 *************************************************************************************************************
 *
 * <h2><center > Copyright (C) 2014 Holtek. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#ifdef USE_HT32F1755_2755_DVB
  #include "HT32_board/ht32f1765_dvb.c"
#elif defined USE_HT32F1656_DVB
  #include "HT32_board/ht32f1656_dvb.c"
#elif defined USE_HT32F1656_SK
  #include "HT32_board/ht32f1656_sk.c"
#elif defined USE_HT32F1654_DVB
  #include "HT32_board/ht32f1654_dvb.c"
#elif defined USE_HT32F1654_SK
  #include "HT32_board/ht32f1654_sk.c"
#else
 #error "Please select board by define USE_HT32FXXXX_XXX in project."
#endif
