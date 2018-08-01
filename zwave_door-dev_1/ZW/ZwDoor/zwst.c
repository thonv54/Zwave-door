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
 * Last Changed By:  $Author: thangdh $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "zwst.h"
#include <gpio_driver.h>
#include <ZW_timer_api.h>
#include <misc.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define TIMER_200_MSEC               20
#define ON                            1 
#define OFF                           0
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BOOL BIT = FALSE;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void BlinkLed_p(void);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

void
InitPinWakeupMKL(void) {
    gpio_SetPinOut(PIN_STATE);
}

void
ZwReportGoAwake(void) {
    gpio_SetPin(PIN_STATE, OFF);
}

void
ZwReportGosleep(void) {
    gpio_SetPin(PIN_STATE, ON);
}

void
WakeupMKL(void) {
    /* Wakeup by edge falling */
    gpio_SetPin(PIN_STATE, ON);
    gpio_SetPin(PIN_STATE, OFF);
}

void
InitLed(void) {
    gpio_SetPinOut(PIN_LED); 
    LedOff();
}

void
LedOn(void) {
    gpio_SetPin(PIN_LED, OFF);
}

void
LedOff(void) {
    gpio_SetPin(PIN_LED, ON);
}

void 
LedBlink(void) {
    TimerStart(BlinkLed_p, TIMER_200_MSEC, TIMER_FOREVER);
}

PCB(BlinkLed_p)(void) {
    BIT =~BIT;
    if (BIT) gpio_SetPin(PIN_LED, OFF);
    else gpio_SetPin(PIN_LED, ON);
}

/* END FILE */
 
