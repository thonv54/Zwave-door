/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: TrungTQ
 *
 * Last Changed By:  $Author: trungtq $
 * Revision:         $Revision: 2.1 $
 * Last Changed:     $Date: 2016-02-23 02:16:21 (Tue, 23 Feb 2016) $
 *
 ******************************************************************************/
#ifndef _SERIAL_H_
#define _SERIAL_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ZW_typedefs.h>
#include "serial_cmd.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef enum _RX_STATE_ {
    stateIdentifyFrame,
    stateLen,
    stateData,
    stateCheckSum
} RX_STATE;

typedef enum _UART_STATE_ {
    stateIdle,
    stateFrameDataReceived,
    stateFrameACKReceived,
    stateFrameNACKReceived,
    stateFrameSent,
    stateFrameErr,
    stateRxTimeout,
    stateTxTimeout
} UART_STATE;

typedef enum _RESPOND_STATE_ {
    resCheck,
    resNot,
    resAck,
    resNack,
} RES_STATE;
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
 * @func   InitSerial   
 * @brief  None
 * @param  WORD wBaudRate
 * @retval None
 */
void
InitSerial(
    WORD wBaudRate
);

/**
 * @func   RxBufPoll   
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
RxBufPoll(void);

/**
 * @func   SendFrame   
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
SendFrame(
    BYTE option,
    BYTE_P pframebuff,
    BYTE length
);

/**
 * @func   SendACK   
 * @brief  None
 * @param  None
 * @retval None
 */
void
SendACK();

/**
 * @func   SendACK   
 * @brief  None
 * @param  None
 * @retval None
 */
void
SendNACK();

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
RES_STATE
GetRespond(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
procSerial(void);

extern void
HandleSerialCommand(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length
);

#endif /* _SERIAL_H_ */

