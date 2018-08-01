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
#include "rtc.h"
#include "sim.h"
#include "sysregval.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

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
 * @func   RTC_Init
 * @brief  None
 * @param  None
 * @retval None
 */
void
RTC_Init(void) {
    CLOCK_SelectOSC32Clock(OSC32K_LPO_1KHZ);
    CLOCK_EnableRtcClock();
    /* Clear the software reset bit */
    RTC->CR  |= RTC_CR_SWR_MASK;
    RTC->CR  &= ~RTC_CR_SWR_MASK;  

    if (RTC->SR & RTC_SR_TIF_MASK)
    {
        RTC->TSR = 0;   /* This action clears the TIF */
    }

    RTC_SetIRQ(RTC_IT_TIME_SECOND, DISABLE);
    RTC_SetIRQ(RTC_IT_TIME_ALARM, ENABLE);
    RTC_SetIRQ(RTC_IT_TIME_OVERFL, DISABLE);

    RTC->SR |= RTC_SR_TCE_MASK;
//    NVIC_EnableIRQ(RTC_IRQn);
//    NVIC_EnableIRQ(RTC_Seconds_IRQn);
}

/**
 * @func   RTC_GetTime
 * @brief  None
 * @param  None
 * @retval 
 */
DWORD
RTC_GetTime() {
    return (RTC->TSR << 15) + RTC->TPR; 
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
RTC_Cmd(
    FunctionalState NewState
) {
    if (NewState == ENABLE) 
    {
        RTC->SR |= RTC_SR_TCE_MASK; 
    }
    else
    {
        RTC->SR &= ~RTC_SR_TCE_MASK; 
    }
}

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
) {
    if (NewState == ENABLE)
    {
        RTC->IER |= byITReg;
    }
    else if(NewState == DISABLE)
    {
        RTC->IER &= ~byITReg;
    }
}

/**
 * @func   RTC_GetITStatus
 * @brief  None
 * @param  None
 * @retval SET OR RESET
 */
ITStatus
RTC_GetITStatus(
    BYTE byITReg
) {
    if (byITReg == RTC_IT_TIME_SECOND) 
    {

    }
    else if (RTC->SR & byITReg) 
    {
        return SET;
    }

    return RESET;
}


/**
 * @func   RTC_IRQHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void
RTC_IRQHandler() {
    if (RTC_GetITStatus(RTC_IT_TIME_ALARM) == SET)
    {

    }
    else if (RTC_GetITStatus(RTC_IT_TIME_OVERFL) == SET)
    {

    }
    else if (RTC_GetITStatus(RTC_IT_TIME_INVALID) == SET)
    {

    } 
}

/**
 * @func   RTC_SetTimeAlarm
 * @brief  None
 * @param  None
 * @retval None
 */
void
RTC_SetTimeAlarm (
    DWORD dwTimeAlarm
) {
    RTC->TAR = dwTimeAlarm;    
}

/**
 * @func   RTC_ReadTimeAlarm
 * @brief  None
 * @param  None
 * @retval None
 */
DWORD
RTC_ReadTimeAlarm (void) {
    return (RTC->TAR);
}


/**
 * @func   RTC_Seconds_IRQHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void
RTC_Seconds_IRQHandler() {

}

/* END FILE */
