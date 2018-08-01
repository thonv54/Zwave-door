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
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
#ifndef _PWR_MANAGER_H_
#define _PWR_MANAGER_H_ 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef enum {
    WakeupFistTime = 0x01,
    WakeupPwOn,
    WakeupSwRst,
    WakeupSwCfg,
    WakeupByZwave,
    WakeupSensDoor,
    WakeupSensDoorCk,
    WakeupNotification,
    WakeupSensEvMeasure,
} wakupReason_t;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
extern DWORD gDwTimeWakeup;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   WakeupReason
 * @brief  None
 * @param  None
 * @retval None
 */
void 
WakeupReason(
    BYTE_p bWakeupReason 
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
DWORD
GetTimeSys(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
SetNextPowerMode(
    BYTE nextMode
);

/**
 * @func   SetPowerDownTimeout
 * @brief  None
 * @param  None
 * @retval None
 */
void
SetPowerDownTimeout(
    WORD timeout
);

/**
 * @func   StartPowerDownTimeout
 * @brief  None
 * @param  None
 * @retval None
 */
void
StartPowerDownTimeout(void);

/**
 * @func   PowerDownNow
 * @brief  None
 * @param  None
 * @retval None
 */
void
PowerDownNow(void);

#endif /* END FILE */
