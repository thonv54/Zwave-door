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
#ifndef _PIT_H_
#define _PIT_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "driver_common.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define MAX_TASK                    2
#define TASK_ONE_TIME               0
#define TASK_FOREVER                0xFF
#define NO_TASK                     0xFF

typedef void (*task_callback)(void_p);
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
PIT_Init(
    BOOL isRunInDebug
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
PIT_ITConfig(
    BYTE byPITchanel,
    FunctionalState NewState 
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
PIT_cmd(
    BYTE byPITchanel,
    FunctionalState NewState   
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
ITStatus
PIT_GetITStatus(
    BYTE byPITchanel
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
PIT_ClearITPendingBit(
    BYTE byPITchanel   
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */ 
void
PIT_SetPeroidMs(
    BYTE byPITchanel,
    DWORD dwPeroidMs
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */ 
void
PIT_SetPeroidCount(
    BYTE byPITchanel,
    DWORD dwCount
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */ 
DWORD
PIT_GetPeriodByCount(
    BYTE byPITchanel   
);

/**
 * @func   TaskStart
 * @brief  None
 * @param  None
 * @retval Task_ID OR NO_TASK
 */
BYTE
TaskStart(
    BYTE intervalCallMs,
    BYTE byRepeats,
    task_callback pCallbackFunc,
    void_p pCallBackData   
);

/**
 * @func   TaskCancer
 * @brief  Cancer task
 * @param  None
 * @retval None
 */
void
TaskCancer(
    BYTE idTask
);

#endif /* END FILE */
