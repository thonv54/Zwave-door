/******************************************************************************
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
 * Last Changed By:  $Author: ThangDH $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
#include "configapp.h"
#include "cmdcommon.h"
#include "debug.h"
#include "timer.h"
#include "eventman.h"
#include "battmnge.h"
#include "pwrmanager.h"
#include "powerapi.h"
#include "zwstatus.h"
#include "sysregval.h"
#include "keyconfig.h"
#include "led.h"
#include "rtc.h"
#include "rcm.h"
#include "zwnetstatus.h"
#include "sensdoor.h"

#include "cmddoor.h"
#include "cmdkeyconfig.h"
#include "cmdnetw.h"
#include "cmdtxstatus.h"
#include "cmdtimewakup.h"
#include "cmdtimesensrp.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef MAIN_DEBUG
#define DBG_MAIN_SEND_STR(x)         Debug_SendStr(x)
#define DBG_MAIN_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_MAIN_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_MAIN_SEND_STR(x)
#define DBG_MAIN_SEND_NUM(x)
#define DBG_MAIN_SEND_HEX(x)
#endif

#define IS_WAKEUP(x)                (byWakeupReason == x)

typedef enum _EVENT_APP_ {
    EVENT_EMPTY,
    EVENT_APP_KEY_WAKEUP,
    EVENT_APP_MCU_GOSLEEP,

    EVENT_APP_STARTUP_CHECK,
    EVENT_APP_REQUEST_NETW,
    EVENT_APP_WAKUP_NOTIFY,
    EVENT_APP_SENS_REPORT,
    EVENT_APP_REPORT_BATT,
} EVENT_APP;

typedef enum _STATE_APP_ {
    STATE_APP_STARTUP,
    STATE_APP_IDLE,
    STATE_APP_KEY_CONFIG,
    STATE_APP_TRANSMIT,
    STATE_APP_MCUSLEEP,
} STATE_APP;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BYTE byWakeupReason;
static BYTE currentState = STATE_APP_STARTUP;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void ChangeState(BYTE st);
static void AppStateManager(BYTE ev);
static void MCB_RequestNetwDone(BYTE result);
static void MCB_KeyConfigReportDone(BYTE result);
BOOL
DoorSensorSendReport(void);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   AppInit 
 * @brief  None
 * @param  None
 * @retval None
 */
