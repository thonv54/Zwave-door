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
/*                               INCLUDE FILES                                */
/******************************************************************************/
#include <ZW_stdint.h>
#include "config_app.h"

#include <slave_learn.h>
#include <ZW_slave_api.h>
#ifdef ZW_SLAVE_32
#include <ZW_slave_32_api.h>
#else
#include <ZW_slave_routing_api.h>
#endif  /* ZW_SLAVE_32 */
#include <ZW_classcmd.h>
#include <ZW_mem_api.h>
#include <ZW_TransportLayer.h>
#include <ZW_task.h>
#include <ev_man.h>
#include <eeprom.h>
#include <ZW_uart_api.h>

#include <misc.h>

#ifdef ZW_ISD51_DEBUG
#include "ISD51.h"
#endif
#include <association_plus.h>
#include <agi.h>
#include <CommandClassAssociation.h>
#include <CommandClassAssociationGroupInfo.h>
#include <CommandClassVersion.h>
#include <CommandClassZWavePlusInfo.h>
#include <CommandClassPowerLevel.h>
#include <CommandClassDeviceResetLocally.h>
#include <CommandClassSupervision.h>
#include <CommandClassSecurity.h>
#include <CommandClassBattery.h>
#include <CommandClassNotification.h>
#include <notification.h>

#include <ZW_conbufio.h>
#include <ZW_power_api.h>
#include <battery_plus.h>
#include <battery_monitor.h>
#include <zaf_version.h>
#include <app_version.h>
#include <gpio_driver.h>

#include "serial.h"
#include "zwmcu_cmd.h"
#include "sensor.h"
#include "zwsensconvert.h"
#include "msgcount.h"
#include "zwst.h"
/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/
#ifdef USEDEBUG
#define SENS_DBG(x)                          (x)
#else /* USEDEBUG */
#define SENS_DBG(x)
#endif /* USEDEBUG */

#ifdef ZW_DEBUG_SENS
#define ZW_DEBUG_SENS_SEND_BYTE(data)         ZW_DEBUG_SEND_BYTE(data)
#define ZW_DEBUG_SENS_SEND_STR(STR)           ZW_DEBUG_SEND_STR(STR)
#define ZW_DEBUG_SENS_SEND_NUM(data)          ZW_DEBUG_SEND_NUM(data)
#define ZW_DEBUG_SENS_SEND_WORD_NUM(data)     ZW_DEBUG_SEND_WORD_NUM(data)
#define ZW_DEBUG_SENS_SEND_NL()               ZW_DEBUG_SEND_NL()
#else /* ZW_DEBUG_SENSOR */
#define ZW_DEBUG_SENS_SEND_BYTE(data)
#define ZW_DEBUG_SENS_SEND_STR(STR)
#define ZW_DEBUG_SENS_SEND_NUM(data)
#define ZW_DEBUG_SENS_SEND_WORD_NUM(data)
#define ZW_DEBUG_SENS_SEND_NL()
#endif /* ZW_DEBUG_SENSOR */

typedef enum _EVENT_APP_
{
    EVENT_EMPTY = DEFINE_EVENT_APP_NBR,
    EVENT_APP_LEARN_MODE_FINISH,
    EVENT_APP_NEXT_EVENT_JOB,
    EVENT_APP_FINISH_EVENT_JOB,
    EVENT_REPORT_BATTLEVL,
    EVENT_WAKUP_NOTIFY,
    EVENT_DOOR_REPORT_JOB
} EVENT_APP;

typedef enum _STATE_APP_
{
    STATE_APP_STARTUP,
    STATE_APP_IDLE,
    STATE_APP_LEARN_MODE,
    STATE_APP_WATCHDOG_RESET,
    STATE_APP_POWERDOWN,
    STATE_APP_TRANSMIT_DATA
} STATE_APP;

#define DEFAULT_SLEEP_TIME               0        
#define MIN_SLEEP_TIME                   0       
#define MAX_SLEEP_TIME                   64800    
#define STEP_SLEEP_TIME                  3600  
/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/

/**
 * Unsecure node information list.
 * Be sure Command classes are not duplicated in both lists.
 * CHANGE THIS - Add all supported non-secure command classes here
 **/
static code BYTE cmdClassListNonSecureNotIncluded[] = {
    COMMAND_CLASS_ZWAVEPLUS_INFO,
    COMMAND_CLASS_ASSOCIATION_V2,
    COMMAND_CLASS_ASSOCIATION_GRP_INFO,
    COMMAND_CLASS_TRANSPORT_SERVICE_V2,
    COMMAND_CLASS_VERSION_V3,
    COMMAND_CLASS_MANUFACTURER_SPECIFIC_V2,
    COMMAND_CLASS_DEVICE_RESET_LOCALLY,
    COMMAND_CLASS_POWERLEVEL,
    COMMAND_CLASS_BATTERY,
    COMMAND_CLASS_WAKE_UP_V2,
    COMMAND_CLASS_NOTIFICATION_V8,
    COMMAND_CLASS_SUPERVISION,
    COMMAND_CLASS_SECURITY_2
};

