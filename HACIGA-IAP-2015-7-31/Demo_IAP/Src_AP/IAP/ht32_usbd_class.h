/*********************************************************************************************************//**
 * @file    USBD/GK_LQFP100_STD/Src_AP/ht32_usbd_class.h
 * @version V1.00
 * @date    6/10/2013
 * @brief   The header file of USB Device Class.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2012 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32_USBD_CLASS_H
#define __HT32_USBD_CLASS_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32_usbd_core.h"

/* Settings ------------------------------------------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------------------------------------*/

/* For ht32_usbd_descriptor.c                                                                               */
#define CLASS_INF_CLASS                   (DESC_CLASS_03_HID)
#define CLASS_INF_SUBCLASS                (HID_SUBCLASS_00_NONE)
#define CLASS_INF_PTCO                    (HID_PROTOCOL_00_NONE)

/* HID related definition                                                                                   */
#define DESC_LEN_HID                      ((u32)(9))
#define DESC_LEN_RPOT                     ((u16)(34))

#define DESC_TYPE_21_HID                  (0x21)
#define DESC_TYPE_22_RPOT                 (0x22)
#define DESC_TYPE_23_PHY                  (0x23)

#define HID_SUBCLASS_00_NONE              (0x00)
#define HID_SUBCLASS_01_BOOT              (0x01)

#define HID_PROTOCOL_00_NONE              (0x00)
#define HID_PROTOCOL_01_KEYBOARD          (0x01)
#define HID_PROTOCOL_02_MOUSE             (0x02)

/* Exported types ------------------------------------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
void USBDClass_Init(USBDCore_Class_TypeDef *pClass);

#endif /* __HT32_USBD_CLASS_H ------------------------------------------------------------------------------*/
