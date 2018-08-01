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
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "rcm.h"
#include "driver_common.h"
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
 * @func  
 * @brief  None  
 * @param  None
 * @retval None
 */
BYTE
RST_Reason(void){                  
    if (RCM->SRS1 & RCM_SRS1_SACKERR_MASK)
        return RST_SACKERR;

    if (RCM->SRS1 & RCM_SRS1_MDM_AP_MASK)
        return RST_MDM_AP;

    if (RCM->SRS1 & RCM_SRS1_SW_MASK)
        return RST_SW;

    if (RCM->SRS1 & RCM_SRS1_LOCKUP_MASK)
        return RST_LOCKUP;
	
    if (RCM->SRS0 & RCM_SRS0_POR_MASK)
        return RST_POR; 

    if (RCM->SRS0 & RCM_SRS0_PIN_MASK)
        return RST_PINRST;

    if (RCM->SRS0 & RCM_SRS0_WDOG_MASK)
        return RST_WDOG;

    if (RCM->SRS0 & RCM_SRS0_LOC_MASK)
        return RST_LOC;

    if (RCM->SRS0 & RCM_SRS0_LOL_MASK)
        return RST_LOL;

    if (RCM->SRS0 & RCM_SRS0_LVD_MASK)
        return RST_LVD;

    if (RCM->SRS0 & RCM_SRS0_WAKEUP_MASK) {
        if ((SMC->PMCTRL & SMC_PMCTRL_STOPM_MASK)== 3) {}
        if (((SMC->PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) && ((SMC->STOPCTRL & SMC_STOPCTRL_VLLSM_MASK)== 0)) {}
        if (((SMC->PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) && ((SMC->STOPCTRL & SMC_STOPCTRL_VLLSM_MASK)== 1)) {}
        if (((SMC->PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) && ((SMC->STOPCTRL & SMC_STOPCTRL_VLLSM_MASK)== 2)) {}
        if (((SMC->PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) && ((SMC->STOPCTRL & SMC_STOPCTRL_VLLSM_MASK)== 3)) {}
        
        return RST_WAKEUP;
    }

    return MAX_U8;
}
/* END FILE */
