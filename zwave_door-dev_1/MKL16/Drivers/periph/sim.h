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
#ifndef _SIM_H_
#define _SIM_H_ 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define OSC32K_SYS_OSC32            0
#define OSC32K_RTC_CLKIN            2
#define OSC32K_LPO_1KHZ             3

#define TPM_PLLFLL                  1
#define TPM_OSCERCLK                2
#define TPM_MCGIRCLK                3

#define UART0SRC_MCGIRCLK           3
#define UART0SRC_OSCERCLK           2
#define UART0SRC_PLL_FLL            1
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
);

/**
 * @func   CLOCK_SelectTPMClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_EnableTpmClock(
    BYTE tpmNum
);

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
);

/**
 * @func   CLOCK_EnableRtcClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_EnableRtcClock(void);

/**
 * @func   CLOCK_SelectOSC32Clock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SelectOSC32Clock(
    BYTE srcClock
);

/**
 * @func   CLOCK_EnableTsiClock   
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_EnableTsiClock(void);

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
);

/**
 * @func   CLOCK_SelectTPMClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SelectTPMClock(
    BYTE srcClock
);

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
);

/**
 * @func   CLOCK_SelectUart0Clock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SelectUart0Clock(
    BYTE srcClock
);

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
);

#endif /* END FILE */
