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
 * Last Changed By:  $Author:  $
 * Revision:         $Revision:  $
 * Last Changed:     $Date: $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "utilities.h"
#include "serial_cmd.h"
#include "serial.h"
#include "debug.h"
#include "timer.h"
#include "uart.h"
#include "buff.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef SERIAL_DEBUG
#define DBG_SER_SEND_STR(x)         Debug_SendStr(x)
#define DBG_SER_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_SER_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_SER_SEND_STR(x)
#define DBG_SER_SEND_NUM(x)
#define DBG_SER_SEND_HEX(x) 
#endif

#define SIZE_BUFF_DATA                      64
#define RX_BUFFER_SIZE                      64
#define TX_BUFFER_SIZE                      64

#define RlengthBuf                          (*pSerialRxBuff)
/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/
static BYTE byRxBufState;
static BYTE byIndexRxBuf;
static BYTE byCheckSumRxBuf;
static DWORD wTimeoutRx;
static BOOL bRxActive = FALSE;
static BOOL bTxActive = FALSE;

static RES_STATE resState = resNot;
static BYTE byTimerCheckRes = NO_TIMER;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
BYTE pSerialRxBuff[RX_BUFFER_SIZE] = {0};

static BYTE pBuffDataSerial[SIZE_BUFF_DATA];
static buffqueue serialQueueDat;
static BOOL fl = FALSE;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void TimerReset(u32_p pdwTimerStartTick);
static WORD TimerGetElapsedTime(DWORD dwTimerStartTick);
static void SendACK(void);
static void SendNACK(void);
void SerialFreeTxState(void);
static void SerialAckTimemout(void_p pRespond);
static BYTE_CALLBACKFUNC txResultCallback = NULL;
static BOOL SerialRequesTxState(BYTE_CALLBACKFUNC completeFunc);
static BYTE GetFuncId (BYTE cmdType);
static BYTE PollRxBuff(void);
static WORD_CALLBACKFUNC pSetTimeoutCallbackFunc = NULL;

extern void
HandleSerialCommand(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length
); 
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   TimerReset   
 * @brief  Reset startTick to current timer tick
 * @param  u32_p pWTimerStartTick
 * @retval None
 */
void
TimerReset(
    u32_p pdwTimerStartTick
) {
    *pdwTimerStartTick = GetMilSecTick();
}

/**
 * @func   TimerGetElapsedTime   
 * @brief  Get elapsed time  
 * @param  DWORD wTimerStartTick
 * @retval DWORD Elapsed Time in 10 ms ticks
 */
WORD
TimerGetElapsedTime(
    DWORD dwTimerStartTick
) {
    return (GetMilSecTick() - dwTimerStartTick);
}

/**
 * @func   InitSerial   
 * @brief  Initialize UART
 * @param  WORD wBaudRate
 * @retval None
 */
void
InitSerial() {
    bufInit( pBuffDataSerial, &serialQueueDat, sizeof(pBuffDataSerial[0]), SIZE_BUFF_DATA);
    UART_RegBuffer(UART_ZW, &serialQueueDat);
    UART_Init(UART_ZW, BAUD57600, NO_PARITY, ONE_STOP_BIT);
    UART_Open(UART_ZW);
    UART_SetRxIrq(UART_ZW, ENABLE);
    byRxBufState = stateIdentifyFrame;
}

/**
 * @func   GetFuncId
 * @brief  None
 * @param  None
 * @retval FuncID
 */
BYTE 
GetFuncId (
    BYTE cmdType
) {
    BYTE funcID;
    static BYTE byFuncIdMZ = FUNID_MZ_MIN;

    switch(cmdType) {
    case CMD_TYPE_UPD:
    case CMD_TYPE_SET:
    case CMD_TYPE_GET:
        byFuncIdMZ++;
        if (byFuncIdMZ >= FUNID_MZ_MAX) {
            byFuncIdMZ  = FUNID_MZ_MIN;
        }
        funcID = byFuncIdMZ;
        break;

    case CMD_TYPE_RES:
//        funcID = RfuncId;
        break;
    }

    return funcID;
}

/**
 * @func   InitSerialProc  
 * @brief  None
 * @param  Register call back process data receiver
 * @retval None
 */
void
InitSerialProc(
    WORD_CALLBACKFUNC pSetTimeoutFunc
) {
    pSetTimeoutCallbackFunc = pSetTimeoutFunc;
}

/**
 * @func   SerialPWTimeoutKick   
 * @brief  None  
 * @param  None  
 * @retval None
 */
void
SerialPWTimeoutKick() {
    if (pSetTimeoutCallbackFunc != NULL) {
        pSetTimeoutCallbackFunc(PWTIMEOUT_200_MSEC);
    }
}

/**
 * @func   PollRxBuff   
 * @brief  None  
 * @param  None  
 * @retval None
 */
