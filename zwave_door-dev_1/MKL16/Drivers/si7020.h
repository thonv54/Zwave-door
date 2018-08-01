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
#ifndef _SI7020_H_
#define _SI7020_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
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
 * @func   Si7020Init
 * @brief  None
 * @param  None
 * @retval None
 */
void
Si7020Init(void);

/**
 * @func   Si7020GetVerFW
 * @brief  None
 * @param  None
 * @retval
 */
void
Si7020GetVerFW(void);

/**
 * @func   Si7020GetSerialNum
 * @brief  None
 * @param  None
 * @retval None
 */
void
Si7020GetSerialNum(
    BYTE_p pbySerial
);

/**
 * @func   Si7020GetHumi
 * @brief  None
 * @param  None
 * @retval
 */
DWORD
Si7020GetHumi(void);

/**
 * @func   Si7020GetTemp
 * @brief  None
 * @param  None
 * @retval
 */
DWORD
Si7020GetTemp(void);

/**
 * @func   Si7020GetTempPre
 * @brief  None
 * @param  None
 * @retval 
 */
DWORD 
Si7020GetTempPre(void);

#endif /* END FILE */