/**
 * Unsecure node information list Secure included.
 * Be sure Command classes are not duplicated in both lists.
 * CHANGE THIS - Add all supported non-secure command classes here
 **/
static code BYTE cmdClassListNonSecureIncludedSecure[] = {
    COMMAND_CLASS_ZWAVEPLUS_INFO_V2,
    COMMAND_CLASS_TRANSPORT_SERVICE_V2,
    COMMAND_CLASS_SUPERVISION,
    COMMAND_CLASS_SECURITY_2
};

/**
 * Secure node inforamtion list.
 * Be sure Command classes are not duplicated in both lists.
 * CHANGE THIS - Add all supported secure command classes here
 **/
static code BYTE cmdClassListSecure[] = {
    COMMAND_CLASS_VERSION,
    COMMAND_CLASS_ASSOCIATION_V2,
    COMMAND_CLASS_ASSOCIATION_GRP_INFO,
    COMMAND_CLASS_MANUFACTURER_SPECIFIC,
    COMMAND_CLASS_DEVICE_RESET_LOCALLY,
    COMMAND_CLASS_POWERLEVEL,
    COMMAND_CLASS_BATTERY,
    COMMAND_CLASS_WAKE_UP,
    COMMAND_CLASS_NOTIFICATION_V8
};

/**
 * Structure includes application node information list's and device type.
 */
APP_NODE_INFORMATION m_AppNIF = {
    cmdClassListNonSecureNotIncluded, sizeof(cmdClassListNonSecureNotIncluded),
    cmdClassListNonSecureIncludedSecure, sizeof(cmdClassListNonSecureIncludedSecure),
    cmdClassListSecure, sizeof(cmdClassListSecure),
    DEVICE_OPTIONS_MASK, GENERIC_TYPE, SPECIFIC_TYPE
};

const char GroupName[] = "Lifeline";
static CMD_CLASS_GRP agiTableLifeLine[] = {AGITABLE_LIFELINE_GROUP};
static AGI_PROFILE gProfile;

static BOOL gbLearnMode = FALSE;
static BYTE myNodeID = 0;
static STATE_APP currentState = STATE_APP_IDLE;

static SW_WAKEUP wakeupReason;
static BOOL IsSentWakeupNotify = FALSE;
static BOOL IsSentBattRepport = FALSE;
static BYTE gAwakeTimeCount = 0;

static BOOL repMcuSleep = FALSE;

static BYTE suppportedEventsDOOR[] = NOTIFICATION_DOOR;
static BYTE gNotificationEvent;
static BYTE gNotificationType;
static BYTE gNotificationParam;
static DWORD cntTxFalse;

#if defined(APP_SUPPORTS_CLIENT_SIDE_AUTHENTICATION)
static s_SecurityS2InclusionCSAPublicDSK_t sCsaResponse = { 0, 0, 0, 0};
#endif /* defined(APP_SUPPORTS_CLIENT_SIDE_AUTHENTICATION) */

/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/

/****************************************************************************/
/*                            PRIVATE FUNCTIONS                             */
/****************************************************************************/
static void SetDefaultConfiguration(void);
static void LoadConfiguration(void);
static void LoadNotificationConfig(void);
static void SensorDOORreport(void);

void ZCB_DeviceResetLocallyDone(TRANSMISSION_RESULT * pTransmissionResult);
void ZCB_BattReportSentDone(TRANSMISSION_RESULT * pTransmissionResult);
void ZCB_SensorNotificationCallback(TRANSMISSION_RESULT * pTransmissionResult);

void ChangeState(STATE_APP st);
void AppStateManager(EVENT_APP ev);

/**
 * @func   ApplicationRfNotify
 * @brief  Notify the application when the radio switch state
 *         Called from the Z-Wave PROTOCOL When radio switch from Rx to Tx or from Tx to Rx
 *         or when the modulator PA (Power Amplifier) turn on/off
 * @param  BYTE rfState
 * @retval None
 */
void          
ApplicationRfNotify(
    BYTE rfState            /* IN state of the RF, the available values is as follow:
                               ZW_RF_TX_MODE: The RF switch from the Rx to Tx mode, the modualtor is started and PA is on
                               ZW_RF_PA_ON: The RF in the Tx mode, the modualtor PA is turned on
                               ZW_RF_PA_OFF: the Rf in the Tx mode, the modulator PA is turned off
                               ZW_RF_RX_MODE: The RF switch from Tx to Rx mode, the demodulator is started.*/
) {
    UNUSED(rfState);
}

