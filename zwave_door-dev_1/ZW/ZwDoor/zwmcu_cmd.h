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
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
#ifndef  _COMMCATION_ZWMCU_H_
#define  _COMMCATION_ZWMCU_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ZW_typedefs.h>
#include "serial.h"
#include "ev_man.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef void (CODE *APP_STATE_FUNC)(BYTE);

typedef enum _EVENT_BUTTON_ {
    EVENT_BUTTON_RELEASE = DEFINE_EVENT_KEY_NBR,
    EVENT_BUTTON_PRESS,
    EVENT_BUTTON_HOLD,
    EVENT_BUTTON_RESET,
	EVENT_BUTTON_KEEP_AWAKE,
    EVENT_BUTTON_ADDNODE   
} EVENT_BUTTON;

#define EVENT_SENSOR_REPORT         DEFINE_EVENT_SENS  
#define TIMER_SENS_REPORT_VAL       2
#define NUM_TRIES_REPORT            1
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
 * @func   InitProcessManager   
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitProcessManager(
    APP_STATE_FUNC procStateApp
);

/**
 * @func   InitCmmZw   
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitCmmZw (void);

void
UpdZwNetw(
    BYTE netwStatus
);

void
CmdZwNetwReportStatus(void);

void
CmdZwTxFailReport(
    WORD numbsfail
);

void
CmdZwTimeConfigSet(
    BYTE typeTime,
    WORD value
);

#endif /* END FILE */
