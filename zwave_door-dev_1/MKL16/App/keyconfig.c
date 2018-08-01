/*******************************************************************************
 *
 * Copyright (c) 2017
 * Lumi, JSC.
 * All Rights Resered
 *
 *
 * Description:
 *
 * Author: ThangDH 
 *
 * Last Changed By:  $Author: ThangDH $
 * Revision:         $Revision:  $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "keyconfig.h"
#include "keypad.h"
#include "debug.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef KEYCF_DEBUG
#define DBG_KEYCFSEND_STR(x)         Debug_SendStr(x)
#define DBG_KEYCFSEND_NUM(x)         Debug_SendNum(x)
#define DBG_KEYCFSEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_KEYCFSEND_STR(x)
#define DBG_KEYCFSEND_NUM(x)
#define DBG_KEYCFSEND_HEX(x)
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
void BTProcEventPress(BYTE time);
void BTProcEventHold(BYTE time);
void BTProcEventRelease(BYTE time);
static BYTE_CALLBACKFUNC pAppProccessEvent;
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   InitKeyConfig
 * @brief  None
 * @param  None
 * @retval None
 */
void 
InitKeyConfig(
    BYTE_CALLBACKFUNC pFuncAppProcEvent,
    WORD_CALLBACKFUNC pSetTimeoutFunc
) {
    KeyInit();
    KeyHandlerRegisterCallback(0, KEYEV_PRESS, BTProcEventPress);  
    KeyHandlerRegisterCallback(0, KEYEV_HOLD, BTProcEventHold); 
    KeyHandlerRegisterCallback(0, KEYEV_RELEASE, BTProcEventRelease);  
    KeyTimeoutRegisterCallback(pSetTimeoutFunc);
    pAppProccessEvent = pFuncAppProcEvent;
}

/**
 * @func   BTProcEventPress
 * @brief  None
 * @param  None
 * @retval None
 */
void
BTProcEventPress(
    BYTE time
) {
    DBG_KEYCFSEND_STR("$ Butt press event ");
    DBG_KEYCFSEND_NUM(time);
    DBG_KEYCFSEND_STR("\n");

    if (pAppProccessEvent == NULL) return;

    switch (time) {
    case 0:
    case 1: pAppProccessEvent(EVENT_KEY_1PRESS); break;
    case 3: pAppProccessEvent(EVENT_KEY_3PRESS); break;  
    }   
}

/**
 * @func   BTProcEventHold
 * @brief  None
 * @param  None
 * @retval None
 */
void
BTProcEventHold(
    BYTE time
) {
    DBG_KEYCFSEND_STR("$ Butt hold event\n");
    if (pAppProccessEvent == NULL) return;

    switch(time) {
    case TIMECNT_IS_HOLD: break;      
    case TIMECNT_HOLD3S: pAppProccessEvent(EVENT_KEY_HOLD2S); break;
    case TIMECNT_HOLD5S: pAppProccessEvent(EVENT_KEY_HOLD5S); break;
    case TIMECNT_HOLD10S: pAppProccessEvent(EVENT_KEY_HOLD10S); break;
    }        
}

/**
 * @func   BTProcEventRelease
 * @brief  None
 * @param  None
 * @retval None
 */
void
BTProcEventRelease(
    BYTE time
) {
    DBG_KEYCFSEND_STR("$ Butt release event\n");
    if (pAppProccessEvent == NULL) return;

    if (time >= TIMECNT_HOLD10S) {
        pAppProccessEvent(EVENT_KEY_RHOLD10S);
    }
    else if (time >= TIMECNT_HOLD5S) {
        pAppProccessEvent(EVENT_KEY_RHOLD5S);
    }
    else if (time >= TIMECNT_HOLD3S) {
        pAppProccessEvent(EVENT_KEY_RHOLD2S);
    }       
}

/* END FILE */