/**
 * @func   ApplicationInitHW
 * @brief  Initialization of non Z-Wave module hardware
 * @param  None
 * @retval Returning FALSE from this function will force the API into
 *         production test mode.
 */
BYTE                       
ApplicationInitHW(
    SW_WAKEUP bWakeupReason 
) {
    wakeupReason = bWakeupReason;
    gpio_DriverInit(TRUE);

    Transport_OnApplicationInitHW(bWakeupReason);
    InitCmmZw();
    InitBatteryMonitor(wakeupReason);

    #ifdef USE_LED_DEBUG
    InitLed();
    #endif

    return(TRUE);
}

/**
 * @func   ApplicationInitSW
 * @brief  Initialization of the Application Software variables and states
 * @param  None
 * @retval BYTE
 */
BYTE               
ApplicationInitSW(
    ZW_NVM_STATUS nvmStatus
) {
    currentState = STATE_APP_STARTUP;
    IsSentWakeupNotify = FALSE;
    IsSentBattRepport = FALSE;

    UNUSED(nvmStatus);
    InitProcessManager(AppStateManager);
    EventSchedulerInit(AppStateManager);

    LoadConfiguration();

    if ((FALSE == BatterySensorRead(NULL)) && (ST_BATT_DEAD ==BatteryMonitorState()))
    {
        ZW_DEBUG_SENS_SEND_NL();
        ZW_DEBUG_SENS_SEND_STR("Go to sleep!");
        ZW_DEBUG_SENS_SEND_NL();
        PowerDownNow();
        ChangeState(STATE_APP_POWERDOWN);
        return (TRUE);
    }

    if (FALSE == BatteryInit( BATT_MODE_NOT_LISTENING, wakeupReason))
    {
        PowerDownNow();
        ChangeState(STATE_APP_POWERDOWN);
        return (TRUE);
    }

    /* Setup AGI group lists */
    AGI_Init();
    AGI_LifeLineGroupSetup(agiTableLifeLine, (sizeof(agiTableLifeLine)/sizeof(CMD_CLASS_GRP)), GroupName, ENDPOINT_ROOT);
    CC_Version_SetApplicationVersionInfo(APP_VERSION, APP_REVISION, APP_PATCH_VERSION, ZAF_BUILD_NO);

    Transport_OnApplicationInitSW (&m_AppNIF, &ZCB_SetPowerDownTimeoutWakeUpStateCheck);

    InitNotification();
    LoadNotificationConfig();

    ZCB_EventSchedulerEventAdd((EVENT_WAKEUP)wakeupReason);

    InitPinWakeupMKL();
    ZwReportGoAwake();

    SENS_DBG(ZW_UART_tx_send_str("\n!S!\n"));

    return(TRUE);
}

/**
 * @func   ApplicationTestPoll
 * @brief  This function is called when the slave enters test mode.
 *         Code will not exit until it is reset
 * @param  None
 * @retval None
 */
void
ApplicationTestPoll(void) {}

/**
 * @func   ApplicationPoll
 * @brief  Application poll function for the slave application
 * @param  None
 * @retval None
 */
void                 
ApplicationPoll(void) {
    #ifdef WATCHDOG_ENABLED
    ZW_WatchDogKick(); /* Kick watchdog */
    #endif

    TaskApplicationPoll();
}

/**
 * @func   Transport_ApplicationCommandHandlerEx
 * @brief  Handling of a received application commands and requests
 * @param  RECEIVE_OPTIONS_TYPE_EX_P    rxOpt
 *         ZW_APPLICATION_TX_BUFFER_P   pCmd
 *         BYTE                         cmdLength
 * @retval None
 */
