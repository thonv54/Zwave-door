/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: 
 *
 * Last Changed By:  ThangDH $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 29/08/17 9:00 $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "uart.h"
#include "gpio.h"
#include "sim.h"
#include "pincommon.h"
#include "utilities.h"
#include <stdio.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define CPU_INT_FAST_CLK_HZ         4000000

typedef struct _uart_ {
    u32 tx;
    u32 rx;
} uart_t, *uart_p;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static void_p g_pUartQueue[UART_COUNT] = { NULL };
static uart_t const g_uartPin[UART_COUNT] = { UART_PIN };
static IRQn_Type g_uartIRQ[UART_COUNT] = { UART0_IRQn, UART1_IRQn, UART2_IRQn};
static BOOL g_uartIsInit[UART_COUNT];
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void UART_DRIVER_IRQHandler(BYTE byUartNumber);
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
) {
    g_pUartQueue[byUartNumber] = pQueue;  
}

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
) {
    DWORD dwSBR = 0;

    if ((byUartNumber >= COUNT_UART) || !IS_STOP_BIT(byStopBit) || \
        !IS_BAUD(dwBaudRate) || !IS_PARITY(byParity))
        return ;

    CLOCK_SetUartClock(byUartNumber, ENABLE);
    CLOCK_EnablePortClock(EXTRACT_PORT(g_uartPin[byUartNumber].tx));
    pinmap_peripheral(g_uartPin[byUartNumber].tx, PinMap_UART_TX);
    
    CLOCK_EnablePortClock(EXTRACT_PORT(g_uartPin[byUartNumber].rx));
    pinmap_peripheral(g_uartPin[byUartNumber].rx, PinMap_UART_RX);

    if (byUartNumber == 0) 
    {
        UART0->C2 &= ~UART0_C2_RE_MASK;
        if ((MCG->C1 & MCG_C1_CLKS_MASK) != MCG_C1_CLKS(0)) { /* PLL/FLL is not selected */
            CLOCK_SelectUart0Clock(UART0SRC_MCGIRCLK);   
            UART0->C4 = UART0_C4_OSR(3); /* */
            UART0->C5 |= UART0_C5_BOTHEDGE_MASK;
            dwSBR = (CPU_INT_FAST_CLK_HZ >> 2) / dwBaudRate;
        }
        else {
            CLOCK_SelectUart0Clock(UART0SRC_PLL_FLL);  
            UART0->C4 = UART0_C4_OSR(7); /* */
            UART0->C5 |= UART0_C5_BOTHEDGE_MASK;
            dwSBR = (SystemBusClock >> 1) /dwBaudRate;
        }

        UART0->BDL = LO_UINT16(dwSBR); /* Set Baud rate */
        UART0->BDH = HI_UINT16(dwSBR);     
        UART0->C1 &= ~UART0_C1_M_MASK; /* Set Mode 8 bit */
        if (NO_PARITY == byParity)     /* Parity */
            UART0->C1 &= ~UART0_C1_PE_MASK;
        else 
        {
            UART0->C1 |= UART0_C1_PE_MASK;

            if (EVEN_PARITY == byParity)
                UART0->C1 &= ~UART0_C1_PT_MASK;
            else if (ODD_PARITY == byParity)
                UART0->C1 |= UART0_C1_PT_MASK;
        }
        /* Stopbit */
        if (ONE_STOP_BIT == byStopBit) {
            UART0->BDH &= ~UART0_BDH_SBNS_MASK;
        }
        else if (TWO_STOP_BIT == byStopBit) {
            UART0->BDH |= UART0_BDH_SBNS_MASK;
        }      
    }
    else 
    {
        dwSBR = (SystemBusClock >> 4) / dwBaudRate;        
        g_uartbase[byUartNumber]->BDL = dwSBR & 0xFF;         /* Set UART baud rate */
        g_uartbase[byUartNumber]->BDH = (dwSBR >> 8) & 0xFF;  
        g_uartbase[byUartNumber]->C1 &= ~UART_C1_M_MASK;      /* Set Mode 8 bit */

        if (NO_PARITY == byParity)
            g_uartbase[byUartNumber]->C1 &= ~UART_C1_PE_MASK;
        else 
        {
            g_uartbase[byUartNumber]->C1 |= UART_C1_PE_MASK;
            if (EVEN_PARITY == byParity)
                g_uartbase[byUartNumber]->C1 &= ~UART_C1_PT_MASK;
            else if (ODD_PARITY == byParity)
                g_uartbase[byUartNumber]->C1 |= UART_C1_PT_MASK;
        }

        if (ONE_STOP_BIT == byStopBit) {
            g_uartbase[byUartNumber]->BDH &= ~ UART_BDH_SBNS_MASK;
        }
        else if (TWO_STOP_BIT == byStopBit) {
            g_uartbase[byUartNumber]->BDH |= UART_BDH_SBNS_MASK;
        }
    }
    
    g_uartIsInit[byUartNumber] = TRUE;
    NVIC_EnableIRQ(g_uartIRQ[byUartNumber]);
}

