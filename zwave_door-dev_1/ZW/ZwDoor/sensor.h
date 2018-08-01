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
#ifndef _SENSOR_H_
#define _SENSOR_H_ 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ZW_typedefs.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef enum {
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;

#define SENSOR_TEMP                         0x01
#define SENSOR_HUMI                         0x02
#define SENSOR_LUX                          0x04

#define SENSOR_DEFAULT                      SENSOR_TEMP

#define SENSOR_PIR                          0x10
#define SENSOR_DOOR                         0x20

#define SENSOR_MASK                         (SENSOR_TEMP|SENSOR_HUMI|SENSOR_LUX|SENSOR_DOOR)

#define DOOR_OPEN                           0xFF
#define DOOR_CLOSE                          0x00

#define MOTION_DETECT                       0xFF
#define MOTION_NOT_DETECT                   0x00 

#define SENSOR_TYPE_TEMPERATURE_AIR         0x01    
#define SENSOR_TYPE_LUMINANCE               0x03
#define SENSOR_TYPE_HUMIDITY                0x05

#define SENSOR_TYPE_DEFAULT                 SENSOR_TYPE_TEMPERATURE_AIR

#define SUPPORT_TEMPERATURE_AIR_MASK        0x01
#define SUPPORT_LUMINANCE_MASK              0x04
#define SUPPORT_HUMIDITY_MASK               0x10

#define SENSOR_TEMPERATURE_UNIT_CELSIUS     0x00
#define SENSOR_LUMINANCE_UNIT_LUX           0x01
#define SENSOR_HUMIDITY_UNIT_PERCENTAGE     0x00

#define SENSOR_PRECISION_SHIFT              5
#define SENSOR_UNIT_SHIFT                   3
#define SENSOR_SIZE_SHIFT                   0

#define SENSOR_SUPPORTED_MASK1             (SUPPORT_TEMPERATURE_AIR_MASK | \
                                            SUPPORT_LUMINANCE_MASK | \
                                            SUPPORT_HUMIDITY_MASK)

typedef struct _SENSOR_PROPERTY_ {
    BYTE unit;
    BYTE precision;
    BYTE size;
} sensor_property;

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
 * @func   GetFlagSensReport
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
GetFlagSensReport(
    BYTE sensorMask
);

/**
 * @func   SensSetAllReport
 * @brief  None
 * @param  None
 * @retval None
 */
void
SensSetAllReport(
    BOOL bEnable
);


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
);

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
);

/**
 * @func   SensGetVal
 * @brief  None
 * @param  None
 * @retval None
 */
WORD
SensGetVal(
    BYTE sensorMask
);

/**
 * @func   getSensorProperty  
 * @brief  None
 * @param  None
 * @retval 
 */
BYTE
getSensorProperty(
    BYTE typeSensor
);

BYTE
getSensorScaleSupport(
    BYTE typeSensor
);

BOOL
IsSensorSupported(
    BYTE typeSensor
);

void
SensLoadVal();

void
SensStoreVal();

#endif

