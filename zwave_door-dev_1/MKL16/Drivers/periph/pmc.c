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
#include "configapp.h"
#include "pmc.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define PMC_REGSC                   ((PMC)->REGSC)
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
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
PMC_InitLVD(
    BYTE volSelect
) { 
    /* Disable LVD Reset */
   	PMC->LVDSC1 &= ~PMC_LVDSC1_LVDRE_MASK ;

    /* Selects the LVD trip point voltage */
    PMC->LVDSC1 |= PMC_LVDSC1_LVDV((volSelect & 0x4) >> 2);  
 
    /* Low-Voltage Warning Voltage Select */
    PMC->LVDSC2  = PMC_LVDSC2_LVWV(volSelect & 0x3);

    PMC->LVDSC1 |= PMC_LVDSC1_LVDACK_MASK; 
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
PMC_SetIrqLVD(
    BOOL state
) {
    if (state == ENABLE) {
        PMC->LVDSC1 |= PMC_LVDSC1_LVDIE_MASK;
    }
    else if (state == DISABLE) {
        PMC->LVDSC1 &= ~PMC_LVDSC1_LVDIE_MASK;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
PMC_SetIrqLVW(
    BOOL state
) {
    if (state == ENABLE) {
        PMC->LVDSC2 |= PMC_LVDSC2_LVWIE_MASK;
    }
    else if (state == DISABLE) {
        PMC->LVDSC2 &= ~PMC_LVDSC2_LVWIE_MASK;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void 
LVD_LVW_IRQHandler(void) {
    if (PMC->LVDSC1 &PMC_LVDSC1_LVDF_MASK){

    }
    if (PMC->LVDSC2 &PMC_LVDSC2_LVWF_MASK){

    }
 
    PMC->LVDSC1 |= PMC_LVDSC1_LVDACK_MASK; 
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
ITStatus
PMC_GetITStatus(
    BYTE byFlag
) {
    if (byFlag == LVD_FLAG) {    
       if (PMC->LVDSC1 & PMC_LVDSC1_LVDF_MASK) 
          return SET;    
       else 
          return RESET;      
    }  
    else if (byFlag == LVW_FLAG) {
        if (PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
          return SET;    
       else 
          return RESET; 
    }
    
    return RESET;     
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
BOOL
PMC_IsIsolatedState(void) {
    if (PMC->REGSC & PMC_REGSC_ACKISO_MASK)
        return TRUE;

    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
PMC_ExitIsolatedState(void) {
    PMC->REGSC |= PMC_REGSC_ACKISO_MASK;
}

/* END FILE */
