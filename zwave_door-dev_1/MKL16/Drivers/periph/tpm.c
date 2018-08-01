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
 * Last Changed By:  $Author: ThangDH $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date:  17/08/17 11:00 $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "tpm.h"
#include "sim.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static IRQn_Type g_tpmIRQ[TPM_COUNT] = { TPM0_IRQn, TPM1_IRQn, TPM2_IRQn};
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
 * @func   TPM_Init
 * @brief  None
 * @param  None
 * @retval None
 */
void
TPM_Init(
    BYTE byTimerNum,
    BYTE byClkMode,
    BYTE byPresscale,
    WORD wModulo
) {
    if (byTimerNum >= TPM_COUNT) return;   
    CLOCK_EnableTpmClock(byTimerNum);  /* Enable clock gate */
    g_tpmbase[byTimerNum]->SC |= TPM_SC_CMOD(byClkMode)|TPM_SC_PS(byPresscale);
    g_tpmbase[byTimerNum]->MOD = wModulo;
    NVIC_EnableIRQ(g_tpmIRQ[byTimerNum]);
}

/**
 * @func   TPM_SetOverflowIRQ
 * @brief  None
 * @param  None
 * @retval None
 */
void
TPM_SetOverflowIRQ(
    BYTE byTimerNum,
    BOOL bState
) {
    if (bState == ENABLE) { 
        /* Timer Overflow Interrupt Enable */
        g_tpmbase[byTimerNum]->SC |= TPM_SC_TOIE_MASK; 
    }
    else if (bState == DISABLE) { 
        /* Timer Overflow Interrupt Disable */
        g_tpmbase[byTimerNum]->SC &= ~TPM_SC_TOIE_MASK; 
    }
}


/**
 * @func   TPM_InitPWM
 * @brief  None
 * @param  None
 * @retval None
 */
void 
TPM_InitPWM(
    BYTE byTimerNum,
    BYTE byChannel
) {  
    TPM_CnSC_REG(g_tpmbase[byTimerNum], byChannel) |= TPM_PWM_H;
}

/**
 * @func   TPM_SetDuty
 * @brief  None
 * @param  None
 * @retval None
 */ 
void 
TPM_SetDuty(
    BYTE byTimerNum,
    BYTE byChannel,
    BYTE byDuty
) {
    WORD value;
    WORD wCountOverflow;
 
    wCountOverflow = g_tpmbase[byTimerNum]->MOD;     
    value = byDuty * wCountOverflow/100;
    g_tpmbase[byTimerNum]->SC &= TPM_SC_CMOD_MASK;
    TPM_CnV_REG(g_tpmbase[byTimerNum], byChannel) = value;
    g_tpmbase[byTimerNum]->SC |= TPM_SC_CMOD(TPM_CMOD_CLK);
}

/**
 * @func   TPM_GetITStatus
 * @brief  None
 * @param  None
 * @retval None
 */
ITStatus
TPM_GetITStatus(
    BYTE byTimerNum
) {
    if (g_tpmbase[byTimerNum]->SC & TPM_SC_TOF_MASK)
        return SET;

    return RESET;
}

/**
 * @func   TPM_ClearITPendingBit
 * @brief  None
 * @param  None
 * @retval None
 */
void
TPM_ClearITPendingBit(
    BYTE byTimerNum
) {
    g_tpmbase[byTimerNum]->SC |= TPM_SC_TOF_MASK;
}

/* END FILE */
