/*******************************************************************************
 *
 * Copyright (c) 2017
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: ThangDH
 *
 * Last Changed By:  $Author: thangdh $
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "driver_common.h"
#include "sim.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define SIM_CLKDIV1                 ((SIM)->CLKDIV1)
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
 * @func   CLOCK_EnablePortClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_EnablePortClock(
    BYTE portNum
) {
    SIM->SCGC5 |= g_portclock[portNum];
}

/**
 * @func   CLOCK_SelectTPMClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SelectTPMClock(
    BYTE srcClock
) {
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(srcClock); 
}

/**
 * @func   CLOCK_EnableTpmClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_EnableTpmClock(
    BYTE tpmNum
) {
    SIM->SCGC6	|= g_tpmclock[tpmNum];
}

/**
 * @func   CLOCK_SetUartClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SetUartClock(
    BYTE uartNum,
    FunctionalState newState
) {
    if (newState) {
        SIM->SCGC4 |= g_uartclock[uartNum];
    } 
    else {
        SIM->SCGC4 &= ~g_uartclock[uartNum];
    }
}

/**
 * @func   CLOCK_SelectUart0Clock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SelectUart0Clock(
    BYTE srcClock
) {
    SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(srcClock); 
}

/**
 * @func   CLOCK_EnableRtcClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_EnableRtcClock(void) {
    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
}

/**
 * @func   CLOCK_SelectOSC32Clock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SelectOSC32Clock(
    BYTE srcClock
) {
    SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK;
    SIM->SOPT1 |=  SIM_SOPT1_OSC32KSEL(srcClock);
}

/**
 * @func   CLOCK_EnableTsiClock   
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_EnableTsiClock(void) {
    SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;    
}

/**
 * @func   CLOCK_SetI2CClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SetI2CClock(
    BYTE i2cNum,
    FunctionalState newState
) {
    if (newState)
        SIM->SCGC4 |= g_i2cclock[i2cNum];
    else {
        SIM->SCGC4 &= ~g_i2cclock[i2cNum];
    }
}

/**
 * @func   CLOCK_SetSpiClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SetSpiClock(
    BYTE spiNum,
    FunctionalState newState
) {
    if (newState) {
        SIM->SCGC4 |= g_spiclock[spiNum];
    }
    else {
        SIM->SCGC4 &= ~g_spiclock[spiNum];
    }
}

/**
 * @func   CLOCK_SetOutDiv
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SetOutDiv(
    BYTE outDiv1,
    BYTE outDiv4
) {
    SIM_CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(outDiv1) | SIM_CLKDIV1_OUTDIV4(outDiv4));
}

/* END FILE */ 