/**
 * @func   UART_DeInit 
 * @brief  None  
 * @param  None
 * @retval None
 */
void
UART_DeInit(
    BYTE byUartNumber
) {  
    if (!g_uartIsInit[byUartNumber]) return;
    GPIO_DeInitPin(g_uartPin[byUartNumber].tx);
    GPIO_DeInitPin(g_uartPin[byUartNumber].rx); 
    CLOCK_SetUartClock(byUartNumber, DISABLE);
}

/**
 * @func   SetTransmitter
 * @brief  None
 * @param  None
 * @retval None
 */
void
UART_SetTransmitter(
    BYTE byUartNumber,
    BOOL bState                      
) {
    if (byUartNumber >= COUNT_UART)
        return ;

    else if (byUartNumber == 0) 
    {
        if (bState)
            UART0->C2 |= UART0_C2_TE_MASK;
        else
            UART0->C2 &= ~UART0_C2_TE_MASK;
    }
    else 
    {
        if (bState)
            g_uartbase[byUartNumber]->C2 |= UART_C2_TE_MASK;
        else
            g_uartbase[byUartNumber]->C2 &= ~UART_C2_TE_MASK;
    }
}

/**
 * @func   UART_SetReciver
 * @brief  None
 * @param  None
 * @retval None
 */
void
UART_SetReciver(
    BYTE byUartNumber,
    BOOL bState 
) {
    if (byUartNumber >= COUNT_UART)
        return ;

    else if (byUartNumber == 0) 
    {
        if (bState)
            UART0->C2 |= UART0_C2_RE_MASK;
        else
            UART0->C2 &= ~UART0_C2_RE_MASK;
    }
    else 
    {
        if (bState)
            g_uartbase[byUartNumber]->C2 |= UART_C2_RE_MASK;
        else
            g_uartbase[byUartNumber]->C2 &= ~UART_C2_RE_MASK;
    }
}

/**
 * @func   UART_SetRxIrq
 * @brief  None
 * @param  None
 * @retval None
 */
void
UART_SetRxIrq(
    BYTE byUartNumber,
    BOOL bState
) {
    if (byUartNumber >= COUNT_UART)
        return ;

    else if (byUartNumber == 0) 
    {
        if (bState)
            UART0->C2 |= UART0_C2_RIE_MASK;
        else
            UART0->C2 &= ~UART0_C2_RIE_MASK;
    }
    else 
    {
        if (bState)
            g_uartbase[byUartNumber]->C2 |= UART_C2_RIE_MASK;
        else
            g_uartbase[byUartNumber]->C2 &= ~UART_C2_RIE_MASK;
    }
}

/**
 * @func   UartOpen
 * @brief  None
 * @param  Enable Transmit and receiver
 * @retval None
 */
void
UART_Open(
    BYTE byUartNumber
) {
    if (byUartNumber >= COUNT_UART)  return ;

    UART_SetTransmitter(byUartNumber, TRUE);
    UART_SetReciver(byUartNumber, TRUE);
}

