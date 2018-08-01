/*******************************************************************************
 *
 * Copyright (c) 2017
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: ThanfDH
 *
 * Last Changed By:  $Author: ThangDH $
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date:  20/04/2018 $
 *
 ******************************************************************************/
#ifndef _SERIAL_COMMAND_H_
#define _SERIAL_COMMAND_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "ZW_typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define ZW_INF_BAURATE                      576
#define RX_TIMEOUT                          20
#define WAIT_ACK_TIMEOUT                    5
#define TX_TRIES_TIME                       2
#define TIMER_RESOLUTION                    10
#define WAIT_RES_TIMEOUT                    20

#define BUTTON_1                            1      
#define BUTTON_2                            2       
#define BUTTON_3                            3
#define BUTTON_4                            4
#define BUTTON_5                            5
#define BUTTON_6                            6
#define BUTTON_7                            7
#define BUTTON_8                            8
#define BUTTON_9                            9
#define BUTTON_10                           10

#define FUNID_MZ_MIN                        0x80
#define FUNID_MZ_MAX                        0xFE

#define FUNID_ZM_MIN                        0x00
#define FUNID_ZM_MAX                        0x7F

#define FUNID_ZM                            0
#define FUNID_MZ                            1

#define FRAME_LENGTH_V1                     0x07
#define FRAME_LENGTH_MAX                    0x20
#define FRAME_LENGTH_MIN                    0x07

#define FRAME_ACK                           0x06
#define FRAME_NACK                          0x15
#define FRAME_SOF                           0xB1

#define CMD_TYPE_GET                        0x00
#define CMD_TYPE_RES                        0x01
#define CMD_TYPE_SET                        0x02
#define CMD_TYPE_UPD                        0x03

#define CSUM_INIT_VAL                       0xFF

#define CMD_CTROL_LED                       0x01
#define CMD_TIME_WAKEUP                     0x02

#define CMD_DEVICE_MODE                     0x80
#define CMD_BAT_LEVEL                       0x81
#define CMD_EVNT_BUTT                       0x82
#define CMD_EVNT_TOUCH                      0x83
#define CMD_TEMPERATURE                     0x84
#define CMD_HUMIDITY                        0x85
#define CMD_LUX                             0x86
#define CMD_MOTION                          0x87
#define CMD_DOOR                            0x88
#define CMD_DIM                             0x89
#define CMD_CURTAIN                         0x8A
#define CMD_SWITCH                          0x8B
#define CMD_FAN                             0x8C

#define CMD_START_LEVEL_CHANGE              0x8E
#define CMD_STOP_LEVEL_CHANGE               0x8F

#define CMD_MULTI                           0xAA

#define CMD_ZW_STATE                        0xC0
#define CMD_ZW_NETWORK                      0xC1
#define CMD_ZW_TXFALSE                      0xC2

#define OPT_NOT_USE                         0x00
#define OPT_EXISTCMD_MASK                   0x01

#define ZW_INVALID_NETW                     0x00
#define ZW_NOT_NETW                         0x01
#define ZW_IN_NETW                          0x02

#define BAT_FULL                            0x00
#define BAT_HIGH                            0x01
#define BAT_LOW                             0x02
#define BAT_DEAD                            0x03

#define BUTT_RELEASE                        0x00
#define BUTT_HOLD_03SEC                     0x01
#define BUTT_HOLD_05SEC                     0x02
#define BUTT_HOLD_10SEC                     0x03
#define BUTT_PRESS_1TIME                    0x04
#define BUTT_PRESS_2TIME                    0x05
#define BUTT_PRESS_3TIME                    0x06
#define BUTT_PRESS_4TIME                    0x07
#define BUTT_PRESS_5TIME                    0x08

#define TOUCH_EVNT_ONE_TOUCH                0x01
#define TOUCH_EVNT_DOUBLE_TOUCH             0x02
#define TOUCH_EVNT_SWIPE_UP                 0x03
#define TOUCH_EVNT_SWIPE_DOWN               0x04
#define TOUCH_EVNT_SWIPE_RIGHT              0x05
#define TOUCH_EVNT_SWIPE_LEFT               0x06
#define TOUCH_EVNT_ROTATE_ANTICLKWISE       0x07
#define TOUCH_EVNT_ROTATE_CLKWISE           0x08
#define TOUCH_EVNT_HOLD                     0x09
#define TOUCH_EVNT_TRIPLE_TOUCH             0x0A

#define ZW_GOSLEEP                          0x00
#define ZW_ACTIVE                           0x01

#define PDOOR_OPEN                          0xFF
#define PDOOR_CLOSE                         0x00

#define PMOTION_DETECT                      0xFF
#define PMOTION_NOT_DETECT                  0x00

typedef struct _common_ {
    BYTE cmdid;
    BYTE cmdtype;
} COMMON;

typedef struct _time_config_ {
    BYTE cmdid;
    BYTE cmdtype;
    BYTE timeH;
    BYTE timeL;
} TIMECONF;

typedef struct _device_state_ {
    BYTE cmdid;
    BYTE cmdtype;
    BYTE epoint;
    BYTE level;
} DEVSTATE;

typedef struct _netw_status_ {
    BYTE cmdid;
    BYTE cmdtype;
    BYTE reserved;
    BYTE status;
} NETWSTATUS;

typedef struct _key_config_ {
    BYTE cmdid;
    BYTE cmdtype;
    BYTE no;
    BYTE evkey;
} KEYSTATUS;

typedef struct _sensor_level_ {
    BYTE cmdid;
    BYTE cmdtype;
    BYTE levelH;
    BYTE levelL;
} SENSMULTILEVEL;

typedef struct _multi_cmd_ {
    BYTE cmdid;
    BYTE cmdtype;
    BYTE numbCmd;
    BYTE length;
} MULTICMD;

typedef struct _door_cmd_ {
    BYTE cmdid;
    BYTE cmdtype;
    BYTE reserved;
    BYTE status;    
} DOORSTATUS;

typedef struct _numb_tx_fail_ {
    BYTE cmdid;
    BYTE cmdtype;
    BYTE numbH;
    BYTE numbL;
} NUMBTXFAIL;

typedef union _CMD_BUFFER_ {
    COMMON scommon;
    MULTICMD multicmd;
    TIMECONF stimeconfig;
    DEVSTATE sdevlevel;
    SENSMULTILEVEL ssenslevl;
    NETWSTATUS netwstatus;
	NUMBTXFAIL txfailstatus;
    KEYSTATUS keyconfig;
    DOORSTATUS door;
} CMD_BUFFER, *CMD_BUFFER_P;

#endif /* END FILE */
