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
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 29/08/17   09:10 $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "utilities.h"
#include "debug.h"
#include "buff.h"
#include "uart.h"
#include "stdio.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define SIZE_BUFF_DBG               8
#define DEBUG_BAUD                  BAUD57600
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BYTE pBuffDataDbg[SIZE_BUFF_DBG];
static buffqueue rxQueueDatDbg;
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
 * @func   InitDebug
 * @brief  None
 * @param  None
 * @retval None
 */
void 
InitDebug(void) {
    bufInit(pBuffDataDbg, &rxQueueDatDbg, sizeof(pBuffDataDbg[0]), SIZE_BUFF_DBG);
    UART_RegBuffer(UART_DEBUG, &rxQueueDatDbg);
    UART_Init(UART_DEBUG, DEBUG_BAUD, NO_PARITY, ONE_STOP_BIT);
    UART_SetTransmitter(UART_DEBUG, TRUE);
    UART_SetReciver(UART_DEBUG, FALSE);
    UART_SetRxIrq(UART_DEBUG, FALSE);
}

/**
 * @func   TestHardFault
 * @brief  None
 * @param  None
 * @retval None
 */
void
TestHardFault(void) {
    u8_p pTest;
    *pTest = 0; /* Generate hardfault */
};

#ifdef USE_DEBUG

struct __FILE {
  int handle;
};

FILE __stdout;  
FILE __stdin;  

/**
 * @func   retarget printf
 * @brief  None
 * @param  None
 * @retval None
 */
int 
fputc(
    int ch, 
    FILE *f
) {
    UART_PutChar(UART_DEBUG,(u8_t)ch);
    return ch;
}

#endif /* USE_DEBUG */

/**
 * @func   Debug_SendStr
 * @brief  None
 * @param  None
 * @retval None
 */
void
Debug_SendStr(
    char *pString
) {
    while (*pString != NULL) {
        UART_PutChar(UART_DEBUG, *pString);
        pString++;
    }  
}

/**
 * @func   Debug_SendHex
 * @brief  Send number type hex
 * @param  None
 * @retval None
 */
void
Debug_SendHex(
    BYTE byData
) {
    UART_PutChar(UART_DEBUG, hex2Char((byData & 0xF0) >> 4));
    UART_PutChar(UART_DEBUG, hex2Char((byData & 0x0F)));
}

/**
 * @func   Debug_SendArrHex
 * @brief  None
 * @param  None
 * @retval None
 */
void
Debug_SendArrHex(
    BYTE_p pData,
    BYTE size
) {
    for (BYTE i = 0; i < size; i++) {
        Debug_SendHex(pData[i]);
        Debug_SendStr(" ");
    }
}

/**
 * @func   Debug_SendNum
 * @brief  None
 * @param  None
 * @retval None
 */
void
Debug_SendNum(
    DWORD dwNum
) {
    BYTE i = 0;
    BYTE buff[16];
    DWORD dwTemp = dwNum;

    while(1) {
        buff[i++] = dwTemp%10;
        dwTemp /= 10;
        if (dwTemp  == 0) {
            buff[i++] = dwTemp;
            break;
        }
    }

    while(i-- >1) {
        UART_PutChar(UART_DEBUG, buff[i-1] + '0');
    }    
}

/* END FILE */
