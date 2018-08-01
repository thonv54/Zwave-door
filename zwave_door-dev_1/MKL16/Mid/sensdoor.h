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
#ifndef _DOOR_SENSOR_H_
#define _DOOR_SENSOR_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef enum _DOOR_EVENT_ {
    DOOR_IDLE,
    DOOR_OPEN,
    DOOR_CLOSE,
} doorEv;
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
InitDoorSens(void);

/**
 * @func   DoorSensRead
 * @brief  None
 * @param  None
 * @retval None
 */
void
DoorSensRead(void) ;

BYTE
DoorGetState(void);

BOOL
DoorGetFlagReport(void);

void
DoorSetFlagReport(
    FunctionalState rpstatus
);

BYTE
GetCountCheckDoorStatus(void);

void
SetCountCheckDoorStatus(
    BYTE count
);

#endif /* END FILE */
