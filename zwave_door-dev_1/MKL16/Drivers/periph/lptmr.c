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
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "driver_common.h"
#include "lptmr.h"
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
 * @func   CLOCK_EnableLptmrClock
 * @brief  None
 * @param  None
 * @retval
 */
void
CLOCK_EnableLptmrClock(void) {
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
LPTMR_Init(
    DWORD count,
    DWORD clock_source
) {
    CLOCK_EnableLptmrClock();
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;   // LPTMR Disable

    LPTMR0->PSR = ( LPTMR_PSR_PRESCALE(0) // 0000 is div 2
                  | LPTMR_PSR_PBYP_MASK   // LPO feeds directly to LPT
                  | LPTMR_PSR_PCS(1)) ;   // use the choice of clock
      
    LPTMR0->CMR = LPTMR_CMR_COMPARE(count);  //Set compare value

    LPTMR0->CSR =  LPTMR_CSR_TCF_MASK   // Clear any pending interrupt
                  | LPTMR_CSR_TPS(0)     // TMR pin select
                  |!LPTMR_CSR_TPP_MASK   // TMR Pin polarity
                  |!LPTMR_CSR_TFC_MASK   // Timer Free running counter is reset whenever TMR counter equals compare
                  |!LPTMR_CSR_TMS_MASK;   // LPTMR0 as Timer
               

    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;   // Turn on LPT and start counting
//    LPTMR_SetIrq(ENABLE);  
//    NVIC_EnableIRQ(LPTimer_IRQn);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LPTMR_DelayMs(
    DWORD count_val
) {
    /* Make sure the clock to the LPTMR is enabled */
    SIM->SCGC5|=SIM_SCGC5_LPTMR_MASK;

    /* Reset LPTMR settings */
    LPTMR0->CSR=0;

    /* Set the compare value to the number of ms to delay */
    LPTMR0->CMR = count_val;

    /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
    LPTMR0->PSR = LPTMR_PSR_PCS(1)|LPTMR_PSR_PBYP_MASK;

    /* Start the timer */
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;

    /* Wait for counter to reach compare value */
    while (!(LPTMR0->CSR & LPTMR_CSR_TCF_MASK));

    /* Disable counter and Clear Timer Compare Flag */
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;

    return;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LPTMR_Reset(void) {
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
LPTMR_ClearITPendingBit(void) {
    /* Clear lptmr flag interrup */
    LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LPTMR_Cmd(
    FunctionalState NewState
) {
    if (NewState == ENABLE) {
        LPTMR0->CSR |= LPTMR_CSR_TEN_MASK; 
    }
    else if (NewState == DISABLE) {
        LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK; 
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LPTMR_SetIrq (
    FunctionalState NewState
) {
    if (NewState == ENABLE) {
        LPTMR0->CSR |= LPTMR_CSR_TIE_MASK; 
    }
    else if (NewState == DISABLE) {
        LPTMR0->CSR &= ~LPTMR_CSR_TIE_MASK; 
    }   
}
/* END FILE */
