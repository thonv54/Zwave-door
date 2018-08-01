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
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "sysregval.h"
#include "configapp.h"
#include "configboard.h"
#include "utilities.h"
#include "flash.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define FLASH_ADDR_SYSPARAM          (FLASH_SIZE - FLASH_PAGE_SIZE)
#define SYSCONFIG_SIZE_MAX             64
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BYTE buffConfig[SYSCONFIG_SIZE_MAX];
static SysConfig_p pSysConfig = (SysConfig_p) buffConfig;
static BOOL editFlash = FALSE;
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
ConfigurationSetDefault(void) {
    SysPara->bTimerType = TIMER_LONG;
    SysPara->netwReqCount = TIME_TRIES_REQ_ZWNETW;
    SysPara->doorCheckCount = 0;

    SysPara->countRpDoor = TIME_INTERVAL_REPORT_DOOR;
    SysPara->countWakeup = TIME_INTERVAL_WAKEUP;

    SetConfigParam(PARAM_TIME_WAKEUP, &SysPara->countWakeup);
    SetConfigParam(PARAM_TIME_REPORT_SENS, &SysPara->countRpDoor);
   
    /* Warning */
    StoreConfigParam();
}

void
LoadConfigParam(void) {  
    FLASH_ReadNbyte(FLASH_ADDR_SYSPARAM, buffConfig, SYSCONFIG_SIZE_MAX);
} 

/**
 * @func   StoreConfigParam 
 * @brief  Note: func NOT work in mode VLPR
 * @param  None
 * @retval None
 */
void
StoreConfigParam(void) {
    if (!editFlash) return;

    FLASH_Erasepage(FLASH_ADDR_SYSPARAM);
    FLASH_WriteNbyte(FLASH_ADDR_SYSPARAM, buffConfig, SYSCONFIG_SIZE_MAX);   
}

void
SetConfigParam(
    BYTE param,
    void* val
) {
    editFlash = TRUE;

    switch(param) {
    case PARAM_TIME_WAKEUP:
        pSysConfig->timeWakeup = *((WORD_p)val);
        SysPara->countWakeup = pSysConfig->timeWakeup;
        break;
    
    case PARAM_TIME_REPORT_SENS:
        pSysConfig->timeRpSens = *((WORD_p)val);
        SysPara->countRpDoor = pSysConfig->timeRpSens;
        break;
    }
}

void
GetConfigParam(
    BYTE param,
    void* val
) {
    switch(param) {
    case PARAM_TIME_WAKEUP:
        *((WORD_p)val) = pSysConfig->timeWakeup;
        break;

    case PARAM_TIME_REPORT_SENS:
        *((WORD_p)val) = pSysConfig->timeRpSens;
        break;
    }
}

void
DecreCount(
    WORD_p pCount
) {
    if (pCount == NULL) return;
    if ((*pCount == 0xFFFF) || (*pCount == 0)) return;
    else {
        (*pCount)--;
    }
}

/* END FILE */
