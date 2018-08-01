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
#include "configapp.h"
#include "zwnetstatus.h"
#include "sysregval.h"
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
 * @func   GetZwNetwStatus
 * @brief  None
 * @param  None
 * @retval None 
 */
BYTE
GetZwNetwStatus(void) {
    return SysPara->netwState;
}

/**
 * @func   SetZwNetwStatus 
 * @brief  None
 * @param  None
 * @retval None 
 */
void
SetZwNetwStatus(
    ZW_NETW_STATUS status
) {
    SysPara->netwState = status;
    if (status == ZW_NETW_REQ) {
        if (SysPara->netwReqCount != 0) 
            SysPara->netwReqCount--;
    } 
    else {
        SysPara->netwReqCount = 0;
    }
}

void
SetZwNetwReqCount(
    BYTE count
) {
    SysPara->netwReqCount = (count > TIME_TRIES_REQ_ZWNETW) ? TIME_TRIES_REQ_ZWNETW:count;
}

BYTE 
GetZwNetwReqCount(void) {
    return SysPara->netwReqCount;
}
