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
#ifndef _TPM_H_
#define _TPM_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
#include "driver_common.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define TPM_CMOD_DIS                0
#define TPM_CMOD_CLK                1
#define TPM_CMOD_EXTCLK             2

#define PS_1                        0
#define PS_2                        1
#define PS_4                        2
#define PS_8                        3
#define PS_16                       4
#define PS_32                       5
#define PS_64                       6
#define PS_128                      7

#define TPM_PWM_H                   TPM_CnSC_MSB_MASK|TPM_CnSC_ELSB_MASK
#define TPM_PWM_L                   TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK

#define TPM_CnSC_REG(base,index)    ((base)->CONTROLS[index].CnSC)
#define TPM_CnV_REG(base,index)     ((base)->CONTROLS[index].CnV)
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
);

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
);

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
);
 
/**
 * @func   TPM_SetDuty
 * @brief  None
 * @param  None
 * @retval None
**/
void 
TPM_SetDuty(
    BYTE byTimerNum,
    BYTE byChannel,
    BYTE byDuty
);

/**
 * @func   TPM_ClearITPendingBit
 * @brief  None
 * @param  None
 * @retval None
 */
void
TPM_ClearITPendingBit(
    BYTE byTimerNum
);

/**
 * @func   TPM_GetITStatus
 * @brief  None
 * @param  None
 * @retval None
 */
ITStatus
TPM_GetITStatus(
    BYTE byTimerNum
);

#endif /* END FILE */
