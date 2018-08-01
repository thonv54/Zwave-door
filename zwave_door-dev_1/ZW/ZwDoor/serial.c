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
 * Last Changed:     $Date: 2016-02-25 15:40:00 (5 Jeb 2016) $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ZW_basis_api.h>
#include <ZW_uart_api.h>
#include <ZW_conbufio.h>
#include <ZW_SerialAPI.h>
#include <misc.h>
#include "serial.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef ZW_DEBUG_SERIAL
#define ZW_DEBUG_SERIAL_SEND_BYTE(data)         ZW_DEBUG_SEND_BYTE(data)
#define ZW_DEBUG_SERIAL_SEND_STR(STR)           ZW_DEBUG_SEND_STR(STR)
#define ZW_DEBUG_SERIAL_SEND_NUM(data)          ZW_DEBUG_SEND_NUM(data)
#define ZW_DEBUG_SERIAL_SEND_WORD_NUM(data)     ZW_DEBUG_SEND_WORD_NUM(data)
#define ZW_DEBUG_SERIAL_SEND_NL()               ZW_DEBUG_SEND_NL()
#else /* ZW_DEBUG_SERIAL */
#define ZW_DEBUG_SERIAL_SEND_BYTE(data)
#define ZW_DEBUG_SERIAL_SEND_STR(STR)
#define ZW_DEBUG_SERIAL_SEND_NUM(data)
#define ZW_DEBUG_SERIAL_SEND_WORD_NUM(data)
#define ZW_DEBUG_SERIAL_SEND_NL()
#endif /* ZW_DEBUG_SERIAL */

#define RX_BUFFER_SIZE                      64
#define TX_BUFFER_SIZE                      64
#define RlengthBuf                          (*pSerialRxBuff)
/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/
static BYTE byRxBufState;
static BYTE byIndexRxBuf;
static BYTE byCheckSumRxBuf;
static WORD wTimeoutRx;
static BOOL bRxActive = FALSE;
static BOOL bTxActive = FALSE;
static BYTE g_bCheckAckTimerHandle = 0xFF;
static RES_STATE resState = resNot;
static BYTE idTimerSendFrame = 0xFF;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
BYTE pSerialRxBuff[RX_BUFFER_SIZE] = { 0 };
BYTE pSerialTxBuff[TX_BUFFER_SIZE];
BYTE gSerialTxLength;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void TimerReset(WORD_P pwTimerStartTick);
static WORD TimerGetElapsedTime(WORD wTimerStartTick);
static void StartTimerCheckAck(void);
static void StopTimerCheckAck(void);
static void CheckAck();
static void SendData();
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   TimerReset   
 * @brief  Reset startTick to current timer tick
 * @param  WORD_P pWTimerStartTick
 * @retval None
 */
void
TimerReset(
    WORD_P pWTimerStartTick
) {
    *pWTimerStartTick = getTickTime();
}

/**
 * @func   TimerGetElapsedTime   
 * @brief  Get elapsed time  
 * @param  WORD wTimerStartTick
 * @retval WORD Elapsed Time in 10 ms ticks
 */
WORD
TimerGetElapsedTime(
    WORD wTimerStartTick
) {
    return (getTickTime() - wTimerStartTick);
}

/**
 * @func   InitSerial   
 * @brief  Initialize UART
 * @param  WORD wBaudRate
 * @retval None
 */
void
InitSerial(
    WORD wBaudRate
) {
    ZW_InitSerialIf(wBaudRate);
    ZW_FinishSerialIf();

    byRxBufState = stateIdentifyFrame;
}

/**
 * @func   GetFuncId
 * @brief  None
 * @param  None
 * @retval
 */
BYTE 
GetFuncId (
    BYTE type
) {
	static BYTE byFuncIdUpd = FUNID_MZ_MIN;

    if (type == FUNID_ZM) {
        byFuncIdUpd++;
        if (byFuncIdUpd >= FUNID_ZM_MAX) {
            byFuncIdUpd  = FUNID_ZM_MIN;
        }
        return byFuncIdUpd;
    }

    return 0;
}

/**
 * @func   RxBufPoll   
 * @brief  None  
 * @param  None  
 * @retval None
 */
