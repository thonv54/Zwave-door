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
#ifndef __LED_H__
#define __LED_H__
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configboard.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define LEDKEY_PRESS            1
#define LEDKEY_HOLD             2
#define LEDKEY_HOLD_2S          3
#define LEDKEY_HOLD_5S          4
#define LEDKEY_HOLD_8S          5
#define LEDKEY_RELEASE          6
#define LEDZW_TXFALSE           7
#define LEDDOOR_EVENT           8
#define LEDPIR_EVENT            9
#define LEDZW_COMMFALSE         10
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
 * @func   LedInit
 * @brief  
 * @param  None
 * @retval None
 */
void
LedInit(void);

/**
 * @func   LedInit
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedOn(
    WORD LedPin
);

/**
 * @func
 * @brief  None
 * @param  
 * @retval None
 */
void
LedOff(
    WORD LedPin
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedToggle(
    WORD LedPin
);


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedOnAll(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedOffAll(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedAllToggle(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedDeInit(void);

/**
 * @func   LedShow
 * @brief  None
 * @param  None
 * @retval None
 */
void
LedShow(
    BYTE typeIndicator
);

#endif /* _LED_H_ */
