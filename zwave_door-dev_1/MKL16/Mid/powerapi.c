/*******************************************************************************
 *
 * Copyright (c) 2017
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Setup peripheral after sleep
 *              Check peripheral wakeup mcu
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
#include "configapp.h"
#include "driver_common.h"
#include "powerapi.h"
#include "gpio.h"
#include "lptmr.h"
#include "llwu.h"
#include "sim.h"
#include "mcg.h"
#include "smc.h"
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
 * @func   SetPeriphWakup  
 * @brief  None
 * @param  None
 * @retval None
 */
void 
SetPeriphWakup(
    DWORD Peripheral,
    FunctionalState NewState,
    WuSetup SetupData    
) {
    WORD pin, port;
    
    pin = EXTRACT_PIN(Peripheral);
    port = EXTRACT_PORT(Peripheral);

    if (pin == NO_PIN) {
        if (port == WU_LPTMR) {
            if (NewState == ENABLE) {
                LPTMR_Init(SetupData.TimerInterval, LPTMR_USE_LPOCLK);
                LPTMR_SetIrq(ENABLE);
                LLWU_SetModulWakeup(WU_LPTMR, ENABLE);
            }
            else {
                LLWU_SetModulWakeup(WU_LPTMR, DISABLE);
            } 
        }
        else if (port == WU_RTCA) {
            if (NewState == ENABLE) {
                LLWU_SetModulWakeup(WU_RTCA, ENABLE);  
            }
            else {
                LLWU_SetModulWakeup(WU_RTCA, DISABLE);       
            }
        }        
    }
    else if (port != NO_PORT) {
        LLWU_SetPinWakeup(Peripheral, SetupData.IOTrigger);
        if (NewState == ENABLE) {
            LLWU_ClearPinITPending(Peripheral);
            GPIO_SetPinIsInput(Peripheral, PIN_FLOATING);
        }
    } 
}

/**
 * @func   IsWakeupByPeriph
 * @brief  Check wakeup by peripheral and clear flag wakeup
 * @param  None
 * @retval TRUE if wakeup by peripheral
 */
BOOL 
IsWakeupByPeriph(
    DWORD Peripheral
) {
    DWORD pin, port;
    
    pin = EXTRACT_PIN(Peripheral);
    port = EXTRACT_PORT(Peripheral);

    if (pin == NO_PIN) {
        if (port == WU_LPTMR) {
            CLOCK_EnableLptmrClock();
            if (LLWU_GetModulITStatus(WU_LPTMR) == SET) 
            {
                LPTMR_ClearITPendingBit();
                return TRUE;
            }
        }
        else if (port == WU_RTCA) {
           CLOCK_EnableRtcClock();
        }
    }
    else if (port != NO_PORT) {
        if (LLWU_GetPinITStatus(Peripheral) == SET)
        {
            LLWU_ClearPinITPending(Peripheral);
            return TRUE;                     
        }
    }

    return FALSE;
}

/**
 * @func   SetPowerMode
 * @brief  None
 * @param  None
 * @retval None
 */
void
SetPowerMode(
    powerMode mode
) {
    switch ((BYTE)mode) {
    case POWER_NOMAL:
        break;

    case POWER_LOWEN:
        CLOCK_SetFastIrcDivClock(0);
        MCG_FEI2FBI(CPU_FASTIRC_CLK, FAST_IRC);
        MCG_FBI2BLPI(CPU_FASTIRC_CLK, FAST_IRC);
        CLOCK_SetOutDiv(0, 4);
        SystemCoreClockUpdate();
        SysTick_Config(SystemCoreClock/1000);
        SMC_EnterModeVLPR();
        break;

    case POWER_SLEEP:
        SMC_EnterModeVLLS1(); /* */
        break;
    }
}

/* END FILE */
