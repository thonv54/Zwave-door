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
#ifndef __PMC_H__
#define __PMC_H__ 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "driver_common.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define LVD_1V74                     0
#define LVD_1V84                     1
#define LVD_1V94                     2
#define LVD_2V04                     3
#define LVD_2V62                     4
#define LVD_2V72                     5
#define LVD_2V82                     6
#define LVD_2V92                     7

#define LVD_FLAG                     1
#define LVW_FLAG                     2
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
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval
 */
void
PMC_InitLVD (
    BYTE volSelect
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
PMC_SetIrqLVD(
    BOOL state
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
PMC_SetIrqLVW(
    BOOL state
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
ITStatus
PMC_GetITStatus(
    BYTE byFlag
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
BOOL
PMC_IsIsolatedState(void);

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
PMC_ExitIsolatedState(void);

#endif /* __PMC_H__ */
