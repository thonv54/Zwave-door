/*******************************************************************************
 *
 * Copyright (c) 2017
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
#include "configboard.h"
#include "sysregval.h"
#include "sensdoor.h"
#include "gpio.h"

#include "debug.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef DOOR_DEBUG
#define DBG_SENS_SEND_STR(x)         Debug_SendStr(x)
#define DBG_SENS_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_SENS_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_SENS_SEND_STR(x)
#define DBG_SENS_SEND_NUM(x)
#define DBG_SENS_SEND_HEX(x)
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
 * @func   InitDoorSens
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitDoorSens(void) {
    GPIO_SetPinIsInput(DOOR_SENS, PIN_PULL_UP);
}

/**
 * @func   IsDoorOpen
 * @brief  None
 * @param  None
 * @retval TRUE if DOOR OPEN
 */
static BOOL
IsDoorOpen(void) {
    BYTE cnt = 0;

    for (BYTE i = 0; i < 20; i++) {
        for (WORD j = 0; j < 200; j++) {}
        if (GPIO_PinReadInput(DOOR_SENS)) {
            cnt++;
        }
    }

    if (cnt > 15) return TRUE;
    return FALSE;
}

/**
 * @func   DoorSensRead
 * @brief  None
 * @param  None
 * @retval None
 */
void
DoorSensRead(void) {
    static BYTE gdoorState;

    if (IsDoorOpen()) {
        gdoorState = DOOR_OPEN;
    }
    else {
        gdoorState = DOOR_CLOSE;
    }

    if ((gdoorState != SysPara->doorState) || /* DOOR change satus */
        (SysPara->countRpDoor == 0))          /* reached interval report */
    {
        SysPara->doorState = gdoorState;
        DoorSetFlagReport(ENABLE);
    }

    if (SysPara->doorCheckCount != 0)         /* DOOR not stable, set status is OPEN */
    {
        SysPara->doorState = DOOR_OPEN;
        DoorSetFlagReport(ENABLE);        
    }
}

BOOL
DoorGetFlagReport(void) {
    return SysPara->doorFlagReport;
}

void
DoorSetFlagReport(
    FunctionalState rpstatus
) {
    if (rpstatus == ENABLE) {
        SysPara->doorFlagReport = TRUE;
        SysPara->countRpDoor = TIME_INTERVAL_REPORT_DOOR;
    }
    else {
        SysPara->doorFlagReport = FALSE;
    }
}

BYTE
DoorGetState(void) {
    return SysPara->doorState;
}

BYTE
GetCountCheckDoorStatus(void) {
    return SysPara->doorCheckCount;
}

void
SetCountCheckDoorStatus(
    BYTE count
) {
    SysPara->doorCheckCount = (count > TIME_TRIES_CHECK_DOOR_STABLE) ? TIME_TRIES_CHECK_DOOR_STABLE:count;
}
/* END FILE */
