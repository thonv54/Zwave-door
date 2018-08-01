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
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configboard.h"
#include "keypad.h"
#include "gpio.h"
#include "pit.h"
#include "debug.h"
#include "utilities.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef KEYPAD_DEBUG
#define DBG_KEYPAD_SEND_STR(x)         Debug_SendStr(x)
#define DBG_KEYPAD_SEND_NUM(x)         Debug_SendNum(x)
#else
#define DBG_KEYPAD_SEND_STR(x)
#define DBG_KEYPAD_SEND_NUM(x)
#endif

typedef struct _key_data_ {
    BYTE TimeH;
    BYTE TimeL;
    BYTE Time;
    BYTE cnt;
    BYTE event  :4;
    BYTE status :2;
    BYTE sold   :1;
} keydat;

typedef struct _key_pin_ {
    WORD pin;
    BYTE logicPress;
} keypin;

typedef enum _key_status_ {
    KEYST_IDLE = 0,
    KEYST_INPROCESS,
    KEYST_FINISH
} keyStatus;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BYTE idTaskScankey = NO_TASK;
static keypin pinKeyPad[KEY_MAX] = {KEY_CONFIG};
static keydat kpad[KEY_MAX];
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void KeyScan(void_p pData);
static BYTE_CALLBACKFUNC pEventCallbacks[KEY_MAX][NUMBER_OF_KEY_EVENTS];
static WORD_CALLBACKFUNC pSetTimeoutCallback = NULL;
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   KeyInit
 * @brief  None
 * @param  None
 * @retval None
 */
void
KeyInit(void) {
    for (BYTE i = 0; i < KEY_MAX; i++) {
        GPIO_SetPinIsInput(pinKeyPad[i].pin, PIN_PULL_UP);
    }

    idTaskScankey = TaskStart(KEY_TIME_SCAN, TASK_FOREVER, KeyScan, NULL);
    if (idTaskScankey == NO_TASK) {
        DBG_KEYPAD_SEND_STR("$ Keypad error\n");
    }
    else {
        DBG_KEYPAD_SEND_STR("$ Keypad start! ID = ");
        DBG_KEYPAD_SEND_NUM(idTaskScankey);
        DBG_KEYPAD_SEND_STR("\n");
    }
}

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
) {
    pEventCallbacks[keyIndex][keyEvent] = procKeyEvent;
}

/**
 * @func   KeyTimeoutRegisterCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void 
KeyTimeoutRegisterCallback(
    WORD_CALLBACKFUNC pSetTimeoutFunc
) {
    pSetTimeoutCallback = pSetTimeoutFunc;
}

/**
 * @func   KeyPWTimeoutKick
 * @brief  None
 * @param  None
 * @retval None
 */
void 
KeyPWTimeoutKick() {
    if (pSetTimeoutCallback != NULL) {
        pSetTimeoutCallback(PWTIMEOUT_KEY_NOPRESS);
    }
}

 /**
 * @func   KeyScan
 * @brief  None
 * @param  None
 * @retval None
 */
void
KeyScan(
    void_p pData
) {
    static BYTE sampleCount = 0;
    BYTE i;
    BOOL flgCheck;
    keydat *bt;

//    DBG_KEYPAD_SEND_STR("Scan Key\n");
    sampleCount++;
    if (sampleCount == KEY_SAMPLE_COUNT) {
        flgCheck = TRUE;
        sampleCount = 0;
    }
    else {
        flgCheck = FALSE;
    }

    for (i = 0; i < KEY_MAX; i++) {
        bt = &kpad[i];
        if (GPIO_PinReadInput(pinKeyPad[i].pin) == pinKeyPad[i].logicPress) {
            bt->cnt++;
            KeyPWTimeoutKick();
        }

        if (flgCheck) {
            if (bt->cnt == KEY_COUNT_IS_PRESS)
            {
                bt->status = KEYST_INPROCESS;

                if (bt->TimeL != MAX_U8) {
                    bt->TimeL++;
                }

                if (bt->TimeL >= TIMECNT_IS_HOLD) {
                    bt->Time = bt->TimeL;
                    bt->event = KEYEV_HOLD;

                    if ((bt->Time == TIMECNT_IS_HOLD) || (bt->Time == TIMECNT_HOLD3S) ||
                        (bt->Time == TIMECNT_HOLD5S) || (bt->Time == TIMECNT_HOLD10S)) 
                    {
                        if (pEventCallbacks[i][KEYEV_HOLD] != NULL) {
                            pEventCallbacks[i][KEYEV_HOLD](bt->Time);
                        }
                    }
                }
                else {
                    if (bt->sold) {
                        bt->Time++;
                        bt->event = KEYEV_PRESS;
                    }
                }

                bt->sold = 0;
                bt->TimeH = 0;
            } 
            else {
                if (bt->TimeH != MAX_U8) {
                    bt->TimeH++;
                }
           
                if (bt->status == KEYST_INPROCESS) {
                    if (bt->event == KEYEV_HOLD) {
                        bt->event = KEYEV_RELEASE;
                        bt->status = KEYST_FINISH;
                    }
                    else if (bt->TimeH >= TIMECNT_BW2PRESS) {
                        bt->status = KEYST_FINISH;    
                    } 
                }
        
                bt->sold = 1;
                bt->TimeL = 0;
            } 

            if (flgCheck) {
                bt->cnt = 0;
            }

            if (bt->status == KEYST_FINISH) {
                if (bt->event == KEYEV_PRESS) {
                    if (pEventCallbacks[i][KEYEV_PRESS] != NULL) {
                        pEventCallbacks[i][KEYEV_PRESS](bt->Time);
                    }
                }
                else if (bt->event == KEYEV_RELEASE) {
                    if (pEventCallbacks[i][KEYEV_RELEASE] != NULL) {
                        pEventCallbacks[i][KEYEV_RELEASE](bt->Time);
                    }
                }

                memsetl((BYTE_p)bt, 0, sizeof(keydat)); /* Clear keydat */
                bt->status = KEYST_IDLE;
            }
        }
    }
}

/* END FILE */
