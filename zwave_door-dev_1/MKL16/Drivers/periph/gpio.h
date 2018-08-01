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
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
#ifndef _GPIO_H_
#define _GPIO_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
#include "pincommon.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define GIPO_IRQ_DISABLE            0
#define GPIO_IRQ_RISING             9 
#define GPIO_IRQ_FALLING            10
#define GPIO_IRQ_ANY                11
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
 * @func   GPIO_SetPinIsOutput
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_SetPinIsOutput(
    WORD IoPin
);

/**
 * @func   GPIO_SetPinIsInput
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_SetPinIsInput(
    WORD IoPin,
    BYTE pullMode
);

/**
 * @func   GPIO_DeInitPin
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_DeInitPin(
    WORD IoPin
);

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_PinHigh (
    WORD IoPin
);

/**
 * @func   GPIO_PinLow
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_PinLow (
    WORD IoPin
);

/**
 * @func   GPIO_PinToggle
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_PinToggle(
    WORD IoPin
);

/**
 * @func   GPIO_PinReadInput 
 * @brief  None
 * @param  None
 * @retval BOOL
 */
BOOL
GPIO_PinReadInput(
    WORD IoPin
);

/**
 * @func   GPIO_SetPinIrq
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_SetPinIrq(
    WORD IoPin,
    BYTE byModeIrq
);

/**
 * @func   GPIO_GetITStatus  
 * @brief  None
 * @param  None
 * @retval SET OR RESET
 */
ITStatus
GPIO_GetITStatus(
    WORD IoPin
);

/**
 * @func   GPIO_ClearITPendingBit
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_ClearITPendingBit(
    WORD IoPin
);

#endif /* END FILE */
