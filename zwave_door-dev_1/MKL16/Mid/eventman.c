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
 * Last Changed:     $Date:  29/08/17   08:30 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "eventman.h"
#include "buff.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define SIZE_EVENT_QUEUE              8
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
app_state_callback pAppStateFunc = NULL;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
static BYTE pBuffEvent[SIZE_EVENT_QUEUE];
static buffqueue eventQueue; 
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   EventSchedulerInit 
 * @brief  None
 * @param  None
 * @retval None
 */
void
EventSchedulerInit(
    app_state_callback func    
) {
    if (func != NULL) {
        pAppStateFunc = func;
        bufInit(pBuffEvent, &eventQueue, sizeof(pBuffEvent[0]), SIZE_EVENT_QUEUE);
    }
}

/**
 * @func   EventSchedulerAdd 
 * @brief  Add event to queue
 * @param  None
 * @retval None
 */
void
EventSchedulerAdd(
    BYTE event
) {
    if (bufEnDat(&eventQueue, &event) == ERR_OK) {
    }
}

/**
 * @func   EventScheduler 
 * @brief  Proccess event in queue
 * @param  None
 * @retval None
 */
void
EventScheduler(void) {
    if (pAppStateFunc != NULL) {
        BYTE event;
        if (bufDeDat(&eventQueue, &event) == ERR_OK) {
            pAppStateFunc(event);
        }
    }
}

/* END FILE */
