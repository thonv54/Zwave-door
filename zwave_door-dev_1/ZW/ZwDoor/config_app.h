/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * Description: Include file for application
 *
 * Author: ThangDH
 *
 * Last Changed By:  Author: ThangDH
 * Revision:         Revision:
 * Last Changed:     Date: 
 *                   Add more preprocessor about config device
 *
 ******************************************************************************/
#ifndef _CONFIG_APP_H_
#define _CONFIG_APP_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#ifdef __C51__
#include "ZW_product_id_enum.h"
#include <commandClassManufacturerSpecific.h>
#endif
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define DEVICE_OPTIONS_MASK                 APPLICATION_NODEINFO_NOT_LISTENING | APPLICATION_NODEINFO_OPTIONAL_FUNCTIONALITY

#define APP_ROLE_TYPE                       ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_SLAVE_SLEEPING_REPORTING
#define APP_NODE_TYPE                       ZWAVEPLUS_INFO_REPORT_NODE_TYPE_ZWAVEPLUS_NODE

#define APP_MANUFACTURER_ID                 MFG_ID_HOME_CONTROLS

#define APP_PRODUCT_TYPE_ID                 PRODUCT_TYPE_ID_ZWAVE_PLUS
#define APP_PRODUCT_ID                      PRODUCT_ID_Lumi_DoorSensor

#define APP_FIRMWARE_ID                     APP_PRODUCT_ID | (APP_PRODUCT_TYPE_ID << 8)

#define APP_DEVICE_ID_TYPE                  DEVICE_ID_TYPE_PSEUDO_RANDOM
#define APP_DEVICE_ID_FORMAT                DEVICE_ID_FORMAT_BIN
#define GENERIC_TYPE                        GENERIC_TYPE_SENSOR_NOTIFICATION
#define SPECIFIC_TYPE                       SPECIFIC_TYPE_NOTIFICATION_SENSOR

#define NUMBER_OF_INDIVIDUAL_ENDPOINTS      0
#define NUMBER_OF_AGGREGATED_ENDPOINTS      0
#define NUMBER_OF_ENDPOINTS                (NUMBER_OF_INDIVIDUAL_ENDPOINTS + NUMBER_OF_AGGREGATED_ENDPOINTS)

#define MAX_ASSOCIATION_GROUPS              1
#define MAX_ASSOCIATION_IN_GROUP            4

#define APP_ICON_TYPE                       ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_ACCESS_CONTROL
#define APP_USER_ICON_TYPE                  ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_ACCESS_CONTROL

#define REQUESTED_SECURITY_AUTHENTICATION   SECURITY_AUTHENTICATION_SSA
#define REQUESTED_SECURITY_KEYS             (SECURITY_KEY_S2_UNAUTHENTICATED_BIT)

#define MSEC_200_POWERDOWNTIMEOUT           2    /* 200 mSec */
#define SEC_2_POWERDOWNTIMEOUT              20
#define SEC_10_POWERDOWNTIMEOUT             100

#define TIMEOUT_KEEP_AWAKE                  100
#define KEEP_AWAKE_MAX_COUNT                6
#define ADDNODE_POWERDOWNTIMEOUT            150
#define REMOVENODE_POWERDOWNTIMEOUT         40
#define RESETLOCAL_POWERDOWNTIMEOUT         40
#define WAKEUP_NOMORE_WAITTIMEOUT           20
#define MAX_NOTIFICATIONS                   3

#define FIRMWARE_UPGRADABLE                 0x00
#define SENSOR_VAL_SIZE                     10  

#define AGITABLE_LIFELINE_GROUP \
  {COMMAND_CLASS_BATTERY, BATTERY_REPORT},\
  {COMMAND_CLASS_NOTIFICATION_V8, NOTIFICATION_REPORT_V8}, \
  {COMMAND_CLASS_DEVICE_RESET_LOCALLY, DEVICE_RESET_LOCALLY_NOTIFICATION}

#define NOTIFICATION_DOOR \
  {NOTIFICATION_EVENT_ACCESS_CONTROL_DOOR_OPEN, NOTIFICATION_EVENT_ACCESS_CONTROL_DOOR_CLOSE} 

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

#endif /* _CONFIG_APP_H_ */

