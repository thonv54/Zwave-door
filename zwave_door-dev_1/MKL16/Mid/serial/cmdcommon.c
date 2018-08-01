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
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "cmdcommon.h"
#include "cmdmulti.h"
#include "utilities.h"
#include "timer.h"
#include "zwstatus.h"
#include "zwnetstatus.h"
#include "sysregval.h"
#include "debug.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef ZWCMD_DEBUG
#define DBG_ZWCMD_SEND_STR(x)            Debug_SendStr(x)
#define DBG_ZWCMD_SEND_NUM(x)            Debug_SendNum(x)
#else
#define DBG_ZWCMD_SEND_STR(x)
#define DBG_ZWCMD_SEND_NUM(x)
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
 * @func   SendCmd
 * @brief  None
 * @param  None
 * @retval TRUE if send finish
 */
BOOL
SendCmd( 
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length,
    BYTE_CALLBACKFUNC completefunc
) {
    if (pCmd->scommon.cmdid == CMD_EVNT_BUTT) {
        if (pCmd->keyconfig.evkey != BUTT_PRESS) {
            SetZwNetwStatus(ZW_NETW_REQ);
            SetZwNetwReqCount(TIME_TRIES_REQ_ZWNETW);
        }
        else {
            if (GetZwNetwStatus() != ZW_IN_NETW) {
                return FALSE;
            }
        }
    }
    else if (pCmd->scommon.cmdid == CMD_ZW_NETWORK) {
        if (GetZwNetwReqCount() == 0) 
            return FALSE;

        SetZwNetwReqCount(GetZwNetwReqCount() - 1);
    }
    else {
        if (GetZwNetwStatus() != ZW_NETW_INNW) {
            return FALSE;
        }
    }

    if (!ActivateZwave(TIMEOUT_ZW_GOACTIVE))  
        return FALSE;

    DBG_ZWCMD_SEND_STR("$ SendFrame\n");
    return SendFrame(option, (BYTE_p) pCmd, length, completefunc); 
}

void
HandleSerialCommand(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length
) {
    if (pCmd->scommon.cmdid == CMD_MULTI) {
        HandleCmdMulti(option, pCmd, length);
    }
    else {
        HandleCmd(option, pCmd, length);
    }
}

/* END FILE */
