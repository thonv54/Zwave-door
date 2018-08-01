/*******************************************************************************
 *
 * Copyright (c) 2017
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Queue event 
 *
 * Author: ThangDH
 *
 * Last Changed By:  $Author: thangdh $
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date:  29/08/17   08:30 $
 *
 ******************************************************************************/
#ifndef _EVENT_MAN_H_
#define _EVENT_MAN_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef void (*app_state_callback)(BYTE);
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
 * @func   EventSchedulerInit 
 * @brief  None
 * @param  None
 * @retval None
 */
void
EventSchedulerInit(
    app_state_callback func    
);

/**
 * @func   EventSchedulerAdd 
 * @brief  Add event to queue
 * @param  None
 * @retval None
 */
void
EventSchedulerAdd(
    BYTE event
);

/**
 * @func   EventScheduler 
 * @brief  Proccess event in queue
 * @param  None
 * @retval None
 */
void
EventScheduler(void);

#endif /* END FILE */
