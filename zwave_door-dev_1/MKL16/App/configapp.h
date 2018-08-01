/*******************************************************************************
 *
 * Copyright (c) 2016
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
#ifndef _CONFIG_APP_H_
#define _CONFIG_APP_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "macro.h"
#include "typedefs.h"
/******************************************************************************/
/*                                DEFINITIONS                                 */
/******************************************************************************/
#define DEFINE_EVENT_APP_NBR             0x10   
#define DEFINE_EVENT_KEY_NBR             0x20 
#define DEFINE_EVENT_IN_NBR              0x30 

/**
 * periodic KEY_TIME_SCAN read input
 * number of sample == KEY_SAMPLE_COUNT, check key press
**/

#define KEY_SAMPLE_COUNT                   2
#define KEY_COUNT_IS_PRESS                 2
#define KEY_TIME_SCAN                      25

#define KEY_TIMEOUT_BW2PRESS               400
#define KEY_TIME_IS_HOLD                   500
#define KEY_TIME_HOLD3S                    2000
#define KEY_TIME_HOLD5S                    5000
#define KEY_TIME_HOLD10S                   10000

#define TIMEOUT_ZW_GOACTIVE              120      /* ms */

#define TIME_S                           1000
#define TIME_L                           20000
#define TIMER_SHORT                      0
#define TIMER_LONG                       1

#define TIME_TRIES_REQ_ZWNETW            4
#define TIME_TRIES_CHECK_DOOR_STABLE     3

#define SENSOR_TEMP                      0x01
#define SENSOR_HUMI                      0x02
#define SENSOR_LUX                       0x04
#define SENSOR_PIR                       0x10
#define SENSOR_DOOR                      0x20

#define SENSOR_ALL_MASK                  (SENSOR_DOOR)   

#define TIME_INTERVAL_WAKEUP         0xFFFF
#define TIME_INTERVAL_REPORT_DOOR    3600
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
#define PWDOWN_TIMER_INTERVAL            100
#define PWTIMEOUT_100_MSEC               1
#define PWTIMEOUT_200_MSEC               2
#define PWTIMEOUT_500_MSEC               5
#define PWTIMEOUT_KEY_NOPRESS            20       /* 2s */ 
/******************************************************************************/
/*                               CONFIG KEYPAD                                */
/******************************************************************************/
#define KEYPAD_SCAN_INTERVAL             100      /* ms */
#define KEYPAD_IS_HOLD                   3        /* (*KEYPAD_SCAN_INTERVAL) ms */
#define KEYPAD_HOLD_8S                   78
#define KEYPAD_HOLD_5S                   48
#define KEYPAD_HOLD_2S                   18
#define TIME_SCAN_KEY                    100     /* ms */
#define TIME_KEY_IS_HOLD                 5       /* TIME_SCAN_KEY*TIME_KEY_IS_HOLD ms */
/******************************************************************************/
/*                               SELECT DEBUG                                 */
/******************************************************************************/
#define USE_DEBUG

#ifdef USE_DEBUG

#define MAIN_DEBUG
#define KEYCF_DEBUG

#define PWMANAGE_DEBUG
#define KEYPAD_DEBUG
#define DOOR_DEBUG
#define ZWNETW_DEBUG
//#define ZWSTATUS_DEBUG

#define ZWCMD_DEBUG
#define TXFAIL_DEBUG
//#define SERIAL_DEBUG

//#define TIMER_SYSTICK_DEBUG
//#define DEBUG_LPTMR
//#define DEBUG_LLWU
#endif /* USE_DEBUG */
#endif
/******************************************************************************/
/*                                 END FILE                                   */
/******************************************************************************/

