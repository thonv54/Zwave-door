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
#include "cmdmulti.h"
#include "cmdcommon.h"
#include "utilities.h"
#include "debug.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef MULTICMD_DEBUG
#define DBG_MULTICMD_SEND_STR(x)         Debug_SendStr(x)
#define DBG_MULTICMD_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_MULTICMD_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_MULTICMD_SEND_STR(x)
#define DBG_MULTICMD_SEND_NUM(x)
#define DBG_MULTICMD_SEND_HEX(x)
#endif

#define MAX_SIZE_BUFF                    32
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BYTE pbuffCmd[MAX_SIZE_BUFF];
static BYTE lengthbuff = 0;
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
 * @func   CmdMultiAddCmd
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
CmdMultiAddCmd(
    CMD_BUFFER cmd,
    BYTE length
) {
    CMD_BUFFER_P pCmd = (CMD_BUFFER_P)pbuffCmd;;
    if (lengthbuff == 0) {
        pCmd->multicmd.cmdid = CMD_MULTI;
        pCmd->multicmd.cmdtype = CMD_TYPE_GET;
        pCmd->multicmd.numbCmd = 0;
        pCmd->multicmd.length = length;
        lengthbuff = 4;
    }
    else {
        pbuffCmd[lengthbuff] = length;
        lengthbuff++;
    }

    if (lengthbuff + length > MAX_SIZE_BUFF) return FALSE;
    memcpyl(&pbuffCmd[lengthbuff], (BYTE_p)&cmd, length);
    lengthbuff += length;
    pCmd->multicmd.numbCmd ++;

    return TRUE;
}

BOOL
CmdMultiSendCmd(
    BYTE option,
    BYTE_CALLBACKFUNC completefunc
) {
    BYTE length = lengthbuff;
    lengthbuff = 0;
    return SendCmd(option, (CMD_BUFFER_P) pbuffCmd, length, completefunc);         
}

void
HandleCmdMulti(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length    
) {
    BYTE lengthCmd;
    BYTE numbCmd = pCmd->multicmd.numbCmd;
    BYTE_p index = (BYTE_p)&pCmd->multicmd.length;
    CMD_BUFFER_P pExtractCmd;

    DBG_MULTICMD_SEND_STR("$ CMD MULTI ");
    DBG_MULTICMD_SEND_NUM(numbCmd);
    DBG_MULTICMD_SEND_STR("\n");
    if (pCmd->scommon.cmdtype == CMD_TYPE_GET) {
        for (BYTE i = 0; i < numbCmd; i++) {
            lengthCmd = (BYTE)*index;
            index++;
            pExtractCmd = (CMD_BUFFER_P) index;
            HandleSerialCommand(option, pExtractCmd, lengthCmd);
            index += lengthCmd; 
        }
    }
}