received_frame_status_t                                
Transport_ApplicationCommandHandlerEx(
    RECEIVE_OPTIONS_TYPE_EX *rxOpt, /* IN receive options of type RECEIVE_OPTIONS_TYPE_EX  */
    ZW_APPLICATION_TX_BUFFER *pCmd, /* IN  Payload from the received frame */
    BYTE   cmdLength                /* IN Number of command bytes including the command */
) {
    received_frame_status_t frame_status = RECEIVED_FRAME_STATUS_NO_SUPPORT;

    switch (pCmd->ZW_Common.cmdClass) {
    case COMMAND_CLASS_VERSION:
        frame_status = handleCommandClassVersion(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_ASSOCIATION_GRP_INFO:
        frame_status = handleCommandClassAssociationGroupInfo( rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_ASSOCIATION_V2:
        frame_status = handleCommandClassAssociation(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_POWERLEVEL:
        frame_status = handleCommandClassPowerLevel(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_MANUFACTURER_SPECIFIC:
        frame_status = handleCommandClassManufacturerSpecific(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_ZWAVEPLUS_INFO:
        frame_status = handleCommandClassZWavePlusInfo(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_BATTERY:
        frame_status = handleCommandClassBattery(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_WAKE_UP:
        frame_status = HandleCommandClassWakeUp(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_NOTIFICATION_V8:
        frame_status = handleCommandClassNotification(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_SUPERVISION:
        frame_status = handleCommandClassSupervision(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_SECURITY_2:
        frame_status = handleCommandClassSecurity(rxOpt, pCmd, cmdLength);
        break;
    }

    return frame_status;
}

/**
 * @func   handleCommandClassVersionAppl
 * @brief  Application specific Command Class Version handler
 * @param  None
 * @retval BYTE
 */
BYTE
handleCommandClassVersionAppl( 
    BYTE cmdClass 
) {
    BYTE commandClassVersion = UNKNOWN_VERSION;

    switch (cmdClass) {
    case COMMAND_CLASS_VERSION:
        commandClassVersion = CommandClassVersionVersionGet();
        break;

    case COMMAND_CLASS_POWERLEVEL:
        commandClassVersion = CommandClassPowerLevelVersionGet();
        break;

    case COMMAND_CLASS_MANUFACTURER_SPECIFIC:
        commandClassVersion = CommandClassManufacturerVersionGet();
        break;

    case COMMAND_CLASS_ASSOCIATION:
        commandClassVersion = CommandClassAssociationVersionGet();
        break;

    case COMMAND_CLASS_ASSOCIATION_GRP_INFO:
        commandClassVersion = CommandClassAssociationGroupInfoVersionGet();
        break;

    case COMMAND_CLASS_DEVICE_RESET_LOCALLY:
        commandClassVersion = CommandClassDeviceResetLocallyVersionGet();
        break;

    case COMMAND_CLASS_ZWAVEPLUS_INFO:
        commandClassVersion = CommandClassZWavePlusVersion();
        break;

    case COMMAND_CLASS_BATTERY:
        commandClassVersion = CommandClassBatteryVersionGet();
        break;

    case COMMAND_CLASS_WAKE_UP:
        commandClassVersion = CmdClassWakeupVersion();
        break;

    case COMMAND_CLASS_NOTIFICATION_V8:
        commandClassVersion = CommandClassNotificationVersionGet();
        break;

    case COMMAND_CLASS_SUPERVISION:
        commandClassVersion = CommandClassSupervisionVersionGet();
        break;
   
    case COMMAND_CLASS_TRANSPORT_SERVICE_V2:
        commandClassVersion = TRANSPORT_SERVICE_VERSION_V2;
        break;
    }

    return commandClassVersion;
}

/**
 * @func   ApplicationSlaveUpdate
 * @brief  Inform a slave application that a node information is received.
 *         Called from the slave command handler when a node information frame
 *         is received and the Z-Wave protocol is not in a state where it is needed.
 * @param  BYTE bStatus
 *         BYTE bNodeID
 *         BYTE_P pCmd
 *         BYTE bLen
 * @retval None
 */
void
ApplicationSlaveUpdate(
    BYTE bStatus,     /*IN  Status event */
    BYTE bNodeID,     /*IN  Node id of the node that send node info */
    BYTE* pCmd,       /*IN  Pointer to Application Node information */
    BYTE bLen         /*IN  Node info length */
) {
    UNUSED(bStatus);
    UNUSED(bNodeID);
    UNUSED(pCmd);
    UNUSED(bLen);
}

/**
 * @func   handleNbrFirmwareVersions
 * @brief  Read number of firmwares in application.
 * @param  None
 * @retval BYTE
 */
BYTE
handleNbrFirmwareVersions() {
    return 1; /* CHANGE THIS - firmware 0 version*/
}

/**
 * @func   handleGetFirmwareVersion
 * @brief  Read application firmware version informations
 * @param  BYTE bFirmwareNumber
 *         VG_VERSION_REPORT_V2_VG_P pVariantgroup
 * @retval None
 */
void
handleGetFirmwareVersion(
    BYTE bFirmwareNumber,
    VG_VERSION_REPORT_V2_VG* pVariantgroup
) {
    /* firmware 0 version and sub version */
    if (bFirmwareNumber == 0) {
        pVariantgroup->firmwareVersion    = APP_VERSION;
        pVariantgroup->firmwareSubVersion = APP_REVISION;
    } else {
        /* Just set it to 0 if firmware n is not present */
        pVariantgroup->firmwareVersion = 0;
        pVariantgroup->firmwareSubVersion = 0;
    }
}

/**
 * @func   LearnCompleted
 * @brief  Callback which is called on learnmode completed
 *         Application specific handling of LearnModeCompleted - called from
 *         slave_learn.c
 * @param  None
 * @retval None
 */
void
LearnCompleted(
    BYTE bNodeID    /* IN The nodeID assigned */
) {               
    SENS_DBG(ZW_UART_tx_send_str("LearnCompleted "));
    SENS_DBG(ZW_UART_tx_send_num(bNodeID));
    SENS_DBG(ZW_UART_tx_send_nl());

    if (bNodeID != NODE_BROADCAST) {
        myNodeID = bNodeID;
        if (myNodeID == 0) {
            SetDefaultConfiguration();
        }
        else {
            /* if we have been included to the network extend the wakeup time */
            ZCB_WakeUpStateSet(TRUE);
        }
    }
    else {

    }

    Transport_OnLearnCompleted(bNodeID);
    ZCB_EventSchedulerEventAdd((EVENT_APP)EVENT_APP_LEARN_MODE_FINISH);
}

/**
 * @func   GetMyNodeID
 * @brief  Get the device node ID
 * @param  None
 * @retval BYTE
 */
BYTE
GetMyNodeID(void) {
    return myNodeID;
}

/**
 * @func   AppState
 * @brief  Return application statemachine state
 * @param  None
 * @retval None
 */
STATE_APP
GetAppState(void) {
    return currentState;
}

/**
 * @func   AppStateManager
 * @brief  None
 * @param  EVENT_APP ev
 * @retval None
 */
void
AppStateManager( 
    EVENT_APP ev
) {
    ZW_DEBUG_SENS_SEND_STR("AppStMan ev ");
    ZW_DEBUG_SENS_SEND_NUM(ev);
    ZW_DEBUG_SENS_SEND_STR(" st ");
    ZW_DEBUG_SENS_SEND_NUM(currentState);
    ZW_DEBUG_SENS_SEND_NL();

    if (EVENT_SYSTEM_WATCHDOG_RESET == ev) {
       /* Force state change to activate watchdog-reset without taking care of current state.*/
       ChangeState(STATE_APP_WATCHDOG_RESET);
    }

    switch (currentState) {
    case STATE_APP_STARTUP:
        ChangeState(STATE_APP_IDLE);
        break;

    case STATE_APP_IDLE:
        if ((EVENT_BUTTON_PRESS == ev) | (EVENT_WAKUP_NOTIFY == ev)) {
            ZCB_WakeUpStateSet(TRUE);
            WakeUpNotification();
            IsSentWakeupNotify = TRUE;
            IsSentBattRepport = TRUE;
            SENS_DBG(ZW_UART_tx_send_str("Wakup notify\n"));
            break;
        }

        if (EVENT_BUTTON_ADDNODE == ev) {
            if (myNodeID != 0) {
                SENS_DBG(ZW_UART_tx_send_str("learn_exclusion\n"));
                ZCB_SetPowerDownTimeout(REMOVENODE_POWERDOWNTIMEOUT);
                StartLearnModeNow(LEARN_MODE_EXCLUSION);
            } 
            else {
                SENS_DBG(ZW_UART_tx_send_str("learn_inclusion\n"));
                ZCB_SetPowerDownTimeout(ADDNODE_POWERDOWNTIMEOUT);
                StartLearnModeNow(LEARN_MODE_INCLUSION);
            }
            gbLearnMode = TRUE;
            UpdZwNetw(ZW_INVALID_NETW);
            ChangeState(STATE_APP_LEARN_MODE);
        }

        if (EVENT_BUTTON_RESET == ev) {
            gProfile.profile_MS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_NA_V2;
            gProfile.profile_LS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_LIFELINE;
            ZCB_WakeUpStateSet(FALSE);
            ZCB_SetPowerDownTimeout(RESETLOCAL_POWERDOWNTIMEOUT);
            handleCommandClassDeviceResetLocally(&gProfile, ZCB_DeviceResetLocallyDone);
            UpdZwNetw(ZW_NOT_NETW);
            WakeupMKL();
            CmdZwNetwReportStatus();
            SENS_DBG(ZW_UART_tx_send_str("** DeviceResetLocally **"));
        }

        if (EVENT_BUTTON_KEEP_AWAKE == ev) {
            gAwakeTimeCount = KEEP_AWAKE_MAX_COUNT;
            gAwakeTimeCount--;
            ZCB_WakeUpStateSet(TRUE);
            ZCB_SetPowerDownTimeout(TIMEOUT_KEEP_AWAKE);
        }

        if (EVENT_REPORT_BATTLEVL == ev) {
            if (JOB_STATUS_SUCCESS != SendBattReport(ZCB_BattReportSentDone)) {
                ActivateBattNotificationTrigger();
            }

            IsSentBattRepport = TRUE;
            SENS_DBG(ZW_UART_tx_send_str("Bat report\n"));
        }

        if (EVENT_SENSOR_REPORT == ev) {
            ZCB_EventSchedulerEventAdd(EVENT_DOOR_REPORT_JOB);
            ChangeState(STATE_APP_TRANSMIT_DATA);
            SENS_DBG(ZW_UART_tx_send_str("Sens report\n"));
        }
        break;

    case STATE_APP_LEARN_MODE:
        if (EVENT_APP_LEARN_MODE_FINISH == ev) {
            if (myNodeID != 0 && myNodeID != 0xFF) {
                UpdZwNetw(ZW_IN_NETW);
            }
            else {
                UpdZwNetw(ZW_NOT_NETW);
            }
            
            if (gbLearnMode) {
                WakeupMKL();
                CmdZwNetwReportStatus();
                gbLearnMode = FALSE;
            }
            ChangeState(STATE_APP_IDLE);
            SENS_DBG(ZW_UART_tx_send_str("learn_finish\n"));
        }

        if (EVENT_BUTTON_ADDNODE == ev) {
            SENS_DBG(ZW_UART_tx_send_str("learn_disable\n"));
            StartLearnModeNow(LEARN_MODE_DISABLE);
            ChangeState(STATE_APP_IDLE);
        }

        break;

    case STATE_APP_TRANSMIT_DATA:
        if ((ev == EVENT_BUTTON_ADDNODE) || (ev == EVENT_BUTTON_RESET)) {
            BYTE event;
            while (TRUE == ZCB_EventDequeue(&event)) {}
            ChangeState(STATE_APP_IDLE);
            AppStateManager(ev);
            break;
        } 

        if (EVENT_APP_NEXT_EVENT_JOB == ev) {
            BYTE event;
            if (TRUE == ZCB_EventDequeue(&event)) {
                ZCB_EventSchedulerEventAdd(event);
            }
            else {
                ZCB_EventSchedulerEventAdd(EVENT_APP_FINISH_EVENT_JOB);
            }
        }
 
        if (EVENT_DOOR_REPORT_JOB == ev) {
            SensorDOORreport();
        }

        if (EVENT_APP_FINISH_EVENT_JOB == ev) {
            ChangeState(STATE_APP_IDLE);
        }
        break;

    case STATE_APP_WATCHDOG_RESET:
        break;

    case STATE_APP_POWERDOWN:
        break;
    }
}

/**
 * @func   ChangeState
 * @brief  Change state
 * @param  STATE_APP st
 * @retval None
 */
void
ChangeState(
    STATE_APP st
) {
    ZW_DEBUG_SENS_SEND_STR("ChangeState st = ");
    ZW_DEBUG_SENS_SEND_NUM(currentState);
    ZW_DEBUG_SENS_SEND_STR(" -> new st = ");
    ZW_DEBUG_SENS_SEND_NUM(st);
    ZW_DEBUG_SENS_SEND_NL();
    currentState = st;
}

/**
 * @func   AppPowerDownReady
 * @brief  Check if application is ready to go to power down state
 * @param  None
 * @retval TRUE if ready
 */
BYTE 
AppPowerDownReady() {
    BOOL status = FALSE;

    #if 0
    ZW_UART_tx_send_str(" ev=");
    ZW_UART_tx_send_num(eventQueue);
    ZW_UART_tx_send_str(", st=");
    ZW_UART_tx_send_num(AppState());
    #endif

    if (STATE_APP_IDLE == GetAppState()) {
        if ((TRUE == TimeToSendBattReport()) && (!IsSentBattRepport))
        {
            ZCB_EventSchedulerEventAdd(EVENT_REPORT_BATTLEVL);
        }
        else if ((SW_WAKEUP_WUT == wakeupReason) && (!IsSentWakeupNotify)) 
        {
            ZCB_EventSchedulerEventAdd(EVENT_WAKUP_NOTIFY);
        }
        else if (gAwakeTimeCount != 0) 
        {
            ZCB_WakeUpStateSet(TRUE);
            ZCB_SetPowerDownTimeout(TIMEOUT_KEEP_AWAKE);
            gAwakeTimeCount--;
        }
        else {
            status = TRUE;
        }
    }

    if (!repMcuSleep && status) { 
        repMcuSleep = TRUE;
        ZwReportGosleep();
        SensStoreVal();
    }

    return repMcuSleep;
}

/**
 * @func   SetDefaultConfiguration
 * @brief  Function resets configuration to default values
 * @param  None
 * @retval
 */
void                
SetDefaultConfiguration(void) {
    AssociationInit(TRUE);
    MemoryPutByte((WORD)&EEOFFSET_alarmStatus_far, 0xFF);
    SaveNumbTxFalse(0);
    SetDefaultBatteryConfiguration(DEFAULT_SLEEP_TIME);
    MemoryPutByte((WORD)&EEOFFSET_MAGIC_far, APPL_MAGIC_VALUE);
    CmdClassWakeUpNotificationMemorySetDefault();
}

/**
 * @func   LoadConfiguration
 * @brief  This function loads the application settings from EEPROM
 *         If no settings are found, default values are used and saved
 * @param  None
 * @retval BYTE
 */
void                 
LoadConfiguration(void) {
    MemoryGetID(NULL, &myNodeID);

    if (myNodeID != 0 && myNodeID != 0xFF) {
        UpdZwNetw(ZW_IN_NETW);
    }
    else {
        UpdZwNetw(ZW_NOT_NETW);
    }

    ManufacturerSpecificDeviceIDInit();
    SensLoadVal();
    cntTxFalse = GetNumbTxFalse();
    SetWakeUpConfiguration(WAKEUP_PAR_DEFAULT_SLEEP_TIME, DEFAULT_SLEEP_TIME);
    SetWakeUpConfiguration(WAKEUP_PAR_MAX_SLEEP_TIME, MAX_SLEEP_TIME);
    SetWakeUpConfiguration(WAKEUP_PAR_MIN_SLEEP_TIME, MIN_SLEEP_TIME);
    SetWakeUpConfiguration(WAKEUP_PAR_SLEEP_STEP, STEP_SLEEP_TIME);

    /* Check to see, if any valid configuration is stored in the EEPROM */
    if (MemoryGetByte((WORD)&EEOFFSET_MAGIC_far) == APPL_MAGIC_VALUE)
    {
        /* Initialize association module */
        AssociationInit(FALSE);

        loadStatusPowerLevel(ZCB_StopPowerDownTimer, ZCB_StartPowerDownTimer);
        /* There is a configuration stored, so load it */
        LoadBatteryConfiguration();
    }
    else
    {
        Transport_SetDefault();
        ZW_SetDefault();
        loadInitStatusPowerLevel(ZCB_StopPowerDownTimer, ZCB_StartPowerDownTimer);
        SetDefaultConfiguration();
    }
}

/**
 * @func   LoadNotificationConfig
 * @brief  None
 * @param  None
 * @retval None
 */
void
LoadNotificationConfig(void) {
    BYTE len;
    gProfile.profile_MS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL;
    gProfile.profile_LS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_LIFELINE;

    gNotificationType = NOTIFICATION_TYPE_ACCESS_CONTROL;

    AddNotification(
        &gProfile,
        NOTIFICATION_TYPE_ACCESS_CONTROL,
        &suppportedEventsDOOR,
        2,
        FALSE,
        0);

    ConvertToEventSensorNotification(
        SENSOR_DOOR,
        &gNotificationEvent,
        &gNotificationParam,
        &len);
          
    NotificationEventTrigger(  
        &gProfile, 
        gNotificationType,
        gNotificationEvent,
        &gNotificationParam,
        len,
        ENDPOINT_ROOT);
}

/**
 * @func   SensorDOORreport
 * @brief  None
 * @param  None
 * @retval None
 */
void
SensorDOORreport() {
    if ((GetFlagSensReport(SENSOR_DOOR)) && (myNodeID != 0)){
        JOB_STATUS jobStatus = JOB_STATUS_BUSY;
        BYTE len;

        gProfile.profile_MS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL;
        gProfile.profile_LS = ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_LIFELINE;
        gNotificationType = NOTIFICATION_TYPE_ACCESS_CONTROL;

        ConvertToEventSensorNotification(
            SENSOR_DOOR,
            &gNotificationEvent,
            &gNotificationParam,
            &len);
   
        SENS_DBG(ZW_UART_tx_send_str("DOOR"));
        SENS_DBG(ZW_UART_tx_send_str(" Val "));
        SENS_DBG(ZW_UART_tx_send_w_num(gNotificationEvent));
        SENS_DBG(ZW_UART_tx_send_str("\n"));
           
        NotificationEventTrigger(  
            &gProfile, 
            gNotificationType,
            gNotificationEvent,
            &gNotificationParam,
            len,
            ENDPOINT_ROOT);
 
        jobStatus = UnsolicitedNotificationAction(
                        &gProfile, 
                        ENDPOINT_ROOT,
                        ZCB_SensorNotificationCallback);  
       
        if (JOB_STATUS_SUCCESS != jobStatus) {
            TRANSMISSION_RESULT transmissionResult;
            transmissionResult.nodeId = 0;
            transmissionResult.status = TRANSMIT_COMPLETE_FAIL;
            ZCB_SensorNotificationCallback(&transmissionResult);
        }
    }
    else {
        ZCB_EventSchedulerEventAdd(EVENT_APP_FINISH_EVENT_JOB);
    }
}

/**
 * @func   ZCB_SensorNotificationCallback
 * @brief  Function to be called when done handling 
 * @param  bStatus
 * @retval None
 */
PCB(ZCB_SensorNotificationCallback)(
    TRANSMISSION_RESULT * pTransmissionResult
) {
    if (TRANSMIT_COMPLETE_OK == pTransmissionResult->status) {
        SENS_DBG(ZW_UART_tx_send_str("N Tx OK\n"));
        FlagSensReportNewState(SENSOR_DOOR, DISABLE);
        SaveNumbTxFalse(0);
        ZCB_EventSchedulerEventAdd(EVENT_APP_FINISH_EVENT_JOB);
    }
    else {
        IsSentBattRepport = TRUE;
        cntTxFalse++;
        SaveNumbTxFalse(cntTxFalse);
        WakeupMKL();
        CmdZwTxFailReport(cntTxFalse);
        ChangeState(STATE_APP_IDLE);

        SENS_DBG(ZW_UART_tx_send_str("Tx False "));
        SENS_DBG(ZW_UART_tx_send_num(cntTxFalse));
        SENS_DBG(ZW_UART_tx_send_str("\n"));
    }
}

/**
 * @func   ZCB_DeviceResetLocallyDone
 * @brief  callback function perform reset device
 * @param  None
 * @retval None
 */
PCB(ZCB_DeviceResetLocallyDone)(
    TRANSMISSION_RESULT * pTransmissionResult
) {
    if (TRANSMISSION_RESULT_FINISHED == pTransmissionResult->isFinished)
    {
        Transport_SetDefault();
        SetDefaultConfiguration();
        ZW_SetDefault();
        ZW_WatchDogEnable(); /* Reset asic */
        for (;;) {}
    }
    else {

    }
}

/**
 * @func   ZCB_BattReportSentDone
 * @brief  Call back function used when sending battery report
 * @param  None
 * @retval
 */    
PCB(ZCB_BattReportSentDone)(
    TRANSMISSION_RESULT * pTransmissionResult
) {
    if (TRANSMIT_COMPLETE_OK == pTransmissionResult->status)
    {
        SaveNumbTxFalse(0);
    }
    else {
        // If one of the nodes does not receive the battery status, we activate the trigger once again.

        cntTxFalse++;
        SaveNumbTxFalse(cntTxFalse);

        SENS_DBG(ZW_UART_tx_send_str("Tx False "));
        SENS_DBG(ZW_UART_tx_send_num(cntTxFalse));
        SENS_DBG(ZW_UART_tx_send_str("\n"));
    }

    if (TRANSMISSION_RESULT_FINISHED == pTransmissionResult->isFinished)
    {
        ZCB_EventSchedulerEventAdd(EVENT_APP_NEXT_EVENT_JOB);
    }
}

/**
 * @func   ApplicationSecurityEvent
 * @brief  Called when protocol needs to inform Application about a Security Event.
 *         If the app does not need/want the Security Event the handling can be left empty.
 *           Event E_APPLICATION_SECURITY_EVENT_S2_INCLUSION_REQUEST_DSK_CSA
 *           If CSA is needed, the app must do the following when this event occures:
 *             1. Obtain user input with first 4 bytes of the S2 including node DSK
 *             2. Store the user input in a response variable of type s_SecurityS2InclusionCSAPublicDSK_t.
 *             3. Call ZW_SetSecurityS2InclusionPublicDSK_CSA(response)
 * @param  None
 * @retval None
 */
void
ApplicationSecurityEvent(
    s_application_security_event_data_t *securityEvent
) {
    switch (securityEvent->event) {
    #ifdef APP_SUPPORTS_CLIENT_SIDE_AUTHENTICATION
    case E_APPLICATION_SECURITY_EVENT_S2_INCLUSION_REQUEST_DSK_CSA:
    {
        ZW_SetSecurityS2InclusionPublicDSK_CSA(&sCsaResponse);
    }   break;   
    #endif /* APP_SUPPORTS_CLIENT_SIDE_AUTHENTICATION */

    default:
        break;
    }
}

/**
 * @func   ApplicationSecureKeysRequested
 * @brief  Set up security keys to request when joining a network.
 * @param  None
 * @retval None
 */
BYTE 
ApplicationSecureKeysRequested() {
    return REQUESTED_SECURITY_KEYS;
}

/**
 * @func   ApplicationSecureAuthenticationRequested
 * @brief  Set up security S2 inclusion authentication to request when joining a network.
 * @param  None
 * @retval None
 */
BYTE 
ApplicationSecureAuthenticationRequested() {
    return REQUESTED_SECURITY_AUTHENTICATION;
}

