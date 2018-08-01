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
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
#ifndef _KEY_CONFIG_H_
#define _KEY_CONFIG_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "configapp.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef enum {
    EVENT_KEY_1PRESS = DEFINE_EVENT_KEY_NBR,
    EVENT_KEY_3PRESS,
    EVENT_KEY_HOLD2S,
    EVENT_KEY_RHOLD2S,
    EVENT_KEY_HOLD3S,
    EVENT_KEY_RHOLD3S,
    EVENT_KEY_HOLD5S,
    EVENT_KEY_RHOLD5S,
    EVENT_KEY_HOLD10S,
    EVENT_KEY_RHOLD10S,
} EVENT_KEY_CONFIG;
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
 * @func   InitKeyConfig
 * @brief  None
 * @param  None
 * @retval None
 */
void 
InitKeyConfig(
    BYTE_CALLBACKFUNC pFuncAppProcEvent,
    WORD_CALLBACKFUNC pSetTimeoutFunc
);
 
#endif /* END FILE */
