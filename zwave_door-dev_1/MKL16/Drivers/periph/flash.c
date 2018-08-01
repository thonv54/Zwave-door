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
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "flash.h"
#include "driver_common.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
enum _Flash_Command_ {
    Read1s = 0x01,
    ProgramCheck = 0x02,
    ReadResource = 0x03,
    ProgramLongword = 0x06,
    EraseSector = 0x09,
    Read1sBlock = 0x40,
    ReadOnce = 0x41,
    ProgramOnce = 0x43,
    EraseAll = 0x44,
    VerifyBackdoor = 0x45
};
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
) {
    if((dwAddress & 0x1FF) != 0) return; 
    ENTER_CRITCAL;                                 /* Disable irq while eraser flash */
    while (((FTFA->FSTAT) & (1UL << 7)) == 0x00){} /* */
    FTFA->FSTAT = FTFA_FSTAT_FPVIOL_MASK | FTFA_FSTAT_ACCERR_MASK | FTFA_FSTAT_RDCOLERR_MASK;
    FTFA->FCCOB0 = EraseSector;                    /* Setup command */
    FTFA->FCCOB1 = GET_BIT_16_23(dwAddress);       /* Address eraser */
    FTFA->FCCOB2 = GET_BIT_8_15(dwAddress);        /* */
    FTFA->FCCOB3 = GET_BIT_0_7(dwAddress);         /* */
    FTFA->FSTAT = FTFA_FSTAT_CCIF_MASK;            /* */
    while (!(FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK)); /* Wait for completed command */
    EXIT_CRITICAL;                                 /* Enable irq */
}

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
) {	
    BYTE byNumOfPage = 0;  
    BYTE i; 
    DWORD dwAddr = dwAddrPageStart;

    byNumOfPage = dwSize >> 10;
    if (dwSize > (byNumOfPage << 10))  byNumOfPage++;

    for (i = 0; i < byNumOfPage; i++) {
        FLASH_Erasepage(dwAddr);
        dwAddr += FLASH_PAGE_SIZE;
    }
}

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
) {
    /* Check address */
    if ((dwAddress&0x3) != 0) return; 

    ENTER_CRITCAL;
  
    while (((FTFA->FSTAT)&(1UL << 7)) == 0x00){} 
    if (!((FTFA->FSTAT)==0x80))     { FTFA->FSTAT = 0x30; }
    /* Select command */
    FTFA->FCCOB0 = ProgramLongword;
    /* Write address + data */
    FTFA->FCCOB1 = GET_BIT_16_23(dwAddress);
    FTFA->FCCOB2 = GET_BIT_8_15(dwAddress);
    FTFA->FCCOB3 = GET_BIT_0_7(dwAddress);

    FTFA->FCCOB4 = pbyData[3];
    FTFA->FCCOB5 = pbyData[2];
    FTFA->FCCOB6 = pbyData[1];
    FTFA->FCCOB7 = pbyData[0];
    /* Start command */
    FTFA->FSTAT = 0x80;
    /* Wait for completed */
    while ((((FTFA->FSTAT)&(1UL << 7))==0x00)) {}

    EXIT_CRITICAL;
}

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
) {
    BYTE pbuff[4];

    pbuff[0] = GET_BIT_0_7(dwData);
    pbuff[1] = GET_BIT_8_15(dwData);
    pbuff[2] = GET_BIT_16_23(dwData);
    pbuff[3] = GET_BIT_24_31(dwData);
    FLASH_Write4Byte(dwAddr,pbuff);
 }

/**
 * @func   ReadFlashDWord
 * @brief  None
 * @param  None
 * @retval None
 */ 
DWORD
FLASH_ReadDword(
    DWORD dwAddr
) {
    return READ32(dwAddr);
}

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
) {
    BYTE i;
    DWORD dwAddreess = dwAddr;
    for(i = 0; i < 4; i++) {
       pData[i] = READ8(dwAddreess);
       dwAddreess++;
    }
}

/**
 * @func   ReadFlashNByte
 * @brief  None
 * @param  None
 * @retval None
 */ 
void
FLASH_ReadNbyte(
    DWORD dwAddr,
    u8_p pByBuff,
    DWORD dwSize
) {
    WORD bySizeDiv4;
    DWORD dwTemp;	
    BYTE i,j;

    bySizeDiv4 = dwSize>>2;
    for (i = 0; i < bySizeDiv4; i++){
        dwTemp = FLASH_ReadDword(dwAddr + (i<<2));
        for (j = 0; j < 4; j++) {
            *pByBuff = dwTemp >> (j<<3);
            pByBuff++;
        }
    }	
}

/**
 * @func   ProgramFlashNByte
 * @brief  None
 * @param  None
 * @retval None
 */ 
void
FLASH_WriteNbyte(
    DWORD dwAddr,
    u8_p pbyData,
    WORD wSize
) {
    DWORD dwAddress = dwAddr;
    DWORD dwData;
    BYTE i;
	
    /* Size = k*4 byte */
    wSize = wSize >> 2;

    while (wSize > 0) {
        dwData = 0;
        for (i = 0; i < 4; i++) {
            dwData |= *pbyData << (i<<3); 
            pbyData++;    
        } 
     
        FLASH_WriteDword(dwAddress,dwData);       
        wSize --;
        dwAddress += 4;
    }
}

/**
 * @func   FLASH_Size
 * @brief  None
 * @param  None
 * @retval Return size flash in chip
 */ 
DWORD 
FLASH_Size(void) {
    DWORD retval ;

    retval = (SIM->FCFG2 & 0x7F000000u) >> (24-13);
    if (SIM->FCFG2 & (1<<23))        //Possible second flash bank
        retval += (SIM->FCFG2 & 0x007F0000u) >> (16-13);

    return retval;
}

/* END FILE */
