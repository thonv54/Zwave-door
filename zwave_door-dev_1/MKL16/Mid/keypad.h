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
 * Last Changed By:  $Author: ThangDH $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date:  17/08/17 11:30 $
 *
 ******************************************************************************/
#ifndef _KEYPAD_H_
#define _KEYPAD_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
#include "configapp.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define TIMECNT_IS_HOLD                    (KEY_TIME_IS_HOLD/KEY_TIME_SCAN/KEY_SAMPLE_COUNT)
#define TIMECNT_BW2PRESS                   (KEY_TIMEOUT_BW2PRESS/KEY_TIME_SCAN/KEY_SAMPLE_COUNT)
#define TIMECNT_HOLD3S                     (KEY_TIME_HOLD3S/KEY_TIME_SCAN/KEY_SAMPLE_COUNT)
#define TIMECNT_HOLD5S                     (KEY_TIME_HOLD5S/KEY_TIME_SCAN/KEY_SAMPLE_COUNT)
#define TIMECNT_HOLD10S                    (KEY_TIME_HOLD10S/KEY_TIME_SCAN/KEY_SAMPLE_COUNT)

typedef enum {
    KEYEV_PRESS,
    KEYEV_HOLD,
    KEYEV_RELEASE,
    NUMBER_OF_KEY_EVENTS
} KEY_EVENT_T;
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
 * @func   KeyHandlerRegisterCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
KeyHandlerRegisterCallback(
    BYTE keyIndex,
    KEY_EVENT_T keyEvent,
    BYTE_CALLBACKFUNC procKeyEvent  
);

/**
 * @func   KeyTimeoutRegisterCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void 
KeyTimeoutRegisterCallback(
    WORD_CALLBACKFUNC pSetTimeoutFunc
);

/**
 * @func   KeyInit
 * @brief  None
 * @param  None
 * @retval None
 */
void
KeyInit(void);

#endif /* END FILE */
