/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Value stored in System register is temporary, 
 *              Value will be lost when the power failure
 *
 * Author: ThangDH
 *
 * Last Changed By:  $Author: ThangDH $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
#ifndef _SYSTEM_REG_H_
#define _SYSTEM_REG_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef struct {
    DWORD lastWakeupTime;

    WORD countWakeup;
    WORD countRpDoor;

    BYTE netwState              : 2;
    BYTE netwReqCount           : 6;

    BYTE doorState              : 2;
    BYTE doorCheckCount         : 5;
    BYTE doorFlagReport         : 1;

    BYTE battFlagReport         : 1;
    BYTE battLevel              : 4;

    BYTE bTimerType;
} SYSREG_Type; /* MAX 32 bytes */

typedef struct _config_param_ {
    WORD timeWakeup;
    WORD timeRpSens;
} SysConfig, *SysConfig_p;

#define PARAM_TIME_WAKEUP            1      
#define PARAM_TIME_REPORT_SENS       2

#define SYSREG_BASE                  0x40041000
#define SysPara                      ((SYSREG_Type *)SYSREG_BASE)
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
 * @func   ConfigurationSetDefault 
 * @brief  None
 * @param  None
 * @retval None
 */
void
ConfigurationSetDefault(void);

/**
 * @func   StoreConfigParam 
 * @brief  Note: func NOT work in mode VLPR
 * @param  None
 * @retval None
 */
void
StoreConfigParam(void);

void
LoadConfigParam(void);

void
SetConfigParam(
    BYTE param,
    void* val
);

void
GetConfigParam(
    BYTE param,
    void* val
);

void
DecreCount(
    WORD_p pCount
);

#endif /* END FILE */
