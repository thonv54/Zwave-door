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
 *                   Need: Add timeout check zwave sleep
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "configboard.h"
#include "zwstatus.h"
#include "gpio.h"
#include "timer.h"

#include "debug.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef ZWSTATUS_DEBUG
#define DBG_IOHANDLE_SEND_STR(x)         Debug_SendStr(x)
#define DBG_IOHANDLE_SEND_NUM(x)         Debug_SendNum(x)
#else
#define DBG_IOHANDLE_SEND_STR(x)
#define DBG_IOHANDLE_SEND_NUM(x)
#endif
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
 * @func   InitPinZwState
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitPinZwState (void) {
    GPIO_SetPinIsInput(WAKEUP_MKL, PIN_PULL_UP);
}

/**
 * @func   WakeupZwave
 * @brief  None
 * @param  None
 * @retval None
 */
static void
WakeupZwave(void) {
    static BOOL bFlagInitIOWuZw = FALSE;

    if (!bFlagInitIOWuZw) {
        GPIO_SetPinIsOutput(WAKUP_ZWAVE);
        GPIO_PinHigh(WAKUP_ZWAVE);
 
        bFlagInitIOWuZw = TRUE;
    }
    else {
        GPIO_PinLow(WAKUP_ZWAVE);
        GPIO_PinHigh(WAKUP_ZWAVE);
    }

    DBG_IOHANDLE_SEND_STR("zw act\n");
}

/**
 * @func   CheckZwAwake
 * @brief  None
 * @param  None
 * @retval
 */
static BOOL
CheckZwAwake(void) {
    if (GPIO_PinReadInput(WAKEUP_MKL)) {
        DBG_IOHANDLE_SEND_STR("FAIL\n");
        return FALSE;
    }
    else {
        DBG_IOHANDLE_SEND_STR("OK\n");
        return TRUE;
    }
}

/**
 * @func   ActivateZwave
 * @brief  None
 * @param  None
 * @retval TRUE OR FALSE
 */
BOOL
ActivateZwave(
    WORD timeout
) {
    DWORD dwTime; 
    dwTime = GetMilSecTick() + timeout;
    WakeupZwave();
    while (!CheckZwAwake()) {
       WakeupZwave();
       if (dwTime < GetMilSecTick()) 
       {
           DBG_IOHANDLE_SEND_STR("zw act tmout\n");
           return FALSE;
       } 
    } 
  
    return TRUE;   
}

/* END FILE */