BYTE
PollRxBuff() {
    BYTE byRetVal = stateIdle;
    BYTE byRxData;
    BOOL bFrameErr = FALSE;    

    while((bufNumEle(&serialQueueDat) != 0) && (byRetVal == stateIdle))
    {
        bufDeDat(&serialQueueDat, &byRxData);     
        DBG_SER_SEND_HEX(byRxData);
        DBG_SER_SEND_STR(" ");
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
            } else {
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
    }
    
    if (byRetVal == stateIdle) {
        if (bRxActive && (TimerGetElapsedTime(wTimeoutRx) >= RX_TIMEOUT)) {
            byRetVal = stateRxTimeout;
            bRxActive = FALSE;
            byRxBufState = stateIdentifyFrame;
        }
    }
    
    if (bFrameErr) 
        return stateFrameErr;

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
    BYTE_p pframebuff,
    BYTE length,
    BYTE_CALLBACKFUNC func
) {
    BYTE i;
    BYTE csum = CSUM_INIT_VAL;
    BOOL txState = FALSE;
    BYTE cmdlength = length + 5;
    BYTE pSerialTxBuff[TX_BUFFER_SIZE];
    CMD_BUFFER_P pCmd = (CMD_BUFFER_P) pframebuff;

//    memsetl(pSerialTxBuff, 0, TX_BUFFER_SIZE);
    txState = SerialRequesTxState(func);
    if (!txState) return FALSE;

    pSerialTxBuff[0] = FRAME_SOF;
    pSerialTxBuff[1] = length + 3; /* Option BYTE + FuncID BYTE + Checksum BYTE */
    pSerialTxBuff[2] = option;
    memcpyl(&pSerialTxBuff[3], pframebuff, length);
    pSerialTxBuff[cmdlength - 2] = GetFuncId(pCmd->scommon.cmdtype); /* FuncId */

    for (i = 2; i < cmdlength - 1; i++) {
        csum ^= pSerialTxBuff[i];
    }
   
    pSerialTxBuff[cmdlength - 1] = csum; /* Checksum */

    for (i = 0; i < cmdlength; i++) {
        UART_PutChar(UART_ZW, pSerialTxBuff[i]);
    }

    if (byTimerCheckRes == NO_TIMER)
        byTimerCheckRes = TimerStart(WAIT_RES_TIMEOUT, TIMER_ONE_TIME, SerialAckTimemout, NULL);
    else 
        TimerRestart(byTimerCheckRes, WAIT_RES_TIMEOUT, TIMER_ONE_TIME);

    SerialPWTimeoutKick();

    return TRUE;
}

/**
 * @func   SerialFreeTxState
 * @brief  None
 * @param  None
 * @retval None
 */
void
SerialFreeTxState (void) {
    bTxActive = FALSE;
    txResultCallback = NULL;
}

/**
 * @func   SerialRequesTxState
 * @brief  None
 * @param  None
 * @retval TRUE if TX state idle
 */
BOOL
SerialRequesTxState (
    BYTE_CALLBACKFUNC completeFunc
) {
    DBG_SER_SEND_STR("rquest tx state\n");

    if (bTxActive) {
        DBG_SER_SEND_STR("-busy\n");
        return FALSE;
    }

    txResultCallback = completeFunc;
    bTxActive = TRUE;
    return TRUE;
}

/**
 * @func   SerialAckTimemout
 * @brief  None
 * @param  None
 * @retval None
 */
void
SerialAckTimemout (
    void_p pRespond
) {
    BYTE respondState;
    BYTE result = JOB_FALSE;
    BYTE_p temp = (BYTE_p)pRespond;

    if (temp == NULL) {
      		DBG_SER_SEND_STR("-not res\n");
        respondState = resNot;
    }
    else {
        respondState = *temp;
    }

    if (respondState == resAck) {
        result = JOB_SUCCESS;
    }
    else {
        result = JOB_FALSE;
    }

    /* Cancel timer */
    TimerCancel(byTimerCheckRes);
    byTimerCheckRes = NO_TIMER;

    /* free tx state */
    if (bTxActive) {
        bTxActive = FALSE;
        if (txResultCallback != NULL) {
            BYTE_CALLBACKFUNC completeFunc = txResultCallback;
            txResultCallback = NULL;
            completeFunc(result);
        }
    }
}

/**
 * @func   SendACK
 * @brief  None
 * @param  None
 * @retval None
 */
void
SendACK() {
    DBG_SER_SEND_STR("- send ack\n");
    UART_PutChar(UART_ZW, FRAME_ACK);
}

/**
 * @func   SendNACK
 * @brief  None
 * @param  None
 * @retval None
 */
void
SendNACK() {
    DBG_SER_SEND_STR("- send Nack\n");
    UART_PutChar(UART_ZW, FRAME_NACK);
}

/**
 * @func   procSerial
 * @brief  None
 * @param  None
 * @retval None
 */
void
procSerial(void) {
    BYTE stateRx;

    stateRx = PollRxBuff();
    if (stateRx != stateIdle) {
        switch (stateRx) {
        case stateFrameACKReceived:
            DBG_SER_SEND_STR("-Ack\n");
            resState = resAck;
            SerialAckTimemout(&resState);
            break;

        case stateFrameNACKReceived:
            DBG_SER_SEND_STR("-Nack\n");
            resState = resNack;
            SerialAckTimemout(&resState);
            break;

        case stateFrameDataReceived: 
        {
            DBG_SER_SEND_STR("-rd\n");
            SendACK();
            HandleSerialCommand(pSerialRxBuff[1], (CMD_BUFFER_P)&pSerialRxBuff[2], pSerialRxBuff[0]);
        }   break;

        case stateFrameErr:
            if (fl) 
            {
               fl = FALSE;
               return;
            }

            SendNACK();
            break;
 
        case stateRxTimeout:
            DBG_SER_SEND_STR("-rx tmout\n");
            SendNACK();
            break;

        default:
            DBG_SER_SEND_STR("state invalid!\n");
            break;
        }
    }
}

/**
 * @func   SerialJobFinish
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
SerialJobFinish(void) {
    return (!bTxActive);
}
/* END FILE */
