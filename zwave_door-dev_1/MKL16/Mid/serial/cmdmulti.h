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
#ifndef _COMMAND_MULTI_
#define _COMMAND_MULTI_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "serial_cmd.h"
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
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
void
HandleCmdMulti(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length    
);

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
CmdMultiAddCmd(
    CMD_BUFFER cmd,
    BYTE length
);

BOOL
CmdMultiSendCmd(
    BYTE option,
    BYTE_CALLBACKFUNC completefunc
);

#endif /* END FILE */
