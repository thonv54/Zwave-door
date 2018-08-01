/*******************************************************************************
 *
 * Copyright (c) 2017
 * Lumi, JSC.
 * All Rights Resered
 *
 *
 * Description:
 *
 * Author: ThangDH 
 *
 * Last Changed By:  $Author: ThangDH $
 * Revision:         $Revision:  $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
#ifndef _DEVICE_SETUP_
#define _DEVICE_SETUP_ 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "ZW_typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef struct _device_setup_ {
    BYTE modesel;
    WORD timeRpSens;
} paraSetup_t, *paraSetup_p;

#define PARAM_MODE_SELECT            0      
#define PARAM_TIME_WAKEUP            1      
#define PARAM_TIME_REPORT_SENS       2
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
 * @func   LoadParamSetup
 * @brief  None
 * @param  None
 * @retval None
 */
void
LoadParamSetup(void);

/**
 * @func   GetParamSetup
 * @brief  None
 * @param  None
 * @retval None
 */
void
GetParamSetup(
    BYTE param,
    void* val
);

/**
 * @func   GetParamSetup
 * @brief  None
 * @param  None
 * @retval None
 */
void
SetParamSetup(
    BYTE param,
    void* val
);

#endif 

