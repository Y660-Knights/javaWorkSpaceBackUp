/*********************************************************************************************************//**
 * @file    ht32f1653_54_flash.c
 * @version $Rev:: 307          $
 * @date    $Date:: 2014-12-31 #$
 * @brief   This file provides all the FLASH firmware functions.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f1653_54_flash.h"

/** @addtogroup HT32F1653_54_Peripheral_Driver HT32F1653/1654 Peripheral Driver
  * @{
  */

/** @defgroup FLASH FLASH
  * @brief FLASH driver modules
  * @{
  */


/* Private constants ---------------------------------------------------------------------------------------*/
/** @defgroup FLASH_Private_Define FLASH private definitions
  * @{
  */

/* Delay definition                                                                                         */
#define FLASH_TIMEOUT                 (0x000FFFFF)

/* FLASH OCMR                                                                                               */
#define FLASH_CMD_STADNBY             (0x00000000)
#define FLASH_CMD_PROGRAM             (0x00000004)
#define FLASH_CMD_PAGEERASE           (0x00000008)
#define FLASH_CMD_MASSERASE           (0x0000000A)

/* FLASH OPCR                                                                                               */
#define FLASH_READY                   (0x0000000C)
#define FLASH_SEND_MAIN               (0x00000014)

/* FLASH CFCR                                                                                               */
#define CFCR_WAIT_MASK                (0xFFFFFFF8)

#define FLASH_PREFETCHBUF_ON          (0x00000010)
#define FLASH_PREFETCHBUF_OFF         (0xFFFFFFEF)

#define FLASH_BRANCHCACHE_ON          (0x00001000)
#define FLASH_BRANCHCACHE_OFF         (0xFFFFEFFF)

#define FLASH_DCODECACHE_ON           (0xFFFFFF7F)
#define FLASH_DCODECACHE_OFF          (0x00000080)

#define FLASH_HALFCYCLE_ON            (0x00008000)
#define FLASH_HALFCYCLE_OFF           (0xFFFF7FFF)

#define FLASH_ZWPWRSAVING_ON          (0x00010000)
#define FLASH_ZWPWRSAVING_OFF         (0xFFFEFFFF)
/**
  * @}
  */

/* Private macro -------------------------------------------------------------------------------------------*/
/** @defgroup FLASH_Private_Macro FLASH private macros
  * @{
  */

/**
 * @brief Check parameter of the FLASH wait state.
 */
#define IS_FLASH_WAITSTATE(WAIT)      ((WAIT == FLASH_WAITSTATE_0) || \
                                       (WAIT == FLASH_WAITSTATE_1) || \
                                       (WAIT == FLASH_WAITSTATE_2))

/**
 * @brief Check parameter of the FLASH vector mapping.
 */
#define IS_FLASH_VECTOR_MODE(MODE)    ((MODE == FLASH_BOOT_LOADER) || \
                                       (MODE == FLASH_BOOT_SRAM)   || \
                                       (MODE == FLASH_BOOT_MAIN))

/**
 * @brief Check parameter of the FLASH address.
 */
#define IS_FLASH_ADDRESS(ADDRESS)     (ADDRESS < 0x20000000)  /* Code 0.5GB Area                            */

/**
 * @brief Check parameter of the FLASH interrupt status.
 */
#define IS_FLASH_WC_FLAG(FLAG)        ((FLAG & 0x0000001F) != 0)

/**
 * @brief Check parameter of the FLASH interrupt flag.
 */
#define IS_FLASH_FLAG(FLAG)           ((FLAG & 0x0003001F) != 0)

/**
 * @brief Check parameter of the FLASH interrupt.
 */
#define IS_FLASH_INT(IT)              ((IT & 0x0000001F) != 0)

/**
  * @}
  */

/* Global functions ----------------------------------------------------------------------------------------*/
/** @defgroup FLASH_Exported_Functions FLASH exported functions
  * @{
  */
