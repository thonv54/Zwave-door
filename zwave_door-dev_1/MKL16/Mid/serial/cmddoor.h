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
#ifndef _COMMAND_DOOR_
#define _COMMAND_DOOR_ 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
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
 * @func   DoorSendReport
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
DoorSendReport(
    BYTE status,
    BYTE_CALLBACKFUNC completefunc
);

#endif /* END FILE */
