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
#ifndef __RTC_H__
#define __RTC_H__ 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
#include "driver_common.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define RTC_IT_TIME_INVALID         1
#define RTC_IT_TIME_OVERFL          2
#define RTC_IT_TIME_ALARM           4
#define RTC_IT_TIME_SECOND          16
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
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
RTC_Init(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
RTC_SetIRQ(
    BYTE byITReg,
    FunctionalState NewState
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
RTC_SetTimeAlarm (
    DWORD dwTimeAlarm
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
DWORD
RTC_ReadTimeAlarm (void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
DWORD
RTC_GetTime(void);

#endif /* END FILE */