/**
 * @func   UartClose
 * @brief  None
 * @param  Disable Transmit and receiver
 * @retval None
 */
void
UART_Close(
    BYTE byUartNumber
) {
    if (byUartNumber >= COUNT_UART)
        return ;

    UART_SetTransmitter(byUartNumber, FALSE);
    UART_SetReciver(byUartNumber, FALSE);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
UART_GetChar(
    BYTE byUartNumber
) {
    if (byUartNumber == 0) 
    {
        while(!(UART0->S1 & UART0_S1_RDRF_MASK));
        /* Read the character */
        return UART0->D;
    }

    while(!(g_uartbase[byUartNumber]->S1 & UART_S1_RDRF_MASK));
    /* Read the character */
    return g_uartbase[byUartNumber]->D;	
}

/**
 * @func   UART_PutChar
 * @brief  None
 * @param  None
 * @retval None
 */
void
UART_PutChar(
    BYTE byUartNumber,
    BYTE byData
) {
    if ((byUartNumber >= COUNT_UART) | (!g_uartIsInit[byUartNumber]))
        return ;
   
    if (byUartNumber == 0) 
    {
        /* Wait until space is available in the FIFO */
        while(!(UART0->S1 & UART0_S1_TDRE_MASK));
        /* Send the character */
        UART0->D = byData;

        while(!(UART0->S1 & UART0_S1_TC_MASK));

        return ;
    }

    /* Wait until space is available in the FIFO */
    while(!(g_uartbase[byUartNumber]->S1 & UART_S1_TDRE_MASK));
    /* Send the character */
    g_uartbase[byUartNumber]->D = byData;

    while(!(g_uartbase[byUartNumber]->S1 & UART_S1_TC_MASK));
}

/**
 * @func   UartPutBuffer
 * @brief  None
 * @param  None
 * @retval None
 */
void
UART_PutBuffer(
    BYTE byUartNumber,
    u8_p pbyDataOut,
    WORD byLength
) {
    WORD i;

    for (i = 0; i < byLength; i++)
        UART_PutChar(byUartNumber, pbyDataOut[i]);
}

/**
 * @func   UART0_IRQHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void
UART0_IRQHandler() {
    ENTER_CRITCAL;
    if ((UART0->C2 & UART_C2_RIE_MASK) && \
        (UART0->S1 & UART_S1_RDRF_MASK)) 
    {        
        buffqueue_p pUartBuffQueue = (buffqueue_p) g_pUartQueue[UART0_IDX];
        BYTE byReceiverData = UART_GetChar(UART0_IDX);         
        if (bufEnDat(pUartBuffQueue, &byReceiverData) == ERR_BUF_FUL) {
        }                
    }
    EXIT_CRITICAL;
}

 /**
 * @func   UART2_IRQHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void
UART2_IRQHandler() {
    UART_DRIVER_IRQHandler(UART2_IDX); 
}

/**
 * @func   UART2_IRQHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void
UART1_IRQHandler() {
    UART_DRIVER_IRQHandler(UART1_IDX); 
}

/**
 * @func   UART_DRIVER_IRQHandler
 * @brief  None
 * @param  None
 * @retval None
 */
static void
UART_DRIVER_IRQHandler(
    BYTE byUartNumber
) {
    ENTER_CRITCAL;
    if ((g_uartbase[byUartNumber]->C2 & UART_C2_RIE_MASK) && \
        (g_uartbase[byUartNumber]->S1 & UART_S1_RDRF_MASK)) 
    {        
        buffqueue_p pUartBuffQueue = (buffqueue_p) g_pUartQueue[byUartNumber];
        BYTE byReceiverData = UART_GetChar(byUartNumber);         
        if (bufEnDat(pUartBuffQueue, &byReceiverData) == ERR_BUF_FUL) {
        }                
    }
    EXIT_CRITICAL;
}

/* END FILE */