void
AppInit(void) {
    SystemCoreClockUpdate();
    TimerInit();
    WakeupReason(&byWakeupReason);

    #ifdef USE_DEBUG
    InitDebug();
    #endif

    DBG_MAIN_SEND_STR("\n\n$><$  ^^ ");  
    DBG_MAIN_SEND_HEX(byWakeupReason);
    DBG_MAIN_SEND_STR(" ");
    DBG_MAIN_SEND_NUM(RTC_GetTime());
    DBG_MAIN_SEND_STR("\n");

    if (IS_WAKEUP(WakeupFistTime)) {
        DBG_MAIN_SEND_STR("$ FIRST TIME!\n");
        LedInit();
        LedOnAll();
        delay_ms(100);
        LedOffAll();
        delay_ms(100);
        LedOnAll();
        delay_ms(100);
        LedOffAll();
        LedDeInit(); 
        delay_ms(2000);
    }

    InitBattMonitor();
    InitSerialProc(SetPowerDownTimeout);
    InitSerial();
    InitPinZwState();
    InitDoorSens(); /**/
    EventSchedulerInit(AppStateManager);

    if (IS_WAKEUP(WakeupFistTime)) {    
        BattSetDefault();
        ConfigurationSetDefault();
        if (!ActivateZwave(TIMEOUT_ZW_GOACTIVE)) {
            MCU_RESET;
            while(1) {}
        }
    }

    if (IS_WAKEUP(WakeupSwCfg)) {
        InitKeyConfig(AppStateManager, SetPowerDownTimeout);
        LedInit();
        DBG_MAIN_SEND_STR("$ KEY INIT\n");
        ChangeState(STATE_APP_KEY_CONFIG);
        EventSchedulerAdd(EVENT_APP_KEY_WAKEUP); 
        SetPowerDownTimeout(PWTIMEOUT_KEY_NOPRESS);
        return;      
    }

    if (IS_WAKEUP(WakeupSensDoor)) {
        if ((RTC_GetTime() - SysPara->lastWakeupTime) < 1000) {
            WuSetup cfg;

            cfg.TimerInterval = TIME_S;
            SetPeriphWakup(MODUL_LPTMR, ENABLE, cfg);
            DBG_MAIN_SEND_STR("$ DOOR sens wu NOT STABLE ");
            if (GetCountCheckDoorStatus() == 0) {
                SetCountCheckDoorStatus(TIME_TRIES_CHECK_DOOR_STABLE);
                DoorSensRead(); 
                DBG_MAIN_SEND_STR("report\n");
            }
            else {
                DBG_MAIN_SEND_STR("sleep\n");
                SetCountCheckDoorStatus(TIME_TRIES_CHECK_DOOR_STABLE);
                PowerDownNow(); /* Sleep */
            } 
        }
        else {
            DBG_MAIN_SEND_STR("DOOR sens wu\n");
            SetCountCheckDoorStatus(0);
            DoorSensRead(); 
        }

        LedInit();
        LedShow(LEDDOOR_EVENT);
        LedDeInit();
    }

    if (IS_WAKEUP(WakeupSensDoorCk)) {
        WuSetup cfg;

        cfg.TimerInterval = TIME_S;
        SetPeriphWakup(MODUL_LPTMR, ENABLE, cfg); 
        SetCountCheckDoorStatus(GetCountCheckDoorStatus() - 1);
        if (GetCountCheckDoorStatus() == 0) {
            DBG_MAIN_SEND_STR("DOOR sens check (STABLE)\n");
            DoorSensRead(); 
        }
        else {
            DBG_MAIN_SEND_STR("DOOR sens check (NOT STABLE)\n");
            PowerDownNow();
        }

        LedInit();
        LedShow(LEDDOOR_EVENT);
        LedDeInit();
    }

    if (byWakeupReason >= RESET_REASON_NBR) {
        DBG_MAIN_SEND_STR("Fu~!@#$%^&*k");
        MCU_RESET;
        while(1) {}
    }

    if (IS_WAKEUP(WakeupByZwave)) {
        /* Awake 200ms wait cmd from ZWave */
        SetPowerDownTimeout(PWTIMEOUT_200_MSEC);
        DBG_MAIN_SEND_STR("$ WakeupByZWave\n");
        ChangeState(STATE_APP_IDLE);
    }
    else {
        if (GetZwNetwStatus() == ZW_NETW_REQ) { 
            EventSchedulerAdd(EVENT_APP_REQUEST_NETW);
            SetPowerDownTimeout(PWTIMEOUT_500_MSEC);
        }

        if (IS_WAKEUP(WakeupNotification)) {
            EventSchedulerAdd(EVENT_APP_WAKUP_NOTIFY);
        }

        if (DoorGetFlagReport()) {
            EventSchedulerAdd(EVENT_APP_SENS_REPORT);
        }

        ChangeState(STATE_APP_TRANSMIT);
    }
}

/**
 * @func   main
 * @brief  None
 * @param  None
 * @retval None
 */
int 
main() {  
    AppInit(); 

    while(1) {
        processTimer();
        procSerial(); 
        EventScheduler();
    }
}

/**
 * @func   ChangeState
 * @brief  None
 * @param  None
 * @retval None
 */
void
ChangeState(
    BYTE st
) {
    currentState = st;
}

/**
 * @func   AppStateManager
 * @brief  None
 * @param  EVENT_APP ev
 * @retval None
 */
void
AppStateManager( 
    BYTE ev
) {
    switch (currentState) {
    case STATE_APP_STARTUP: 
        break;

    case STATE_APP_IDLE:
        break;

    case STATE_APP_TRANSMIT:
        if (EVENT_APP_REQUEST_NETW == ev) {
             BOOL sendFinish = CmdZwNetwGetStatus(MCB_RequestNetwDone);
     
             if (sendFinish == FALSE) {
                 MCB_RequestNetwDone(JOB_FALSE);
             }
        }

        if (EVENT_APP_WAKUP_NOTIFY == ev) {
            CmdKeyConfigSendReport(BUTT_PRESS, NULL);
            DBG_MAIN_SEND_STR("$ Wakeup notifiacation\n");
        }

        if (EVENT_APP_SENS_REPORT == ev) {
            DoorSensorSendReport();
        }     
        break;
  
    case STATE_APP_KEY_CONFIG:
        if ((EVENT_APP_KEY_WAKEUP == ev) || (ev == EVENT_KEY_1PRESS)) {
            LedShow(LEDKEY_PRESS);
        }
        
        if (EVENT_KEY_1PRESS == ev) {
            CmdKeyConfigSendReport(BUTT_PRESS, NULL);
        }

        if (ev == EVENT_KEY_HOLD2S) {
            LedShow(LEDKEY_HOLD_2S);
        }

        if (ev == EVENT_KEY_HOLD5S) {
            LedShow(LEDKEY_HOLD_5S);
        }

        if (ev == EVENT_KEY_RHOLD2S) {
            CmdKeyConfigSendReport(BUTT_HOLD_05SEC, MCB_KeyConfigReportDone);
            LedShow(LEDKEY_RELEASE);
        }

        if (ev == EVENT_KEY_RHOLD5S) {
            CmdKeyConfigSendReport(BUTT_HOLD_10SEC, MCB_KeyConfigReportDone);
            LedShow(LEDKEY_RELEASE);
        }
        break;

    case STATE_APP_MCUSLEEP:
        if (ev == EVENT_APP_MCU_GOSLEEP) { 
            if (GetCountCheckDoorStatus() == 0) {
                DoorSensRead();
            }

            if (DoorGetFlagReport()) {
                ChangeState(STATE_APP_TRANSMIT);
                EventSchedulerAdd(EVENT_APP_SENS_REPORT);
            }
            else {
                PowerDownNow();
            }
        }
        break;

    default: 
        break;
    }   
}