/*********************************************************************************************************//**
 * @brief Configure the FLASH wait state.
 * @param FLASH_WaitState: Setting of FLASH wait state.
 *        This parameter can be:
 *        @arg FLASH_WAITSTATE_0: zero wait state.
 *        @arg FLASH_WAITSTATE_1: one wait state.
 *        @arg FLASH_WAITSTATE_2: two wait state.
 * @retval None
 ************************************************************************************************************/
void FLASH_SetWaitState(u32 FLASH_WaitState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_FLASH_WAITSTATE(FLASH_WaitState));

  HT_FLASH->CFCR = (HT_FLASH->CFCR & CFCR_WAIT_MASK) | FLASH_WaitState;
}

/*********************************************************************************************************//**
 * @brief Enable or Disable FLASH pre-fetch buffer.
 * @param NewState: new state of the FLASH pre-fetch buffer.
 *        This parameter can be: ENABLE or DISABLE
 * @retval None
 ************************************************************************************************************/
void FLASH_PrefetchBufferCmd(ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_FLASH->CFCR |= FLASH_PREFETCHBUF_ON;
  }
  else
  {
    HT_FLASH->CFCR &= FLASH_PREFETCHBUF_OFF;
  }
}

/*********************************************************************************************************//**
 * @brief Enable or Disable FLASH DCODE cache mode.
 * @param NewState: new state of the FLASH DCODE cache.
 *        This parameter can be: ENABLE or DISABLE
 * @retval None
 ************************************************************************************************************/
void FLASH_DcodeCacheCmd(ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_FLASH->CFCR &= FLASH_DCODECACHE_ON;
  }
  else
  {
    HT_FLASH->CFCR |= FLASH_DCODECACHE_OFF;
  }
}

/*********************************************************************************************************//**
 * @brief Enable or Disable FLASH branch cache.
 * @param NewState: new state of the FLASH branch cache.
 *        This parameter can be: ENABLE or DISABLE
 * @retval None
 ************************************************************************************************************/
void FLASH_BranchCacheCmd(ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_FLASH->CFCR |= FLASH_BRANCHCACHE_ON;
  }
  else
  {
    HT_FLASH->CFCR &= FLASH_BRANCHCACHE_OFF;
  }
}

/*********************************************************************************************************//**
 * @brief Enable or Disable FLASH half cycle access.
 * @param NewState: new state of the FLASH half cycle access.
 *        This parameter can be: ENABLE or DISABLE
 * @retval ERROR or SUCCESS
 ************************************************************************************************************/
ErrStatus FLASH_FlashHalfCycleCmd(ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if ((HT_FLASH->CFCR & ~CFCR_WAIT_MASK) == FLASH_WAITSTATE_0)
  {
    if (NewState != DISABLE)
    {
      HT_FLASH->CFCR |= FLASH_HALFCYCLE_ON;
    }
    else
    {
      HT_FLASH->CFCR &= FLASH_HALFCYCLE_OFF;
    }

    return SUCCESS;
  }

  return ERROR;
}

/*********************************************************************************************************//**
 * @brief Enable or Disable FLASH zero wait state power saving.
 * @param NewState: new state of the FLASH zero wait state power saving.
 *        This parameter can be: ENABLE or DISABLE
 * @retval ERROR or SUCCESS
 ************************************************************************************************************/
ErrStatus FLASH_FlashZwPwrSavingCmd(ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if ((HT_FLASH->CFCR & ~CFCR_WAIT_MASK) == FLASH_WAITSTATE_0)
  {
    if (NewState != DISABLE)
    {
      HT_FLASH->CFCR |= FLASH_ZWPWRSAVING_ON;
    }
    else
    {
      HT_FLASH->CFCR &= FLASH_ZWPWRSAVING_OFF;
    }

    return SUCCESS;
  }

  return ERROR;
}

/*********************************************************************************************************//**
 * @brief Set vector remapping mode.
 * @param RemapMode: Booting mode.
 *        This parameter can be:
 *        @arg FLASH_BOOT_LOADER: Boot loader mode.
 *        @arg FLASH_BOOT_SRAM  : SRAM booting mode.
 *        @arg FLASH_BOOT_MAIN  : Main FLASH mode.
 * @retval None
 ************************************************************************************************************/
