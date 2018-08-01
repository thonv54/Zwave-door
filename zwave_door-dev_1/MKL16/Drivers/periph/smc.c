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
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "smc.h"
#include "debug.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef SMC_DEBUG
#define DBG_SMC_SEND_STR(x)         Debug_SendStr(x)
#define DBG_SMC_SEND_NUM(x)         Debug_SendNum(x)
#else
#define DBG_SMC_SEND_STR(x)
#define DBG_SMC_SEND_NUM(x)
#endif

#define SMC_PMPROT                  ((SMC)->PMPROT)
#define SMC_PMCTRL                  ((SMC)->PMCTRL)
#define SMC_PMCTRL                  ((SMC)->PMCTRL)
#define SMC_STOPCTRL                ((SMC)->STOPCTRL)
#define SMC_PMSTAT                  ((SMC)->PMSTAT) 
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
 * @func   stop
 * @brief  
 * @param  None
 * @retval None
 */
void stop (void) {
    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB_SCR |= SCB_SCR_SLEEPDEEP_Msk;	
    /* WFI instruction will start entry into STOP mode */
    __wfi();
}

/**
 * @func   wait
 * @brief   
 * @param  None
 * @retval None
 */
void wait (void) {
    /* Clear the SLEEPDEEP bit to make sure we go into WAIT (sleep) mode instead
     * of deep sleep.
     */
    SCB_SCR &= ~SCB_SCR_SLEEPDEEP_Msk;	
    /* WFI instruction will start entry into WAIT mode */
    __wfi();
}

/**
 * @func   SMC_EnterModeVLPR
 * @brief  Puts the processor into very low power run mode.
 *         In this mode all clocks are enabled, but the core clock limited. 
 *         The flash clock is limited to 1MHz or less.  
 * @param  None
 * @retval PMSTAT value or error code
 *                       000_0001 Current power mode is RUN
 *                       000_0100 Current power mode is VLPR
 *         ERROR Code =  0x14 - already in VLPR mode
 *                    =  0x24 - REGONS never clear indicating stop regulation
 */
BYTE
SMC_EnterModeVLPR(void) {
    DWORD i; 
    BYTE return_value;

    if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4) 
    {
         return_value = 0x14;
    }

    SMC_PMPROT = SMC_PMPROT_AVLP_MASK | SMC_PMPROT_AVLLS_MASK | SMC_PMPROT_AVLP_MASK;  

    SMC_PMCTRL &= ~SMC_PMCTRL_RUNM_MASK; 
    SMC_PMCTRL  |= SMC_PMCTRL_RUNM(0x2);
    /* Wait for VLPS regulator mode to be confirmed */
    for (i = 0 ; i < 10000 ; i++) {  
        if ((PMC_REGSC & PMC_REGSC_REGONS_MASK) == PMC_REGSC_REGONS_MASK){
            /* 0 Regulator is in stop regulation or in transition 
            to/from it 1 MCU is in Run regulation mode */
        }  
        else  
            break;
    }  

    if ((PMC_REGSC & PMC_REGSC_REGONS_MASK) == PMC_REGSC_REGONS_MASK) 
    {  
        return_value = 0x24; 
    }

    /* SMC_PMSTAT register only exist in Mode Controller 2 MCU versions */
    if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK) == 4)  
    {
        return_value = SMC_PMSTAT;
    }

    return (return_value);
}

/**
 * @func   SMC_ExitModeVLPR
 * @brief  Puts the processor into normal run mode from VLPR mode. 
 *         You can transition from VLPR to normal run using this function.
 * @param  None
 * @retval None
 */
void 
SMC_ExitModeVLPR(void) {
    int i;
    /* Check to make sure in VLPR before exiting */
    if  ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4) {      
        /* Clear RUNM */
        SMC_PMCTRL &= ~SMC_PMCTRL_RUNM_MASK;                
        /* Wait for normal RUN regulation mode to be confirmed                    
           PMSTAT = 1, MCU is in RUN  mode
           PMSTAT = 4, MCU is in VLPR mode */
        for (i = 0; i < 0xff; i++)
        {
            if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 1)
                if ((PMC_REGSC & PMC_REGSC_REGONS_MASK) == 1)
    	               break;
        }           
    }     
}

/**
 * @func  
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
SMC_GetMode(void) {
    return (SMC->PMSTAT);
}

/**
 * @func  
 * @brief  None
 * @param  None
 * @retval None
 */
void 
SMC_ShowMode(
    BYTE mode
) {
    switch(mode) {
    case PW_MODE_RUN:
        DBG_SMC_SEND_STR("pw run\n"); 
        break;

    case PW_MODE_STOP:
        DBG_SMC_SEND_STR("pw stop\n");        
        break;

    case PW_MODE_VLPR:
        DBG_SMC_SEND_STR("pw vlpr\n"); 
        break;

    case PW_MODE_VLPW:
        DBG_SMC_SEND_STR("pw vlpw\n"); 
        break;

    case PW_MODE_VLPS:
        DBG_SMC_SEND_STR("pw vlps\n"); 
        break;

    case PW_MODE_VLLS:
        DBG_SMC_SEND_STR("pw vlls\n"); 
        break;

    case PW_MODE_LLS:
        DBG_SMC_SEND_STR("pw lls\n"); 
        break;

    default:
        break; 
    }
}

/**
 * @func
 * @brief  Puts the processor into VLLS1 mode from normal run mode or VLPR.
 *         VLLSx modes will always exit to RUN mode even if you were 
 *         in VLPR mode before entering VLLSx.
 *         Setup the desired wakeup sources in the LLWU before calling this function.
 * @param  None
 * @retval
 */
void
SMC_EnterModeVLLS1(void) {
    volatile DWORD dummyread;

    /* Write to PMPROT to allow all possible power modes */
    SMC_PMPROT = SMC_PMPROT_AVLP_MASK | SMC_PMPROT_AVLLS_MASK | SMC_PMPROT_AVLP_MASK; 
    
    /* Set the STOPM field to 0b100 for VLLS1 mode */
    SMC_PMCTRL &= ~SMC_PMCTRL_STOPM_MASK; 
    SMC_PMCTRL |=  SMC_PMCTRL_STOPM(0x4); 
    /* set VLLSM = 0b01 */
    SMC_STOPCTRL =  SMC_STOPCTRL_VLLSM(1);          
    /*wait for write to complete to SMC before stopping core */  
    dummyread = SMC_STOPCTRL;
    /* Now execute the stop instruction to go into VLLS1 */
    stop();
}

/* END FILE */
