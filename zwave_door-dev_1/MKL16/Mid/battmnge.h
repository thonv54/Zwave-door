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
#ifndef _BATT_MANAGE_H_
#define _BATT_MANAGE_H_ 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
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
InitBattMonitor(void);
 
/**
 * @func   BattSetDefault
 * @brief  Call one time when MCU power on
 * @param  None
 * @retval None
 */
void
BattSetDefault(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval TRUE ~ FALSE
 */
BOOL 
GetFlagBatLevlUpd(void);

/**
 * @func   BattReadLevl
 * @brief  None
 * @param  None
 * @retval Battery level
 */
BYTE 
BattReadLevl(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
SetFlagBatLevlUpd(
    BOOL flag
);

#endif /* END FILE */
