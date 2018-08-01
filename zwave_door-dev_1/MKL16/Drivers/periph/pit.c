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
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "pit.h"
#include "gpio.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef struct _TIMER_ {
    BYTE            repeat;
    task_callback   callbackFunc;
    void_p          pCallbackData;
} TASK_t, *TASK_p;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BOOL flgInitPIT = FALSE;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static TASK_t gPTaskHandle[PIT_COUNT];
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

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
) {   
    if (!flgInitPIT) {
        PIT_Init(TRUE); 
        flgInitPIT = TRUE;
    }
                    
    for (BYTE i = 0; i < PIT_COUNT; i++) {
         if (gPTaskHandle[i].callbackFunc == NULL) {
             gPTaskHandle[i].callbackFunc = pCallbackFunc;
             gPTaskHandle[i].pCallbackData = pCallBackData;
             gPTaskHandle[i].repeat = byRepeats;

             PIT_SetPeroidMs(i, intervalCallMs);
             PIT_ITConfig(i, ENABLE);
             PIT_cmd(i, ENABLE);

             return i;
         }
    }  

    return NO_TASK;      
}

/**
 * @func   TaskCancer
 * @brief  Cancer task
 * @param  None
 * @retval None
 */
void
TaskCancer(
    BYTE idTask
) {
    PIT_ITConfig(idTask, DISABLE);
    PIT_cmd(idTask, DISABLE);

    gPTaskHandle[idTask].callbackFunc = NULL;
    gPTaskHandle[idTask].pCallbackData = NULL;
    gPTaskHandle[idTask].repeat = 0;
}

/**
 * @func   TaskProcess
 * @brief  None
 * @param  None
 * @retval None
 */
void 
TaskProcess(
    BYTE idTask
) {
    task_callback pFunc;
    void_p pData;

    if (gPTaskHandle[idTask].callbackFunc != NULL) {
        pFunc = gPTaskHandle[idTask].callbackFunc;
        pData = gPTaskHandle[idTask].pCallbackData;

        if ((gPTaskHandle[idTask].repeat != TASK_FOREVER) && \
            (gPTaskHandle[idTask].repeat != 0))
        {
            gPTaskHandle[idTask].repeat--;
        }

        pFunc(pData);
     
        if (gPTaskHandle[idTask].repeat == 0) {
            TaskCancer(idTask);
        }
    }
}

/**
 * @func   PIT_Init
 * @brief  None
 * @param  None
 * @retval None
 */
void
PIT_Init(
    BOOL isRunInDebug
) {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;
    if (isRunInDebug) 
        PIT->MCR &= ~PIT_MCR_FRZ_MASK;
    else 
        PIT->MCR |= PIT_MCR_FRZ_MASK;

    NVIC_EnableIRQ(PIT_IRQn);
    NVIC_SetPriority(PIT_IRQn, IRQn_LevelHigh);
}

/**
 * @func   PIT_ITConfig
 * @brief  None
 * @param  None
 * @retval None
 */
void
PIT_ITConfig(
    BYTE byPITchanel,
    FunctionalState NewState 
) {
    if (NewState == ENABLE) 
    { 
        PIT->CHANNEL[byPITchanel].TCTRL |= PIT_TCTRL_TIE_MASK;
    }
    else if (NewState == DISABLE) 
    {
        PIT->CHANNEL[byPITchanel].TCTRL &= ~PIT_TCTRL_TIE_MASK;
    }
}

/**
 * @func   PIT_cmd
 * @brief  None
 * @param  None
 * @retval None
 */
void
PIT_cmd(
    BYTE byPITchanel,
    FunctionalState NewState   
) {
    if (NewState == ENABLE) 
    { 
        PIT->CHANNEL[byPITchanel].TCTRL |= PIT_TCTRL_TEN_MASK;
    }
    else if (NewState == DISABLE) 
    {
        PIT->CHANNEL[byPITchanel].TCTRL &= ~PIT_TCTRL_TEN_MASK;
    }
}

/**
 * @func   PIT_IsEnable
 * @brief  None
 * @param  None
 * @retval TRUE if PIT chanel Enable
 */
BOOL
PIT_IsEnable(
    BYTE byPITchanel
) {
    if ((PIT->CHANNEL[byPITchanel].TCTRL & PIT_TCTRL_TEN_MASK) \
         == PIT_TCTRL_TEN_MASK)
    {
        return TRUE;
    }

    return FALSE;
}

/**
 * @func   PIT_GetITStatus
 * @brief  None
 * @param  None
 * @retval None
 */
ITStatus
PIT_GetITStatus(
    BYTE byPITchanel
) {
    if (PIT->CHANNEL[byPITchanel].TFLG & PIT_TFLG_TIF_MASK) 
        return SET;
    else 
        return RESET;    
}

/**
 * @func   PIT_ClearITPendingBit
 * @brief  None
 * @param  None
 * @retval None
 */
void
PIT_ClearITPendingBit(
    BYTE byPITchanel   
) {
    PIT->CHANNEL[byPITchanel].TFLG |= PIT_TFLG_TIF_MASK;
}

/**
 * @func   PIT_SetPeroidMs
 * @brief  None
 * @param  None
 * @retval None
 */ 
void
PIT_SetPeroidMs(
    BYTE byPITchanel,
    DWORD dwPeroidMs
) {
    DWORD count = 0;

    count =  (SystemBusClock * dwPeroidMs)/1000 - 1;
    PIT_SetPeroidCount(byPITchanel, count);
}

/**
 * @func   PIT_SetPeroidCount
 * @brief  None
 * @param  None
 * @retval None
 */ 
void
PIT_SetPeroidCount(
    BYTE byPITchanel,
    DWORD dwCount
) {
    BOOL flg = PIT_IsEnable(byPITchanel);

    if (flg) {
        PIT_cmd(byPITchanel, DISABLE);
    }

    PIT->CHANNEL[byPITchanel].LDVAL = dwCount;
    
    if (flg)
        PIT_cmd(byPITchanel, ENABLE);
}

/**
 * @func   PIT_GetPeriodByCount
 * @brief  None
 * @param  None
 * @retval 
 */ 
DWORD
PIT_GetPeriodByCount(
    BYTE byPITchanel   
) {
    return PIT->CHANNEL[byPITchanel].CVAL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
PIT_IRQHandler(void) {
    if (PIT_GetITStatus(PIT0_IDX)) {
        TaskProcess(PIT0_IDX);
        PIT_ClearITPendingBit(PIT0_IDX); 
    }

    if (PIT_GetITStatus(PIT1_IDX)) {
        TaskProcess(PIT1_IDX);
        PIT_ClearITPendingBit(PIT1_IDX); 
    }
}
/* END FILE */