void FLASH_SetRemappingMode(FLASH_Vector RemapMode)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_FLASH_VECTOR_MODE(RemapMode));

  HT_FLASH->VMCR = RemapMode;
}

/*********************************************************************************************************//**
 * @brief Erase a specific FLASH page.
 * @param PageAddress: Address of the erased page.
 * @retval FLASH_COMPLETE, FLASH_TIME_OUT, FLASH_ERR_WRITE_PROTECTED, FLASH_ERR_ADDR_OUT_OF_RANGE
 ************************************************************************************************************/
FLASH_State FLASH_ErasePage(u32 PageAddress)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_FLASH_ADDRESS(PageAddress));

  HT_FLASH->TADR = PageAddress;
  HT_FLASH->OCMR = FLASH_CMD_PAGEERASE;

  return FLASH_WaitForOperationEnd();
}

/*********************************************************************************************************//**
 * @brief Erase FLASH Option Byte page.
 * @retval FLASH_COMPLETE, FLASH_TIME_OUT, FLASH_ERR_WRITE_PROTECTED
 ************************************************************************************************************/
FLASH_State FLASH_EraseOptionByte(void)
{
  HT_FLASH->TADR = OPTION_BYTE_BASE;
  HT_FLASH->OCMR = FLASH_CMD_PAGEERASE;

  return FLASH_WaitForOperationEnd();
}

/*********************************************************************************************************//**
 * @brief Erase the entire FLASH.
 * @retval FLASH_COMPLETE, FLASH_TIME_OUT
 ************************************************************************************************************/
FLASH_State FLASH_MassErase(void)
{
  HT_FLASH->OCMR = FLASH_CMD_MASSERASE;

  return FLASH_WaitForOperationEnd();
}

/*********************************************************************************************************//**
 * @brief Program one word data.
 * @param Address: The specific FLASH address to be programmed.
 * @param Data: The specific FLASH data to be programmed.
 * @retval FLASH_COMPLETE, FLASH_TIME_OUT, FLASH_ERR_WRITE_PROTECTED, FLASH_ERR_ADDR_OUT_OF_RANGE
 ************************************************************************************************************/
FLASH_State FLASH_ProgramWordData(u32 Address, u32 Data)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_FLASH_ADDRESS(Address));

  HT_FLASH->TADR = Address;
  HT_FLASH->WRDR = Data;
  HT_FLASH->OCMR = FLASH_CMD_PROGRAM;

  return FLASH_WaitForOperationEnd();
}

/*********************************************************************************************************//**
 * @brief Program FLASH Option Byte page.
 * @param Option: Struct pointer of Option Bytes
 * @retval  FLASH_COMPLETE
 ************************************************************************************************************/
FLASH_State FLASH_ProgramOptionByte(FLASH_OptionByte *Option)
{
  s32 i;
  u32 CP = ~(Option->MainSecurity | Option->OptionProtect << 1);
  u32 checksum = 0;

  for (i = 3; i >= 0; i--)
  {
    FLASH_ProgramWordData(OB_PP0 + i * 4, ~(Option->WriteProtect[i]));
    checksum += ~(Option->WriteProtect[i]);
  }

  FLASH_ProgramWordData(OB_CP, CP);
  checksum += CP;

  FLASH_ProgramWordData(OB_CHECKSUM, checksum);

  return FLASH_COMPLETE;
}

/*********************************************************************************************************//**
 * @brief Return security status of the FLASH.
 * @param Option: Struct pointer of Option Bytes
 * @retval None
 ************************************************************************************************************/
