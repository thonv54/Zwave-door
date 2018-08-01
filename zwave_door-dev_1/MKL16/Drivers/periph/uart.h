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
 * Last Changed By:  ThangDH $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 29/08/17 9:00 $
 *
 ******************************************************************************/
#ifndef _UART_H_
#define _UART_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "configboard.h"
#include "driver_common.h"
#include "buff.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifndef DRV_UART1_IDX
#define DRV_UART0_IDX               0
#define DRV_UART1_IDX               1
#define DRV_UART2_IDX               2
#endif /* DRV_UART1_IDX */

#define COUNT_UART                  3

#define BAUD9600                    9600
#define BAUD19200                   19200
#define BAUD38400                   38400
#define BAUD57600                   57600
#define BAUD115200                  115200

#define NO_PARITY                   0
#define EVEN_PARITY                 1
#define ODD_PARITY                  2

#define ONE_STOP_BIT                0
#define TWO_STOP_BIT                1

#define IS_BAUD(x)              (((x) == BAUD9600) || ((x) == BAUD19200) || \
                                 ((x) == BAUD38400) || ((x) == BAUD57600) || \
                                 ((x) == BAUD115200))

#define IS_PARITY(x)            (((x) == NO_PARITY) || ((x) == EVEN_PARITY) || \
                                 ((x) == ODD_PARITY))

#define IS_STOP_BIT(x)          (((x) == ONE_STOP_BIT) || ((x) == TWO_STOP_BIT))
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   UART_RegBuffer
 * @brief  Register buffer  
 * @param  None
 * @retval None
 */
void
UART_RegBuffer(
    BYTE byUartNumber,
    buffqueue_p pQueue 
);

/**
 * @func   UART_Init
 * @brief  Init Uart 
 * @param  None
 * @retval None
 */
void
UART_Init(
    BYTE byUartNumber,
    DWORD dwBaudRate,
    BYTE byParity,
    BYTE byStopBit
);

/**
 * @func  
 * @brief  None  
 * @param  None
 * @retval None
 */
void
UART_DeInit(
    BYTE byUartNumber
);

/**
 * @func   SetTransmitter
 * @brief  None  
 * @param  None
 * @retval None
 */
void
UART_SetTransmitter(
    BYTE byUartNumber,
    BOOL bState   /* IN: TRUE: Enble */
                   /* FALSE: Disable */
);

/**
 * @func   SetReciver
 * @brief  
 * @param  None
 * @retval None
 */
void
UART_SetReciver(
    BYTE byUartNumber,
    BOOL bState    /* IN: TRUE: Enble */
                     /* FALSE: Disable */
);

/**
 * @func   EnableIrqUart
 * @brief  None  
 * @param  None
 * @retval None
 */
void
UART_SetRxIrq(
    BYTE byUartNumber,
    BOOL bState
);

/**
 * @func   UartOpen
 * @brief  None
 * @param  None
 * @retval None
 */
void
UART_Open(
    BYTE byUartNumber
);

/**
 * @func   UartClose
 * @brief  None
 * @param  
 * @retval None
 */
void
UART_Close(
    BYTE byUartNumber
);

/**
 * @func  
 * @brief  None  
 * @param  None
 * @retval None
 */
BYTE
UART_GetChar(
    BYTE byUartNumber
);

/**
 * @func  
 * @brief  None  
 * @param  None
 * @retval None
 */
void
UART_PutChar(
    BYTE byUartNumber,
    BYTE byData
);

/**
 * @func   UartPutBuffer
 * @brief  None
 * @param  None
 * @retval None
 */
void
UART_PutBuffer(
    BYTE byUartNumber,
    BYTE_p pbyDataOut,
    WORD length
);

#endif /* END FILE */
