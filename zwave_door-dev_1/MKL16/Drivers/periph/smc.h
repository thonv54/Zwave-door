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
#ifndef _SMC_H_
#define _SMC_H_ 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "driver_common.h"
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define PW_MODE_RUN                 1
#define PW_MODE_STOP                2
#define PW_MODE_VLPR                4
#define PW_MODE_VLPW                8
#define PW_MODE_VLPS                16
#define PW_MODE_LLS                 32
#define PW_MODE_VLLS                64
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
BYTE
SMC_GetMode(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void 
SMC_ShowMode(
    BYTE mode
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
SMC_EnterModeVLLS1(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
SMC_EnterModeVLPS(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
BYTE 
SMC_EnterModeVLPR(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void 
SMC_ExitModeVLPR(void);

#endif /* __SMC_H__ */
