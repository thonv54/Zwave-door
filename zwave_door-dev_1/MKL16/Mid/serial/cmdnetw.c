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
#include "cmdnetw.h"
#include "zwnetstatus.h"
#include "cmdcommon.h"
#include "debug.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef ZWNETW_DEBUG
#define DBG_ZWNETW_SEND_STR(x)         Debug_SendStr(x)
#define DBG_ZWNETW_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_ZWNETW_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_ZWNETW_SEND_STR(x)         Debug_SendStr(x)
#define DBG_ZWNETW_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_ZWNETW_SEND_HEX(x)         Debug_SendHex(x)
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
 * @func   HandleCmdZwNetw 
 * @brief  None
 * @param  None
 * @retval None
 */
void
HandleCmdZwNetw(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length
) {
    switch(pCmd->netwstatus.cmdtype) {
    case CMD_TYPE_RES:
    {
        BYTE netwstatus =  pCmd->netwstatus.status;
        if (netwstatus == ZW_IN_NETW) {
            DBG_ZWNETW_SEND_STR("$ ifo zw netw: IN NETW\n");
            SetZwNetwStatus(ZW_NETW_INNW);
        }
        else if (netwstatus == ZW_NOT_NETW) {
            DBG_ZWNETW_SEND_STR("$ ifo zw netw: NOT NETW\n");
            SetZwNetwStatus(ZW_NETW_NOTNW);
        }
        else {
            DBG_ZWNETW_SEND_STR("$ ifo zw netw: resend GET\n");
            SetZwNetwStatus(ZW_NETW_REQ);
        }
    }   break;

    default:
        break;
    }
}

BOOL
CmdZwNetwGetStatus(
    BYTE_CALLBACKFUNC completefunc
) {
    CMD_BUFFER cmd;

    cmd.netwstatus.cmdid = CMD_ZW_NETWORK;
    cmd.netwstatus.cmdtype = CMD_TYPE_GET;
    cmd.netwstatus.reserved = 0;
    cmd.netwstatus.status = 0;
    DBG_ZWNETW_SEND_STR("$ Zwcmd request netw\n");

    return SendCmd(OPT_NOT_USE, &cmd, sizeof(cmd.netwstatus), completefunc);
}

/* END FILE */
