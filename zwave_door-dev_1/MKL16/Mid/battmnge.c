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
#include "configapp.h"
#include "battmnge.h"
#include "sysregval.h"
#include "pmc.h"
#include "uart.h"
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
 * @func   InitBattMonitor
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitBattMonitor() {
    PMC_InitLVD(SysPara->battLevel);
    PMC_SetIrqLVD(DISABLE);
    PMC_SetIrqLVW(DISABLE);

    if (PMC_GetITStatus(LVW_FLAG) == SET) {
        SysPara->battLevel--;
//        SetFlagBatLevlUpd(TRUE);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
BattSetDefault() {
    SysPara->battLevel = (LVD_2V92 - LVD_2V62);
//    SetFlagBatLevlUpd(TRUE);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
BOOL 
GetFlagBatLevlUpd() { 
    return SysPara->battFlagReport;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
SetFlagBatLevlUpd(
    BOOL flag
) {
    SysPara->battFlagReport = flag;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
BYTE 
BattReadLevl(void) {
//            DBG_MAIN_SEND_STR("batlvl- ");
//            DBG_MAIN_SEND_HEX(payload);
//            DBG_MAIN_SEND_STR("\n");  
    return (3 -(SysPara->battLevel - LVD_2V62));
}

/* END FILE */ 
