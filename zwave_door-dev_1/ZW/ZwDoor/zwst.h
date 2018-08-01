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
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
#ifndef _ZW_STATE_
#define _ZW_STATE_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ZW_typedefs.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define PIN_STATE                  0x10
#define PIN_LED                    0x36
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
 * @func   InitPinWakeupMKL
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitPinWakeupMKL(void);

/**
 * @func   ZwReportGoAwake
 * @brief  None
 * @param  None
 * @retval None
 */
void
ZwReportGoAwake(void);

/**
 * @func   ZwReportGosleep
 * @brief  None
 * @param  None
 * @retval None
 */
void
ZwReportGosleep(void); 

/**
 * @func   WakeupMKL
 * @brief  None
 * @param  None
 * @retval None
 */
void
WakeupMKL(void);

void InitLed(void);
void LedOn(void);
void LedOff(void);
void LedBlink(void);

#endif /* END FILE */

