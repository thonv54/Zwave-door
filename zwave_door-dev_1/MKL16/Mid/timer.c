/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  ThangDH $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 17/08/17 16:30 $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "driver_common.h"
#include "debug.h"
#include "timer.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef TIMER_SYSTICK_DEBUG
#define DBG_TIMER_SEND_STR(x)         Debug_SendStr(x)
#define DBG_TIMER_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_TIMER_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_TIMER_SEND_STR(x)
#define DBG_TIMER_SEND_NUM(x)
#define DBG_TIMER_SEND_HEX(x)
#endif

typedef struct _TIMER_ {
    DWORD           milSecStart;
    DWORD           milSecTimeout;
    BYTE            repeats;
    timer_callback  callbackFunc;
    void_p          pCallbackData;
} TIMER_t, *TIMER_p;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static TIMER_t g_pTimerHandle[MAX_TIMER] = { NULL };
static volatile DWORD g_wMilSecTickTimer = 0;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static BOOL TimeExpired(BYTE byTimerId);
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
TimerInit(void) {
    SysTick_Config(SystemCoreClock/1000);
    NVIC_SetPriority(SysTick_IRQn, IRQn_LevelHighest);
    BYTE_p px = (BYTE_p) g_pTimerHandle;
    for (WORD i = 0; i < sizeof(g_pTimerHandle); i++) {
        *px++ = 0;
    }
}

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
) {
    DBG_TIMER_SEND_STR("STAT timer\n");
   
    for (BYTE i = 0; i < MAX_TIMER; i++) 
    {
        if (g_pTimerHandle[i].callbackFunc == NULL)
        {
            g_pTimerHandle[i].callbackFunc = callback;
            g_pTimerHandle[i].repeats = byRepeats;
            g_pTimerHandle[i].pCallbackData = pcallbackData;
            g_pTimerHandle[i].milSecStart = GetMilSecTick();
            g_pTimerHandle[i].milSecTimeout = wMilSecTick;
            
            return i;
        }
    }
    
    return NO_TIMER;
}


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
) {
    DBG_TIMER_SEND_STR("RST timer\n");
    
    if ((byTimerId >= MAX_TIMER) || (g_pTimerHandle[byTimerId].callbackFunc == NULL))
        return FALSE;
        
    g_pTimerHandle[byTimerId].repeats = byRepeats;
    g_pTimerHandle[byTimerId].milSecTimeout = wMilSecTick;
    g_pTimerHandle[byTimerId].milSecStart = GetMilSecTick();
    
    return TRUE;
}

/**
 * @func   TimerCancel
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
TimerCancel(
    BYTE byTimerId
) {
    DBG_TIMER_SEND_STR("CAN timer\n");
    
    if ((byTimerId >= MAX_TIMER) || (g_pTimerHandle[byTimerId].callbackFunc == NULL))
        return FALSE;
    
    g_pTimerHandle[byTimerId].callbackFunc = NULL;
    g_pTimerHandle[byTimerId].repeats = 0;
    g_pTimerHandle[byTimerId].milSecTimeout = 0;
    g_pTimerHandle[byTimerId].milSecStart = 0;
    
    return TRUE;
}


/**
 * @func   GetMilSecTick
 * @brief  None
 * @param  None
 * @retval None
 */
DWORD
GetMilSecTick() {
    return g_wMilSecTickTimer;
}

/**
 * @func   processTimer
 * @brief  None
 * @param  None
 * @retval None
 */
void
processTimer() {
    timer_callback callbackfunc;
    void_p pPrameter;
    
    for (BYTE i = 0; i < MAX_TIMER; i++) 
    {
        if ((g_pTimerHandle[i].callbackFunc != NULL) && TimeExpired(i))
        {
            callbackfunc = g_pTimerHandle[i].callbackFunc;
            pPrameter = g_pTimerHandle[i].pCallbackData;
            
            if ((g_pTimerHandle[i].repeats != TIMER_FOREVER) && \
                (g_pTimerHandle[i].repeats != 0)) 
                 g_pTimerHandle[i].repeats--;
            
            callbackfunc(pPrameter);
            
            if (g_pTimerHandle[i].repeats == 0)
                 g_pTimerHandle[i].callbackFunc = NULL;
        }
    }
}

/**
 * @func   TimeExpired
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
TimeExpired(
    BYTE byTimerId              /* IN: Timer identifier */
) {
    DWORD wMilSecTick;
    DWORD wDeltaMilSec = 0;
    
    ENTER_CRITCAL;
    wMilSecTick = GetMilSecTick(); 
    if ((byTimerId >= MAX_TIMER) || (g_pTimerHandle[byTimerId].callbackFunc == NULL)) 
    {
        EXIT_CRITICAL;
        return FALSE;
    }
      
    if (wMilSecTick >= g_pTimerHandle[byTimerId].milSecStart) 
    {
        wDeltaMilSec = wMilSecTick - g_pTimerHandle[byTimerId].milSecStart;
    } 
    else 
    {
        wDeltaMilSec = g_pTimerHandle[byTimerId].milSecStart - wMilSecTick;
        wDeltaMilSec = MAX_U32 - wDeltaMilSec;
    }
    
    if (wDeltaMilSec < g_pTimerHandle[byTimerId].milSecTimeout)
    {
        EXIT_CRITICAL;
        return FALSE;
    }
    
    g_pTimerHandle[byTimerId].milSecStart = wMilSecTick;
    
    EXIT_CRITICAL;

    return TRUE;
}

/**
 * @func   SysTick_Handler
 * @brief  None
 * @param  None
 * @retval None
 */
void
SysTick_Handler() {
    g_wMilSecTickTimer++;
}

/* END FILE */
