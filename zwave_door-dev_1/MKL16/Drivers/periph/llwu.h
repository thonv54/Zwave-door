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
 * Last Changed By:  $Author:  $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $thangdh Date: 22/08/17   11:00 $
 *
 ******************************************************************************/
#ifndef _LLWU_H_
#define _LLWU_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef enum __WU_PIN_TRIGGER__ {
    WUDisable = 0,
    WURisingEdge = 1,
    WUFallingEdge = 2,
    WURisingFalingEdge = 3,
}  wuTrigger;

typedef enum __LLWU_MODUL__ {
    WU_LPTMR = 0x01,
    WU_CMP0 = 0x02,
    WU_TSI0 = 0x10,
    WU_RTCA = 0x20,
    WU_RTCS = 0x80,
} wuModul;
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
 * @func   LLWU_SetPinWakeup
 * @brief  None
 * @param  None
 * @retval None
 */    
void
LLWU_SetPinWakeup(
    DWORD pinWakeup,
    wuTrigger triggerSource
);

/**
 * @func   LLWU_SetModulWakeup
 * @brief  None
 * @param  None
 * @retval None
 */
void
LLWU_SetModulWakeup(
    wuModul modulWakeup,
    FunctionalState NewState
);

/**
 * @func   LLWU_GetModulITStatus
 * @brief  None
 * @param  None
 * @retval 
 */
ITStatus
LLWU_GetModulITStatus(
    wuModul modulWakeup    
);

/**
 * @func   LLWU_GetPinITStatus
 * @brief  None
 * @param  None
 * @retval
 */
ITStatus
LLWU_GetPinITStatus(
    DWORD pinWakeup
);

/**
 * @func   LLWU_ClearPinITPending
 * @brief  None
 * @param  None
 * @retval None
 */
void
LLWU_ClearPinITPending(
    DWORD pinWakeup
);

/**
 * @func   LLWU_Configurefilter
 * @brief  None
 * @param  None
 * @retval
 */
void
LLWU_Configurefilter(
    DWORD wu_pin_num,
    BYTE filter_en,
    BYTE rise_fall
);

#endif /* END FILE */
