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
 * Last Changed By:  $Author: thangdh $
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "driver_common.h"
#include "llwu.h"
#include "mcg.h"
#include "debug.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define LLWU_PE1                       ((LLWU)->PE1)
#define LLWU_PE2                       ((LLWU)->PE2)
#define LLWU_PE3                       ((LLWU)->PE3)
#define LLWU_PE4                       ((LLWU)->PE4)
#define LLWU_F2                        ((LLWU)->F2)
#define LLWU_F1                        ((LLWU)->F1)
#define LLWU_ME                        ((LLWU)->ME)
#define LLWU_FILT1                     ((LLWU)->FILT1)
#define LLWU_FILT2                     ((LLWU)->FILT2)

typedef struct {
    DWORD pin;
    BYTE LLWUpin;
} LLWUPinMap;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static const LLWUPinMap pinMapLLWU[] = {
    {PTB0,  5},
    {PTC1,  6},
    {PTC3,  7},
    {PTC4,  8},
    {PTC5,  9},
    {PTC6, 10},  
    {PTD4, 14},
    {PTD6, 15},
    {NC  ,  0}
}; 
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
) {
    switch(pinWakeup) {
    case PTB0:
        LLWU->PE2 &= ~LLWU_PE2_WUPE5_MASK;
        LLWU->PE2 |= LLWU_PE2_WUPE5(triggerSource);
        break;

    case PTC1:
        LLWU->PE2 &= ~LLWU_PE2_WUPE6_MASK;
        LLWU->PE2 |= LLWU_PE2_WUPE6(triggerSource);
        break;

    case PTC3:
        LLWU->PE2 &= ~LLWU_PE2_WUPE7_MASK;
        LLWU->PE2 |= LLWU_PE2_WUPE7(triggerSource);
        break;

    case PTC4:
        LLWU->PE3 &= ~LLWU_PE3_WUPE8_MASK;
        LLWU->PE3 |= LLWU_PE3_WUPE8(triggerSource);
        break;

    case PTC5:
        LLWU->PE3 &= ~LLWU_PE3_WUPE9_MASK;
        LLWU->PE3 |= LLWU_PE3_WUPE9(triggerSource);
        break;

    case PTC6:
        LLWU->PE3 &= ~LLWU_PE3_WUPE10_MASK;
        LLWU->PE3 |= LLWU_PE3_WUPE10(triggerSource);
        break;

    case PTD4:
        LLWU->PE4 &= ~LLWU_PE4_WUPE14_MASK;
        LLWU->PE4 |= LLWU_PE4_WUPE14(triggerSource);
        break;

    case PTD6:
        LLWU->PE3 &= ~LLWU_PE4_WUPE15_MASK;
        LLWU->PE3 |= LLWU_PE4_WUPE15(triggerSource);
        break;

    default :
        break;
    }
}

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
) {
    if (NewState == ENABLE) 
        LLWU_ME |= modulWakeup;
    else 
        LLWU_ME &= ~modulWakeup;     
}

/**
 * @func   LLWU_GetPinITStatus
 * @brief  None
 * @param  None
 * @retval SET Or RESET
 */
ITStatus
LLWU_GetPinITStatus(
    DWORD pinWakeup
) {
    BYTE pinwu;
    BYTE i, temp = 0;

    for (i = 0; pinMapLLWU[i].pin != NC; i++) {
        if (pinWakeup == pinMapLLWU[i].pin) {
           pinwu = pinMapLLWU[i].LLWUpin;
           break;
        }
    }
  
    if (pinwu < 8) {
        if (LLWU->F1 & (1 << (pinwu))) 
            return SET;
        else 
            return RESET;
    }
    else if (pinwu < 16) {
        temp = pinwu - 8;
        if (LLWU->F2 & (1 << (temp))) 
            return SET;
        else 
            return RESET;       
    }    
    else {
    } 
    
     return RESET;
}

/**
 * @func   LLWU_ClearPinITPending
 * @brief  None
 * @param  [in]pinWakeup
 * @retval None
 */
void
LLWU_ClearPinITPending(
    DWORD pinWakeup
) {
    BYTE pinwu;
    BYTE i, temp = 0;

    for (i = 0; pinMapLLWU[i].pin != NC; i++) {
        if (pinWakeup == pinMapLLWU[i].pin) {
           pinwu = pinMapLLWU[i].LLWUpin;
           break;
        }
    }

    if (pinwu < 8) {
        LLWU->F1 |= (1 << (pinwu));
    }
    else if (pinwu < 16) {
        temp = pinwu - 8;
        LLWU->F2 |= (1 << (temp));   
    }    
    else {
    } 
}

/**
 * @func   LLWU_GetModulITStatus
 * @brief  None
 * @param  None
 * @retval None
 */
ITStatus
LLWU_GetModulITStatus(
    wuModul modulWakeup    
) {
    if (modulWakeup & LLWU->F3) 
       return SET; 
    else 
       return RESET;
}

/**
 * @func   LLWU_Configurefilter
 * @brief  None
 * @param  None
 * @retval None
 */
void
LLWU_Configurefilter(
    DWORD wu_pin_num,
    BYTE filter_en,
    BYTE rise_fall
) {
    // wu_pin_num is the pin number to be written to FILTSEL. 
    // wu_pin_num is not the same as pin_en. 
    BYTE temp;
    temp = 0;
    //first clear filter values and clear flag by writing a 1
    LLWU_FILT1 = LLWU_FILT1_FILTF_MASK;
    LLWU_FILT2 = LLWU_FILT2_FILTF_MASK;
     
    if(filter_en == 1) {
        // clear the flag bit and set the others
        temp |= (LLWU_FILT1_FILTF_MASK) | (LLWU_FILT1_FILTE(rise_fall) | LLWU_FILT1_FILTSEL(wu_pin_num));         
        LLWU_FILT1 = temp;
    }
    else if (filter_en == 2) {
        // clear the flag bit and set the others
        temp |= (LLWU_FILT2_FILTF_MASK) | (LLWU_FILT2_FILTE(rise_fall) | LLWU_FILT2_FILTSEL(wu_pin_num));         
        LLWU_FILT2 = temp;
    }
    else {
    }
}
/* END FILE */
