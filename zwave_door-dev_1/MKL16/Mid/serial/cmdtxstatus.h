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
#ifndef _COMMAND_TXSTATUS_
#define _COMMAND_TXSTATUS_ 
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
 * @func   HandleCmdTxStatus 
 * @brief  None
 * @param  None
 * @retval None
 */
void
HandleCmdTxStatus(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length
);

void
TxFailShow(void);

#endif /* END FILE */