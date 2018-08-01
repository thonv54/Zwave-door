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
#ifndef __LPTMR_H__
#define __LPTMR_H__ 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define LPTMR_USE_IRCLK       0 
#define LPTMR_USE_LPOCLK      1
#define LPTMR_USE_ERCLK32     2
#define LPTMR_USE_OSCERCLK    3 
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
 * @func   CLOCK_EnableLptmrClock
 * @brief  None
 * @param  None
 * @retval
 */
void
CLOCK_EnableLptmrClock(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
LPTMR_Init(
    DWORD count,
    DWORD clock_source
);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
void
LPTMR_delayMs(
    DWORD count_val
);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
void
LPTMR_Reset(void);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
void
LPTMR_SetIrq (
    FunctionalState NewState
);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
void
LPTMR_ClearITPendingBit(void);

/**
 * @func
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
void
LPTMR_Cmd(
    FunctionalState NewState
);
#endif /* __LPTMR_H__ */
