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
#include "zwsensconvert.h"
#include "sensor.h"
#include "CommandClassNotification.h"
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
 * @func   ConverToSensorType
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
ConverToSensorType(
    BYTE sensorMask
) {
    BYTE sensorType;

    switch(sensorMask) {
    case SENSOR_TEMP:
        sensorType = SENSOR_TYPE_TEMPERATURE_AIR;
        break;

    case SENSOR_HUMI:
        sensorType = SENSOR_TYPE_HUMIDITY;
        break;

    case SENSOR_LUX:
        sensorType = SENSOR_TYPE_LUMINANCE;
        break;
    }

    return sensorType;
}

void
ConvertToEventSensorNotification(
    BYTE sensorMask,
    BYTE_P pEvent,
    BYTE_P pParam,
    BYTE_P pParamLen
) {
    BYTE sensVal = (BYTE)SensGetVal(sensorMask);

    switch(sensorMask) {
    case SENSOR_PIR:
        if (sensVal == MOTION_DETECT) {
			*pEvent = NOTIFICATION_EVENT_HOME_SECURITY_MOTION_DETECTION_UNKNOWN_LOCATION;
            *pParamLen = 0;
        }
        else if (sensVal == MOTION_NOT_DETECT) {
            *pEvent = NOTIFICATION_EVENT_HOME_SECURITY_NO_EVENT;
            *pParam = NOTIFICATION_EVENT_HOME_SECURITY_MOTION_DETECTION_UNKNOWN_LOCATION;
            *pParamLen = 1;
        }
        break;

    case SENSOR_DOOR:
        if (sensVal == DOOR_CLOSE) {
            *pEvent = NOTIFICATION_EVENT_ACCESS_CONTROL_DOOR_CLOSE;
        }
        else if (sensVal == DOOR_OPEN) {
            *pEvent = NOTIFICATION_EVENT_ACCESS_CONTROL_DOOR_OPEN;
        }

        pParamLen = 0;
        break;
    }
}

/* END FILE */

