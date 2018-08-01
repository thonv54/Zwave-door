/*******************************************************************************
 *
 * Copyright (c) 2018
 * Lumi, JSC.
 * All Rights Resered
 *
 *
 * Description:
 *
 * Author: ThangDH 
 *
 * Last Changed By:  $Author: ThangDH $
 * Revision:         $Revision:  $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "msgcount.h"
#include "eeprom.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
DWORD numbTxFalse;
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
 * @func   GetNumbTxFalse
 * @brief  None
 * @param  None
 * @retval None
 */
DWORD
GetNumbTxFalse() {
    MemoryGetBuffer((WORD)&EEOFFSET_COUNT_TXFALSE_far, (BYTE_P)&numbTxFalse, sizeof(DWORD));
    return numbTxFalse;
}

/**
 * @func   SaveNumbTxFalse
 * @brief  None
 * @param  None
 * @retval None
 */
void
SaveNumbTxFalse(
    DWORD cntFalse
) {
    numbTxFalse = cntFalse;
	ZW_MemoryPutBuffer((WORD)&EEOFFSET_COUNT_TXFALSE_far, (BYTE_P)&numbTxFalse, sizeof(DWORD));
}


