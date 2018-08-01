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
#ifndef _POWER_API_H_
#define _POWER_API_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "llwu.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef struct {
    BYTE wuChanel;
    WORD thresh;
    WORD thresl;
    WORD timeScan;
} TSI_WuTypeDef;

typedef union {
    TSI_WuTypeDef TSIWuCfg;
    wuTrigger IOTrigger;
    DWORD TimerInterval;
} WuSetup;

typedef enum {
    POWER_NOMAL, /* */
    POWER_LOWEN, /* CPU lowpower run */
    POWER_SLEEP  /* CPU deep sleep, wakeup by peripheral via reset irq */
} powerMode;
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
 * @func   IsWakeupByPeriph
 * @brief  None
 * @param  None
 * @retval TRUE if wakeup by peripheral
 */
BOOL 
IsWakeupByPeriph(
    DWORD Peripheral
);

/**
 * @func   SetPeriphWakup  
 * @brief  None
 * @param  None
 * @retval None
 */
void 
SetPeriphWakup(
    DWORD Peripheral,
    FunctionalState NewState,
    WuSetup SetupData    
);

/**
 * @func   SetPowerMode  
 * @brief  Set power mode
 * @param  [in]powerMode
 *         POWER_NOMAL:
 *         POWER_LOWEN: Use fast internal clock, core 4Mhz, bus 800Khz. 
 *                      Clock change MUST re-initialize peripheral
 *         POWER_SLEEP: Enter mode VLLS1. 
 *                      MUST set peripheral wakeup before call function
 * @retval None
 */
void
SetPowerMode(
    powerMode mode
);

#endif /* END FILE */
