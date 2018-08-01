/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: ThangDH
 *
 * Last Changed By:  $Author: thangdh $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 17/08/17  13:30 $
 *
 ******************************************************************************/
#ifndef _FLASH_H_
#define _FLASH_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define FLASH_SIZE                  32768
#define FLASH_PAGE_SIZE             1024
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   FLASH_Erasepage
 * @brief  Eraser one page flash start at dwAddress
 * @param  [in] address (must % 1024 = 0)
 * @retval None
 */ 
void
FLASH_Erasepage(
      DWORD dwAddress
);

/**
 * @func   FLASH_WriteDword 
 * @brief  None
 * @param  None
 * @retval None
 */
void
FLASH_WriteDword(
    DWORD dwAddr,
    DWORD dwData
);

/**
 * @func   FLASH_ReadDword
 * @brief  None
 * @param  None
 * @retval None
 */
DWORD
FLASH_ReadDword(
    DWORD dwAddr
);

/**
 * @func   FLASH_ReadWord
 * @brief  None
 * @param  None
 * @retval None
 */
WORD
FLASH_ReadWord(
    DWORD dwAddr
);

/**
 * @func   FLASH_EraseNPage
 * @brief  None
 * @param  None
 * @retval None
 */
void
FLASH_EraseNPage(
    DWORD dwAddrPageStart,
    DWORD dwSize
);

/**
 * @func   FLASH_ReadNbyte
 * @brief  None
 * @param  None
 * @retval None
 */
void
FLASH_ReadNbyte(
    DWORD dwAddr,
    u8_p pByBuff,
    DWORD dwSize
);

/**
 * @func   FLASH_WriteNbyte 
 * @brief  None
 * @param  None
 * @retval None
 */
void
FLASH_WriteNbyte(
    DWORD dwAddr,
    u8_p pbyData,
    WORD wSize
);

/**
 * @func   FLASH_Write4Byte
 * @brief  None
 * @param  None
 * @retval None
 */
void
FLASH_Write4Byte(
    DWORD dwAddress,
    u8_p pbyData
);

/**
 * @func   FLASH_Read4Byte
 * @brief  None
 * @param  None
 * @retval None
 */
void
FLASH_Read4Byte(
    DWORD dwAddr,
    u8_p pData
) ;

#endif /* END FILE */