/**
 * @func   MCB_DoorSensReportDone  
 * @brief  None
 * @param  None
 * @retval None
 */
void
MCB_DoorSensReportDone(
    BYTE result
) {
    if (result == JOB_SUCCESS) {
        DoorSetFlagReport(DISABLE); 
    }
    else if (JOB_FALSE == result) {
        SetPowerDownTimeout(PWTIMEOUT_500_MSEC);
        LedInit();
        LedShow(LEDZW_COMMFALSE);
        LedDeInit();
    }

//    ChangeState(STATE_APP_MCUSLEEP);
//    EventSchedulerAdd(EVENT_APP_MCU_GOSLEEP);
}

/**
 * @func   SensorSendReport  
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
DoorSensorSendReport() {
    WORD param;

    if (DoorGetState() == DOOR_OPEN) {
        param = PDOOR_OPEN; 
    }
    else if (SysPara->doorState == DOOR_CLOSE) {
        param = PDOOR_CLOSE; 
    }

    return DoorSendReport(param, MCB_DoorSensReportDone);
}

/**
 * @func   MCB_BattReportSendDone  
 * @brief  None
 * @param  None
 * @retval None
 */
void
MCB_BattReportSendDone(
    BYTE result
) {
    if (result == JOB_SUCCESS) {
        SetFlagBatLevlUpd(FALSE);
        DBG_MAIN_SEND_STR("Batt report done\n");
    }

    ChangeState(STATE_APP_TRANSMIT);
    EventSchedulerAdd(EVENT_APP_SENS_REPORT);
}

/**
 * @func   MCB_RequestNetwDone  
 * @brief  None
 * @param  None
 * @retval None
 */
void
MCB_RequestNetwDone(
    BYTE result
) {
    if (result == JOB_SUCCESS) {
        /**/
        delay_ms(50);
        DBG_MAIN_SEND_STR("Netw request done\n");

        ChangeState(STATE_APP_TRANSMIT);
        EventSchedulerAdd(EVENT_APP_SENS_REPORT);
    }
    else {
        DBG_MAIN_SEND_STR("Netw request false\n");
        LedInit();
        LedShow(LEDZW_COMMFALSE);
        LedDeInit();
        ChangeState(STATE_APP_MCUSLEEP);
        EventSchedulerAdd(EVENT_APP_MCU_GOSLEEP);
    }    
}

void 
MCB_KeyConfigReportDone(
    BYTE result
) {
    if (result == JOB_SUCCESS) { 
    
    }
    else {

    } 

    WuSetup cfg;
    cfg.TimerInterval = 5000;
    SetPeriphWakup(MODUL_LPTMR, ENABLE, cfg);
    ConfigurationSetDefault();
    LedOffAll();
    LedDeInit();
    PowerDownNow(); 
}

void
HandleCmd(
    BYTE option,
    CMD_BUFFER_P pCmd,
    BYTE length
) {
    switch(pCmd->scommon.cmdid) {
    case CMD_ZW_NETWORK:
        HandleCmdZwNetw(option, pCmd, length);
        break;

    case CMD_ZW_TXFALSE:
        HandleCmdTxStatus(option, pCmd, length);
        break;

    case CMD_TIME_WAKEUP:
        HandleCmdTimeWakup(option, pCmd, length);
        break;

    case CMD_TIME_SENS:
        HandleCmdTimeSensRp(option, pCmd, length);
        break;
    }
}

void
TxFailShow(void) {
    LedInit();
    LedShow(LEDZW_TXFALSE);
    LedDeInit();
}

/* END FILE */
