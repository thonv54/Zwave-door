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
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <misc.h>
#include <ZW_tx_mutex.h>
#include <Zw_task.h>

#include "zwmcu_cmd.h"
#include "serial_cmd.h"
#include "sensor.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static APP_STATE_FUNC procStateAppFunc = NULL;
static BYTE taskHandleId = 0;
static BYTE g_bReportTimerHandle = NO_TIMER;
static BYTE gZwNetw = ZW_NOT_NETW;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void SensReportVal(void);
static void DisableTimerTimeoutReport(void);
static void EnableTimerTimeoutReport(void);
BOOL ZCB_SerialPoll(void);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   InitProcessManager   
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitProcessManager(
    APP_STATE_FUNC procStateApp
) {
    procStateAppFunc = procStateApp;
}

/**
 * @func   InitCmmZw 
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitCmmZw (void) {
    InitSerial(ZW_INF_BAURATE);

    if ((taskHandleId = TaskAdd(ZCB_SerialPoll, (const char*)"SerialPoll")) == 0)
    {
        
    }
}

/**
 * @func   ZCB_SerialPoll
 * @brief  None
 * @param  None
 * @retval None
 */
PCB_BOOL(ZCB_SerialPoll)(void)
{
    procSerial();
    return TRUE;
}

void
CmdZwTxFailReport(
    WORD numbsfail
) {
    CMD_BUFFER cmd;

    cmd.txfailstatus.cmdid = CMD_ZW_TXFALSE;
    cmd.txfailstatus.cmdtype = CMD_TYPE_RES;
    cmd.txfailstatus.numbH = HI_UINT16(numbsfail);
    cmd.txfailstatus.numbL = LO_UINT16(numbsfail);

    SendFrame(OPT_NOT_USE, (BYTE_P)&cmd, sizeof(cmd.txfailstatus));
}

void
CmdZwTimeConfigSet(
    BYTE typeTime,
    WORD value
) {
    CMD_BUFFER cmd;

    cmd.txfailstatus.cmdid = typeTime;
    cmd.txfailstatus.cmdtype = CMD_TYPE_SET;
    cmd.txfailstatus.numbH = HI_UINT16(value);
    cmd.txfailstatus.numbL = LO_UINT16(value);

    SendFrame(OPT_NOT_USE, (BYTE_P)&cmd, sizeof(cmd.txfailstatus));
}

void
HandleCmdDoor(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length
) {
    UNUSED(option);
    UNUSED(length);

    switch(pCmd->scommon.cmdtype) {
    case CMD_TYPE_RES:
    {
        SensValUpdate(SENSOR_DOOR, pCmd->door.status);
    }   break;

    default:
        break;
    }

    EnableTimerTimeoutReport();
}

void
HandleCmdCmdKeyConfig(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length
) {
    UNUSED(option);
    UNUSED(length);

    switch(pCmd->scommon.cmdtype) {
    case CMD_TYPE_RES: 
    {
        BYTE buttonState = pCmd->keyconfig.evkey;
        if (procStateAppFunc != NULL) {
            if (BUTT_HOLD_05SEC == buttonState) {
                procStateAppFunc(EVENT_BUTTON_ADDNODE);
            }
            else if (BUTT_HOLD_10SEC == buttonState) {
                procStateAppFunc(EVENT_BUTTON_RESET);
            }
            else if (BUTT_PRESS_1TIME == buttonState) {
                procStateAppFunc(EVENT_BUTTON_PRESS);
            }
            else if (BUTT_PRESS_3TIME == buttonState) {
                procStateAppFunc(EVENT_BUTTON_KEEP_AWAKE);
            }
        }

    }   break;

    default:
        break;
    }
}

void
CmdZwNetwReportStatus() {
    CMD_BUFFER cmd;

    cmd.netwstatus.cmdid = CMD_ZW_NETWORK;
    cmd.netwstatus.cmdtype = CMD_TYPE_RES;
    cmd.netwstatus.reserved = 0;
    cmd.netwstatus.status = gZwNetw;

    SendFrame(OPT_NOT_USE, (BYTE_P)&cmd, sizeof(cmd.netwstatus));
}

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
    UNUSED(option);
    UNUSED(length);
    switch(pCmd->scommon.cmdtype) {
    case CMD_TYPE_GET:
        CmdZwNetwReportStatus();
        break;

    default:
        break;
    }
}

void
UpdZwNetw(
    BYTE netwStatus
) {
    gZwNetw = netwStatus;
}

void
HandleCmd(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length
) {
    switch(pCmd->scommon.cmdid) {
    case CMD_ZW_NETWORK:
        HandleCmdZwNetw(option, pCmd, length);
        break;

    case CMD_EVNT_BUTT:
        HandleCmdCmdKeyConfig(option, pCmd, length);
        break;

    case CMD_DOOR:
        HandleCmdDoor(option, pCmd, length);
        break;
    }
}

void
HandleCmdMulti(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length    
) {
    BYTE i;
    BYTE lengthCmd ;
    BYTE numbCmd = pCmd->multicmd.numbCmd;
    BYTE_P index = (BYTE_P)&pCmd->multicmd.length;
    CMD_BUFFER_P pExtractCmd;

    UNUSED(length);
    if (pCmd->scommon.cmdtype == CMD_TYPE_GET) {
        for (i = 0; i < numbCmd; i++) {
            lengthCmd = (BYTE)*index;
            index++;
            pExtractCmd = (CMD_BUFFER_P) index;
            HandleSerialCommand(option, pExtractCmd, lengthCmd);
            index += lengthCmd; 
        }
    }
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

/**
 * @func   SensReportVal
 * @brief  Report 
 * @param  None
 * @retval None
 */
PCB(SensReportVal) (void) {
    SensStoreVal();

    if ((procStateAppFunc != NULL) && (FALSE == ActiveJobs()))
    {
        DisableTimerTimeoutReport();
        procStateAppFunc(EVENT_SENSOR_REPORT);
    }
}

/**
 * @func   EnableTimerTimeoutReport
 * @brief  None
 * @param  None
 * @retval None
 */
void 
EnableTimerTimeoutReport() {
    if (g_bReportTimerHandle != NO_TIMER) {
        ZW_TIMER_CANCEL(g_bReportTimerHandle);
        g_bReportTimerHandle = NO_TIMER; 
    }

    g_bReportTimerHandle = 
        ZW_TIMER_START(SensReportVal, TIMER_SENS_REPORT_VAL, NUM_TRIES_REPORT);
}

/**
 * @func   DisableTimerTimeoutReport
 * @brief  None
 * @param  None
 * @retval None
 */
void 
DisableTimerTimeoutReport() {
    if (g_bReportTimerHandle != NO_TIMER) {
        ZW_TIMER_CANCEL(g_bReportTimerHandle);
        g_bReportTimerHandle = NO_TIMER;
    }
}

/* END FILE */
