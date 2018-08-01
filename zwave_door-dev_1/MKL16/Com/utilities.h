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
 * Last Changed:     $Date: 17/08/17 09:00 $
 *
 ******************************************************************************/
#ifndef _UTILITIES_H_
#define _UTILITIES_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define NOTFOUND                    MAX_U16
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
 * @func   searchArrayInArray
 * @brief  None
 * @param  None
 * @retval offset ArrKey match in ArrData OR NOT_FOUND
 */
inline WORD 
searchArrayInArray(
    const BYTE *pArrKey,
    u8_t keyLength,
    const u8_p pArrData,
    u16_t dataLength
);

/**
 * @func   hex2Char 
 * @brief  None
 * @param  [in] 
 * @retval char
 */
BYTE 
hex2Char(
    BYTE byHex
); 

/**
 * @func   String2Uint 
 * @brief  None
 * @param  [in] 
 * @retval 
 */
DWORD
String2Uint(
    u8_p pChar,
    u8_t offset,
    u8_t length
);

void
memcpyl(
    BYTE_p dst,
    BYTE_p src, 
    WORD size
);

void
memsetl(
    BYTE_p dst,
    BYTE value,
    WORD size
);

/**
 * @func   valInRange 
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
valInRange(
    DWORD val,
    DWORD rmin,
    DWORD rmax
);

#endif /* END FILE */
