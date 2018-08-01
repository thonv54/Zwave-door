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
#include "configapp.h"
#include "cmdtxstatus.h"
#include "debug.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef TXFAIL_DEBUG
#define DBG_TXFAIL_SEND_STR(x)         Debug_SendStr(x)
#define DBG_TXFAIL_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_TXFAIL_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_TXFAIL_SEND_STR(x)
#define DBG_TXFAIL_SEND_NUM(x)
#define DBG_TXFAIL_SEND_HEX(x)
#endif
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
 * @func   HandleCmdTxStatus 
 * @brief  None
 * @param  None
 * @retval None
 */
void
HandleCmdTxStatus(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length
) {
    switch(pCmd->txfailstatus.cmdtype) {
    case CMD_TYPE_RES:
    {
        WORD numbtxfail;
        numbtxfail = (pCmd->txfailstatus.numbH << 8) + pCmd->txfailstatus.numbL;
        if (numbtxfail != 0) {
            DBG_TXFAIL_SEND_STR("$ ZW transmit fail!\n");
            TxFailShow();
        }
    }   break;

    default:
        break;
    }
}

/* END FILE */
