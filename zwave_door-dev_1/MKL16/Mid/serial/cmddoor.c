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
#include "cmddoor.h"
#include "serial_cmd.h"
#include "cmdcommon.h"
#include "cmdmulti.h"
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
 * @func   DoorSendReport
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
DoorSendReport(
    BYTE status,
    BYTE_CALLBACKFUNC completefunc
) {
    CMD_BUFFER cmd;

//    cmd.netwstatus.cmdid = CMD_ZW_NETWORK;
//    cmd.netwstatus.cmdtype = CMD_TYPE_GET;
//    cmd.netwstatus.reserved = 0;
//    cmd.netwstatus.status = 0;
//    CmdMultiAddCmd(cmd, sizeof(cmd.netwstatus));

    cmd.door.cmdid = CMD_DOOR;
    cmd.door.cmdtype = CMD_TYPE_RES;
    cmd.door.reserved = 0;
    cmd.door.status = status;
    CmdMultiAddCmd(cmd, sizeof(cmd.door));

    return CmdMultiSendCmd(OPT_NOT_USE, completefunc);
//    return SendCmd(OPT_NOT_USE, &cmd, sizeof(cmd.door), completefunc);   
}
