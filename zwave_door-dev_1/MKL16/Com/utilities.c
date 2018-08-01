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
 * Last Changed:     $Date: 28/08/17 15:00 $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "utilities.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

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
WORD 
searchArrayInArray(
    const BYTE *pArrKey,
    u8_t keyLength,
    const u8_p pArrData,
    u16_t dataLength
) {
    WORD i,j = 0;
    if (dataLength < keyLength) return NOTFOUND;
    
    for (i = 0; i!= dataLength && j != keyLength; i++) {
         if (pArrKey[j] == pArrData[i]) {
             j++;
         }
         else {
             i -= j;
             j = 0;
         }
        
         if (j == keyLength) {
             /* */
             return (i + 1 - keyLength );   
         }      
    } 

    return NOTFOUND;
}

/**
 * @func   hex2Char
 * @brief  None
 * @param  None
 * @retval 
 */
BYTE 
hex2Char(
    BYTE byHex
) {
    BYTE byChar; 
    
    if (byHex < 10) byChar = byHex + 0x30;
    else byChar = byHex + 55;
           
    return byChar;
}

/**
 * @func   String2Uint
 * @brief  None
 * @param  None
 * @retval 
 */
DWORD
String2Uint(
    u8_p pChar,
    u8_t offset,
    u8_t length
) {
    u8_t pTemp[8];
    u8_t i;
    u8_t j = length - 1;
    u32_t retval = 0; 
    u32_t temp = 1;  

    for (i = 0; i < length; i++) {
        pTemp[j - i] = pChar[i + offset] - '0';
    } 
    
    for (i = 0; i < length; i++) {
        if (i == 0) {
            temp = 1;
            retval = 0;
        }
        else 
            temp = temp * 10;

        if (pTemp[i] < 10) {
            retval += pTemp[i] *temp;
        } 
        else {
            return 0xFFFFFFFF;
        }
    }  

    return retval;
}

/**
 * @func   memsetl
 * @brief  None
 * @param  None
 * @retval None
 */
void
memsetl(
    BYTE_p dst,
    BYTE value,
    WORD size
) {
    while (size--) {
        *dst++ = value;
    }    
}

/**
 * @func   memcpyl
 * @brief  None
 * @param  None
 * @retval None
 */
void
memcpyl(
    BYTE_p dst,
    BYTE_p src, 
    WORD size
) {
    while(size--) {
        *dst++ = *src++;
    }   
}

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
) {
    return ((val >= rmin) && (val <= rmax));
}

/* END FILE */
