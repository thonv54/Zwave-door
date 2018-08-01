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
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "battmnge.h"
#include "pwrmanager.h" 
#include "powerapi.h"
#include "sysregval.h"
#include "zwnetstatus.h"
#include "sensdoor.h"
#include "debug.h"

#include "rtc.h"
#include "rcm.h"
#include "pmc.h"
#include "mcg.h"
#include "sim.h"
#include "pit.h"
#include "uart.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef PWMANAGE_DEBUG
#define DBG_PWMANAGE_SEND_STR(x)         Debug_SendStr(x)
#define DBG_PWMANAGE_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_PWMANAGE_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_PWMANAGE_SEND_STR(x)
#define DBG_PWMANAGE_SEND_NUM(x)
#define DBG_PWMANAGE_SEND_HEX(x)
#endif

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static WORD powerDownTicks;
DWORD gDwTimeWakeup;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
extern void PowerDownNow(void);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   WakeupReason
 * @brief  None
 * @param  None
 * @retval None
 */
void 
WakeupReason(
    BYTE_p bWakeupReason 
) {  
    WuSetup cfg;
    BYTE rstReason = RST_Reason();

    *bWakeupReason = rstReason;
    StartPowerDownTimeout();
    LoadConfigParam();
    if ((rstReason == RST_POR) | (rstReason == RST_SW)) {
        SetPowerDownTimeout(40);
        CLOCK_SelectOSC32Clock(OSC32K_LPO_1KHZ); 
        RTC_Init();    

        SysPara->bTimerType = TIMER_LONG;
        cfg.TimerInterval = 10000;
        SetPeriphWakup(MODUL_LPTMR, ENABLE, cfg);
        *bWakeupReason = WakeupFistTime;

        return;
    }

    CLOCK_EnableRtcClock();

    if ((rstReason == RST_WAKEUP) && PMC_IsIsolatedState()) {
        PMC_ExitIsolatedState();
        SetPowerDownTimeout(PWTIMEOUT_100_MSEC);

        if (IsWakeupByPeriph(PIN_CONFIG)) {
            *bWakeupReason = WakeupSwCfg; 
            SetPowerDownTimeout(PWTIMEOUT_KEY_NOPRESS);
            return;                     
        } 

        if (IsWakeupByPeriph(DOOR_SENS)) {  
            *bWakeupReason = WakeupSensDoor;
            return;
        }

        if (IsWakeupByPeriph(WAKEUP_MKL)) {
            *bWakeupReason = WakeupByZwave;
            return;
        }

        if (IsWakeupByPeriph(MODUL_LPTMR)) {   
            if (GetZwNetwStatus() == ZW_NETW_NOTNW) {
                SysPara->bTimerType = TIMER_LONG;
                cfg.TimerInterval = TIME_L;
                SetPeriphWakup(MODUL_LPTMR, DISABLE, cfg);
/**/            PowerDownNow();
            }

            if (GetCountCheckDoorStatus() != 0) {
                *bWakeupReason = WakeupSensDoorCk;
                return;
            }

            if (SysPara->bTimerType == TIMER_SHORT) {
                cfg.TimerInterval = TIME_L;
                SetPeriphWakup(MODUL_LPTMR, ENABLE, cfg);
                SysPara->bTimerType = TIMER_LONG;
                return;
            }
            else if (SysPara->bTimerType == TIMER_LONG) {
                cfg.TimerInterval = TIME_L;
                SetPeriphWakup(MODUL_LPTMR, ENABLE, cfg);

                DecreCount(&SysPara->countRpDoor);
                DecreCount(&SysPara->countWakeup);

                if (SysPara->countRpDoor == 0) {
                    /* Wakeup report door */
                    *bWakeupReason = WakeupSensDoor;
                    return;
                }

                if (SysPara->countWakeup == 0) {
                    /* Wakeup notifiacation */
                    *bWakeupReason = WakeupNotification;
                    return;
                }

                PowerDownNow();
            }
        }

        return;
    }

    {
//        MCU_RESET;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval 
 */
DWORD
GetTimeSys(void) {
    return  RTC_GetTime();
}

/**
 * @func   PowerDownTimeoutFunc
 * @brief  None
 * @param  None
 * @retval None
 */
void
PowerDownTimeoutFunc(
    void_p pVoid
) {
    if (!powerDownTicks) {
        DBG_PWMANAGE_SEND_STR("PWR tmout!\n"); 
        PowerDownNow(); 
    }
    else {
        powerDownTicks--;
        DBG_PWMANAGE_SEND_STR("PWD");
        DBG_PWMANAGE_SEND_NUM(powerDownTicks);
        DBG_PWMANAGE_SEND_STR("\n");
    }
}

/**
 * @func   StartPowerDownTimeout
 * @brief  None
 * @param  None
 * @retval None
 */
void
StartPowerDownTimeout(void) {
    TaskStart(PWDOWN_TIMER_INTERVAL, TASK_FOREVER, PowerDownTimeoutFunc, NULL);
}

/**
 * @func   SetPowerDownTimeout
 * @brief  None
 * @param  None
 * @retval None
 */
void
SetPowerDownTimeout(
    WORD timeout
) {
    if (powerDownTicks < timeout) {
        powerDownTicks = timeout;   
    }
}

/**
 * @func   PowerDownNow
 * @brief  None
 * @param  None
 * @retval None
 */
void
PowerDownNow(void) {
    WuSetup cfg;

    SysPara->lastWakeupTime = RTC_GetTime();
    StoreConfigParam();

    cfg.IOTrigger = WUFallingEdge;
    SetPeriphWakup(PIN_CONFIG, ENABLE, cfg);

    cfg.IOTrigger = WURisingFalingEdge;
    SetPeriphWakup(DOOR_SENS, ENABLE, cfg);

    cfg.IOTrigger = WUFallingEdge;
    SetPeriphWakup(WAKEUP_MKL, ENABLE, cfg);

    DBG_PWMANAGE_SEND_STR("$><$  vv\n"); 

    /* MUST Delay IF NOT CORE LOOKUP RESET */
    delay_ms(4);
    UART_DeInit(UART_ZW);

    #ifdef USE_DEBUG
    UART_DeInit(UART_DEBUG);
    #endif

    SetPowerMode(POWER_SLEEP);
}

/* END FILE */
