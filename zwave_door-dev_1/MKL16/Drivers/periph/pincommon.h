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
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
#ifndef _PIN_COMMON_H_
#define _PIN_COMMON_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "pinmap.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define PIN_FLOATING                0
#define PIN_PULL_UP                 1
#define PIN_PULL_DOWN               2
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
 * @func   pin_func  
 * @brief  None
 * @param  None
 * @retval None
 */
void
pin_func(
    BYTE pin,
    BYTE port,
    BYTE altFunc
);

/**
 * @func   pin_mode  
 * @brief  None
 * @param  None
 * @retval None
 */
void
pin_mode(
    BYTE pin,
    BYTE port,
    BYTE mode
);

/**
 * @func   pinmap_peripheral  
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL
pinmap_peripheral(
    WORD IoPin,
    const PinMap* map
);

#endif /* END FILE */
