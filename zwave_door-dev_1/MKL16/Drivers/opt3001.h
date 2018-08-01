/****************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: phuclm
 *
 * Last Changed By:  thangdh 
 * Revision:         Revision: 1.0.0.1 
 * Last Changed:     Date: Jun 16, 2016 10:34:31 AM 
 *
 ****************************************************************************/
#ifndef _OPT3001_H_
#define _OPT3001_H_
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include "typedefs.h"
#include "macro.h"
/****************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                       */
/****************************************************************************/

/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/

/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/

/****************************************************************************/
/*                            PRIVATE FUNCTIONS                             */
/****************************************************************************/

/****************************************************************************/
/*                            EXPORTED FUNCTIONS                            */
/****************************************************************************/

/**
 * @func   Opt3001Init
 * @brief  None
 * @param  None
 * @retval None
 */
void
Opt3001Init(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
Opt3001Configure(void);

/**
 * @func   Opt3001GetManufactorID
 * @brief  None
 * @param  None
 * @retval ManufactorID
 */
WORD
Opt3001GetManufactorID(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
WORD
Opt3001GetDeviceID(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
DWORD
Opt3001GetLux(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
Opt3001SetTimeConvert(
    WORD wTime
);

/**
 * @func   Opt3001ReadLux
 * @brief  None
 * @param  None
 * @retval None
 */
DWORD
Opt3001ReadLux(void);

/**
 * @func   Opt3001StartMeasure
 * @brief  None
 * @param  None
 * @retval None
 */
void
Opt3001StartMeasure(void);

#endif /* OPT3001_H_ */
