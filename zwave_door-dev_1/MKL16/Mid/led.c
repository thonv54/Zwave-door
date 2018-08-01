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
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "led.h"
#include "gpio.h"
#include "timer.h"
#include "sysregval.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define BLINK_R                       0x01
#define BLINK_B                       0x02
#define BLINK_A                       0xFF
#define TIME_LED_BLINK                100
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BYTE timerLedShow = NO_TIMER;
static BYTE typeLedBlink = 0;   
static BOOL g_bInitLed = FALSE;   
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void LedBlink(void_p pData);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   
 * @brief  
 * @param  None
 * @retval None
 */
void
LedInit(void) {
    g_bInitLed = TRUE;
    GPIO_SetPinIsOutput(LEDR);
    GPIO_SetPinIsOutput(LEDB);
    LedOffAll();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedOn(
    WORD LedPin
) {
    GPIO_PinHigh(LedPin);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedOff(
    WORD LedPin
) {
    GPIO_PinLow(LedPin);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedToggle(
    WORD LedPin
) {
    GPIO_PinToggle(LedPin);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedOnAll(void) {
    LedOn(LEDR);
    LedOn(LEDB);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedOffAll(void) {
    LedOff(LEDR);
    LedOff(LEDB);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedAllToggle(void) {
    LedToggle(LEDR);
    LedToggle(LEDB);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedDeInit(void) {
    if (!g_bInitLed) 
        return;

    LedOffAll();
    GPIO_DeInitPin(LEDR);
    GPIO_DeInitPin(LEDB);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedShow(
    BYTE typeIndicator
) {
    switch(typeIndicator) {
    case LEDKEY_PRESS:
    case LEDPIR_EVENT:
    case LEDDOOR_EVENT:
//        if (SysPara->byZwTxFalse > 0) 
//        {
//            LedOff(LEDB);
//            LedOn(LEDR);
//            delay_ms(100);
//            LedOff(LEDR);
//        }
//        else 
        {
            LedOff(LEDR);
            LedOn(LEDB);
            delay_ms(100);
            LedOff(LEDB);
        }
        break;

    case LEDZW_COMMFALSE:
        LedOff(LEDB);
        LedOn(LEDR);
        delay_ms(150);  
        LedOff(LEDR);
        delay_ms(150);
        LedOn(LEDR);
        delay_ms(150);  
        LedOff(LEDR);
        break;
    
    case LEDZW_TXFALSE:
        LedOff(LEDB);
        LedOn(LEDR);
        delay_ms(50);
        LedOff(LEDR);
        break;
     
    case LEDKEY_HOLD_2S:
        LedOn(LEDB);
        LedOff(LEDR);
        typeLedBlink = BLINK_B;

        if (timerLedShow == NO_TIMER) 
            TimerStart(TIME_LED_BLINK, 5, LedBlink, &typeLedBlink);
        else 
            TimerRestart(timerLedShow, TIME_LED_BLINK, 5);
        break;
     
    case LEDKEY_HOLD_5S:
        LedOn(LEDB); 
        LedOff(LEDR);
        typeLedBlink = 0;
        if (timerLedShow != NO_TIMER) {
            TimerCancel(timerLedShow); 
            timerLedShow = NO_TIMER;
        }
        break;  

    case LEDKEY_HOLD_8S:
        LedOff(LEDB);
        LedOn(LEDR);
        typeLedBlink = BLINK_R;

        if (timerLedShow == NO_TIMER) 
            TimerStart(TIME_LED_BLINK, TIMER_FOREVER, LedBlink, &typeLedBlink);
        else 
            TimerRestart(timerLedShow, TIME_LED_BLINK, TIMER_FOREVER); 
        break;
 
     case LEDKEY_RELEASE:
         LedOffAll();
         typeLedBlink = 0;
         if (timerLedShow != NO_TIMER) {
            TimerCancel(timerLedShow); 
            timerLedShow = NO_TIMER;
         }
         break;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedBlink(
    void_p pData
) {
    BYTE_p pLed = (BYTE_p) pData;
    BYTE bLed;
    if (pLed == NULL) return;
    bLed = *pLed;

    switch(bLed) {
    case BLINK_R:
        LedToggle(LEDR);
        break;
  
    case BLINK_B:
        LedToggle(LEDB);
        break;
      
    case BLINK_A:
        LedAllToggle();
        break;
    }
}
/* END FILE */