BYTE
RxBufPoll() {
    BYTE byRetVal = stateIdle;
    BYTE byRxData;
    BOOL bFrameErr = FALSE; 

    if (0 != ZW_SerialCheck()) {
        do {
            byRxData = ZW_SerialGetByte();
            switch (byRxBufState) {
            case stateIdentifyFrame:
                if (FRAME_SOF == byRxData) {
                    byRxBufState = stateLen;
                    bRxActive = TRUE;
                    TimerReset(&wTimeoutRx);
                }
                else if (FRAME_ACK == byRxData) {
                    byRetVal = stateFrameACKReceived;
                }
                else if (FRAME_NACK == byRxData) {
                    byRetVal = stateFrameNACKReceived;
                }
                else {
                    bFrameErr = TRUE;
                }
                break;
                           
            case stateLen:
				if ((FRAME_LENGTH_V1 == byRxData) || 
					((FRAME_LENGTH_MAX > byRxData) && 
					 (FRAME_LENGTH_MIN < byRxData))) 
				{
                    byIndexRxBuf = 0;
                    byCheckSumRxBuf = CSUM_INIT_VAL;
                    byRxBufState = stateData;
                }
                else {
                    bRxActive = FALSE;
                    byRxBufState = stateIdentifyFrame;
                    byRetVal= stateFrameErr;
                    break;
                }
 
            case stateData:
                if (byIndexRxBuf < RX_BUFFER_SIZE) {
                    pSerialRxBuff[byIndexRxBuf] = byRxData;
                    if (byIndexRxBuf > 0) {
                        byCheckSumRxBuf ^= byRxData;
                    }

                    if (++byIndexRxBuf >= RlengthBuf) {
                        byRxBufState = stateCheckSum;
                    }
                } 
                else {
                    byRxBufState = stateIdentifyFrame;
                    byRetVal= stateFrameErr;
                }

                TimerReset(&wTimeoutRx);
                break;
            case stateCheckSum:
                if (byRxData == byCheckSumRxBuf) {
                    byRetVal = stateFrameDataReceived;
                }
                else {
                    byRetVal = stateFrameErr;
                }
                bRxActive = FALSE;

            default:
                byRxBufState = stateIdentifyFrame;
                break;
            }            
        } while ((ZW_SerialCheck() != 0) && (byRetVal == stateIdle));
    }
 
    if (byRetVal == stateIdle) {
        if (bRxActive && (TimerGetElapsedTime(wTimeoutRx) >= RX_TIMEOUT)) {
            byRetVal = stateRxTimeout;
            bRxActive = FALSE;
            byRxBufState = stateIdentifyFrame;
        }
    }

    if (bFrameErr) {
        return stateFrameErr;
    }
 
    return byRetVal;
}

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
) {
    BYTE i;
    BYTE csum = CSUM_INIT_VAL;
    BOOL txState = FALSE;
  
    CMD_BUFFER_P pCmd = (CMD_BUFFER_P) pframebuff;

	gSerialTxLength = length + 5;

    pSerialTxBuff[0] = FRAME_SOF;
    pSerialTxBuff[1] = length + 3; /* Option BYTE + FuncID BYTE + Checksum BYTE */
    pSerialTxBuff[2] = option;
    memcpy(&pSerialTxBuff[3], pframebuff, length);
    pSerialTxBuff[gSerialTxLength - 2] = GetFuncId(FUNID_ZM); /* FuncId */

    for (i = 2; i < gSerialTxLength - 1; i++) {
        csum ^= pSerialTxBuff[i];
    }
   
    pSerialTxBuff[gSerialTxLength - 1] = csum; /* Checksum */

    bTxActive = TRUE;
    resState = resCheck;

	if (idTimerSendFrame == 0xFF)  {
        idTimerSendFrame = ZW_TIMER_START(SendData, 1, TIMER_ONE_TIME);
        return TRUE;
    }

    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
PCB(SendData)(void) {
    BYTE i;
    for (i = 0; i < gSerialTxLength; i++) {
        ZW_SerialPutByte(pSerialTxBuff[i]);
    }

	idTimerSendFrame = 0xFF;
    ZW_SerialFlush();
	StartTimerCheckAck(); 
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
SendACK() {
    ZW_SerialPutByte(FRAME_ACK);
    ZW_SerialFlush();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
SendNACK() {
    ZW_SerialPutByte(FRAME_NACK);
    ZW_SerialFlush();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
procSerial(void) {
    BYTE stateRx;
    RES_STATE resState;

    stateRx = RxBufPoll();

    if (stateRx != stateIdle) {
        switch (stateRx) {
        case stateFrameACKReceived:    
            bTxActive = FALSE;
            resState = resAck;
			StopTimerCheckAck();
            break;

        case stateFrameNACKReceived:
            bTxActive = FALSE;
            resState = resNack;
			StopTimerCheckAck();
            break;

        case stateFrameDataReceived:
            SendACK();
            HandleSerialCommand(pSerialRxBuff[1], (CMD_BUFFER_P)&pSerialRxBuff[2], pSerialRxBuff[0]);
            break;

        case stateFrameErr:
            SendNACK();
            break;
 
        case stateRxTimeout:
            SendNACK();
            break;

        default:
            break;
        }
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
StartTimerCheckAck(void) {
    if (g_bCheckAckTimerHandle == NO_TIMER) {
        g_bCheckAckTimerHandle = 
        ZW_TIMER_START(CheckAck, WAIT_ACK_TIMEOUT, TIMER_ONE_TIME);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
StopTimerCheckAck(void) {
    if (g_bCheckAckTimerHandle != NO_TIMER) {
        ZW_TIMER_CANCEL(g_bCheckAckTimerHandle);
        g_bCheckAckTimerHandle = NO_TIMER;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
PCB(CheckAck) (void) {
    bTxActive = FALSE;
    resState = resNot;
	g_bCheckAckTimerHandle = NO_TIMER;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
RES_STATE
GetRespond (void) {
    if (bTxActive) 
        return resCheck;

    return resState;
}

