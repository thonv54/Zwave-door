/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: 
 *
 * Last Changed By:  $Author: $
 * Revision:         $Revision: $
 * Last Changed:     $Date: $
 *
 ******************************************************************************/
#ifndef _SERIAL_H_
#define _SERIAL_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "serial_cmd.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef struct _SERIAL_FRAME_ {
    BYTE length;
    BYTE option;
    CMD_BUFFER_P pCmd; /* Caution: ALIGN */
} SERIAL_FRAME, *SERIAL_FRAME_p;

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

typedef enum _RX_STATE_ {
    stateIdentifyFrame,
    stateLen,
    stateData,
    stateCheckSum
} RX_STATE;

typedef enum _RESPOND_STATE_ {
    resNot,
    resAck,
    resNack,
} RES_STATE;

typedef enum {
    JOB_BUSY,
    JOB_SUCCESS,
    JOB_FALSE,
} JOB_STATUS;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
typedef void (*proc_serial_frame)(BYTE, CMD_BUFFER_P, BYTE);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   InitSerial   
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitSerial(void);

/**
 * @func   InitSerialProc  
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitSerialProc(
    WORD_CALLBACKFUNC pSetTimeoutFunc
);

/**
 * @func   SendFrame   
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
SendFrame(
    BYTE option,
    BYTE_p pframebuff,
    BYTE length,
    BYTE_CALLBACKFUNC func
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
procSerial(void);

/**
 * @func   SerialJobFinish
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
SerialJobFinish(void);

#endif /* _SERIAL_H_ */
