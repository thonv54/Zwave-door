/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: TrungTQ
 *
 * Last Changed By:  ThangDH $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 17/08/17 16:30 $
 *
 ******************************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define MAX_TIMER                   16
#define TIMER_ONE_TIME              0
#define TIMER_FOREVER               0xFF
#define NO_TIMER                    0xFF

typedef void (*timer_callback)(void_p);
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   TimerInit
 * @brief  None
 * @param  None
 * @retval None
 */
void
TimerInit(void);

/**
 * @func   TimerStart
 * @brief  None
 * @param  None
 * @retval None
 */

BYTE                            /* OUT: index of timer */
TimerStart(
    DWORD wMilSecTick,          /* IN: timer tick */
    BYTE byRepeats,             /* IN: number of repeater */
    timer_callback callback,    /* IN: callback function*/
    void_p pcallbackData        /* IN: parameters */
);

/**
 * @func   TimerRestart
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
TimerRestart(
    BYTE byTimerId,
    DWORD wMilSecTick,
    BYTE byRepeats
);
    
/**
 * @func   TimerCancel
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
TimerCancel(
    BYTE byTimerId
);

/**
 * @func   GetMilSecTick
 * @brief  None
 * @param  None
 * @retval None
 */
DWORD
GetMilSecTick(void);

/**
 * @func   processTimer
 * @brief  None
 * @param  None
 * @retval None
 */
void
processTimer(void);

#endif /* _TIMER_H_ */