void FLASH_GetOptionByteStatus(FLASH_OptionByte *Option)
{
  s32 i;
  for (i = 3; i >= 0; i--)
  {
    Option->WriteProtect[i] = ~HT_FLASH->PPSR[i];
  }

  Option->MainSecurity = !(HT_FLASH->CPSR & 1);
  Option->OptionProtect = !((HT_FLASH->CPSR >> 1) & 1);
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the specific FLASH interrupts.
 * @param FLASH_INT: The specific FLASH interrupt.
 *        This parameter can be:
 *        @arg FLASH_INT_ORFIE, FLASH_INT_ITADIE, FLASH_INT_OBEIE, FLASH_INT_IOCMIE,
 *             FLASH_INT_OREIE, FLASH_INT_ALL.
 * @param Cmd: The status of the FLASH interrupt.
 *        This parameter can be:
 *        @arg DISABLE, ENABLE
 * @retval None
 ************************************************************************************************************/
void FLASH_IntConfig(u32 FLASH_INT, ControlStatus Cmd)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_FLASH_INT(FLASH_INT));
  Assert_Param(IS_CONTROL_STATUS(Cmd));

  if (Cmd != DISABLE)
  {
    HT_FLASH->OIER |= FLASH_INT;
  }
  else
  {
    HT_FLASH->OIER &= ~FLASH_INT;
  }
}

/*********************************************************************************************************//**
 * @brief Return flag status of the FLASH interrupt.
 * @param FLASH_FLAG: Flag of the FLASH interrupt.
 *        This parameter can be:
 *        @arg FLASH_FLAG_OREF, FLASH_FLAG_IOCMF, FLASH_FLAG_OBEF, FLASH_FLAG_ITADF, FLASH_FLAG_ORFF,
 *             FLASH_FLAG_PPERF, FLASH_FLAG_RORFF
 * @retval SET or RESET
 ************************************************************************************************************/
FlagStatus FLASH_GetIntStatus(u32 FLASH_FLAG)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_FLASH_FLAG(FLASH_FLAG));

  if ((HT_FLASH->OISR & FLASH_FLAG) != (u32)RESET)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*********************************************************************************************************//**
 * @brief Clear specific interrupt flags of FLASH.
 * @param FLASH_FLAG: interrupt flag of FLASH.
 *        This parameter can be:
 *        @arg FLASH_FLAG_OREF, FLASH_FLAG_IOCMF, FLASH_FLAG_OBEF, FLASH_FLAG_ITADF, FLASH_FLAG_ORFF
 * @retval None
 ************************************************************************************************************/
void FLASH_ClearIntFlag(u32 FLASH_FLAG)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_FLASH_WC_FLAG(FLASH_FLAG));

  HT_FLASH->OISR = FLASH_FLAG;
}

/*********************************************************************************************************//**
 * @brief Wait untill the FLASH operation has finished or time-out has occurred.
 * @retval FLASH_COMPLETE, FLASH_TIME_OUT, FLASH_ERR_WRITE_PROTECTED, FLASH_ERR_ADDR_OUT_OF_RANGE
 ************************************************************************************************************/
FLASH_State FLASH_WaitForOperationEnd(void)
{
  u32 Timeout = FLASH_TIMEOUT;
  u32 Status;

  HT_FLASH->OIER |= (FLASH_INT_ITADIEN);
  HT_FLASH->OPCR = FLASH_SEND_MAIN;

  /* Waits till the FLASH operation has finished or time-out has occurred                                   */
  while (Timeout--)
  {
    if ((HT_FLASH->OPCR & FLASH_READY) == FLASH_READY)
    {
      break;
    }
  }
  Status = HT_FLASH->OISR;
  HT_FLASH->OISR &= ~(FLASH_INT_ITADIEN);

  if (Status & FLASH_FLAG_PPEF)
  {
    return FLASH_ERR_WRITE_PROTECTED;
  }
  if (Status & FLASH_FLAG_ITADF)
  {
    return FLASH_ERR_ADDR_OUT_OF_RANGE;
  }
  if(Timeout == 0)
  {
    return FLASH_TIME_OUT;
  }

  return FLASH_COMPLETE;
}
/*1656��ֲ����*/
u32 FLASH_GetSecurityStatus(void)
{
  return (HT_FLASH->CPSR);
}

/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */
