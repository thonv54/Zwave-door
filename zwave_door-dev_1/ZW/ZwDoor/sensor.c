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
#include "sensor.h"
#include "zw_uart_api.h"
#include "eeprom.h"
#include <ZW_mem_api.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typedef struct _sens_value_ {
    WORD wTemperature;
    WORD wHumidity;
    WORD wLuminance;
    BYTE bMotion;
    BYTE bDoorSt;
    BYTE bFlagReport;
} sensval;

static BYTE sensValBuff[SENSOR_VAL_SIZE];

static sensval *sensorVal;
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
 * @func   SensLoadVal
 * @brief  None
 * @param  None
 * @retval None
 */
void
SensLoadVal() {
    MemoryGetBuffer((WORD) EEOFFSET_SensorVal_far, sensValBuff, SENSOR_VAL_SIZE);
    sensorVal = (sensval*) sensValBuff;
}

/**
 * @func   SensStoreVal
 * @brief  None
 * @param  None
 * @retval None
 */
void
SensStoreVal() {
    MemoryPutBuffer((WORD) EEOFFSET_SensorVal_far, sensValBuff, SENSOR_VAL_SIZE, NULL);
}

/**
 * @func   FlagSensReportNewState
 * @brief  None
 * @param  None
 * @retval None
 */
void
FlagSensReportNewState(
    BYTE sensorMask,
    FunctionalState state
) {
    if (state == ENABLE) {
        sensorVal->bFlagReport |= sensorMask;
    }
    else if (state == DISABLE){
        sensorVal->bFlagReport &= ~sensorMask;
    }
}

/**
 * @func   SensSetAllReport
 * @brief  None
 * @param  None
 * @retval None
 */
void
SensSetAllReport(
    BOOL bEnable
) {
    if (bEnable) 
        sensorVal->bFlagReport = SENSOR_MASK;
    else
        sensorVal->bFlagReport = 0;
}


/**
 * @func   GetFlagSensReport
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
GetFlagSensReport(
    BYTE sensorMask
) {
    if ((sensorVal->bFlagReport & sensorMask) != 0) {
        return TRUE;
    }
    
    return FALSE;
}

/**
 * @func   SensValUpdate
 * @brief  None
 * @param  None
 * @retval None
 */
void
SensValUpdate(
    BYTE sensorMask,
    WORD sensVal
) {
    switch(sensorMask) {
    case SENSOR_PIR:
        sensorVal->bMotion = (BYTE)sensVal;
        break;
  
    case SENSOR_DOOR:
        sensorVal->bDoorSt = (BYTE)sensVal;
        break;
    
    case SENSOR_TEMP:
        sensorVal->wTemperature = sensVal;
        break;
   
    case SENSOR_HUMI:
        sensorVal->wHumidity = sensVal;
        break;

    case SENSOR_LUX:
        sensorVal->wLuminance = sensVal;
        break;  
    }

    FlagSensReportNewState(sensorMask, ENABLE);
}

/**
 * @func   SensGetVal
 * @brief  None
 * @param  None
 * @retval None
 */
WORD
SensGetVal(
    BYTE sensorMask
) {
    WORD val;

    switch(sensorMask) {    
    case SENSOR_TEMP:
		val = sensorVal->wTemperature;
        break;
   
    case SENSOR_HUMI:
		val = sensorVal->wHumidity;
        break;

    case SENSOR_LUX:
		val = sensorVal->wLuminance;
        break;  

    case SENSOR_PIR:
		val = sensorVal->bMotion;
        break;
  
    case SENSOR_DOOR:
		val = sensorVal->bDoorSt;
        break;
    }

    return val;
}

/**
 * @func   getSensorProperty  
 * @brief  None
 * @param  None
 * @retval 
 */
BYTE
getSensorProperty(
    BYTE typeSensor
) {
    sensor_property sens;
    BYTE property;

    switch(typeSensor) {
    case SENSOR_TYPE_LUMINANCE:
        sens.unit = SENSOR_LUMINANCE_UNIT_LUX;
        sens.precision = 0;
        sens.size = 2;
        break;

    case SENSOR_TYPE_HUMIDITY:
        sens.unit = SENSOR_HUMIDITY_UNIT_PERCENTAGE;
        sens.precision = 1;
        sens.size = 2;
        break;

	case SENSOR_TYPE_TEMPERATURE_AIR:
        sens.unit = SENSOR_TEMPERATURE_UNIT_CELSIUS;
        sens.precision = 1;
        sens.size = 2;
        break;
    }

	property = (BYTE)(sens.precision << SENSOR_PRECISION_SHIFT) +
               (BYTE)(sens.unit << SENSOR_UNIT_SHIFT) + 
               (BYTE)(sens.size << SENSOR_SIZE_SHIFT);

    return property;
}

BYTE
getSensorScaleSupport(
    BYTE typeSensor
) {
    BYTE retVal = 0;

    switch(typeSensor) {
    case SENSOR_TYPE_TEMPERATURE_AIR:
        retVal = 1 << SENSOR_TEMPERATURE_UNIT_CELSIUS; /* Support DegC */
        break;

    case SENSOR_TYPE_LUMINANCE:
        retVal = 1 << SENSOR_LUMINANCE_UNIT_LUX; /* Support Lux */
        break;

    case SENSOR_TYPE_HUMIDITY:
        retVal = 1 << SENSOR_HUMIDITY_UNIT_PERCENTAGE; /* Support percentage */
        break;
    }

    return retVal;
}

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
IsSensorSupported(
    BYTE typeSensor
) {
    BYTE sensorMask;
    BYTE typeTemp = typeSensor - 1;

    if (typeTemp < 8) {
		sensorMask = 1 << typeTemp;
        typeTemp = sensorMask & SENSOR_SUPPORTED_MASK1; /* :( */
        if (typeTemp == sensorMask)
             return TRUE;
    }
    else if (typeTemp < 16) {

    }
    else if (typeTemp < 24) {

    }

    return FALSE;
}

