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
#include "cmdkeyconfig.h"
#include "cmdcommon.h"
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

/*
 * @func   CmdKeyConfigSendReport
 * @brief  None
 * @param  None
 * @retval TRUE if send finish
 */
BOOL
CmdKeyConfigSendReport(
    BYTE keyEvent,
    BYTE_CALLBACKFUNC completefunc
) { 
    CMD_BUFFER cmd;

    cmd.keyconfig.cmdid = CMD_EVNT_BUTT;
    cmd.keyconfig.cmdtype = CMD_TYPE_RES;
    cmd.keyconfig.no = 1;
    cmd.keyconfig.evkey = keyEvent;

    return SendCmd(OPT_NOT_USE, &cmd, sizeof(cmd.keyconfig), completefunc);    
}
