/*******************************************************************************
 *
 * Copyright (c) 2018
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
#ifndef _ZWAVE_NETW_STATUS_
#define _ZWAVE_NETW_STATUS_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef enum {
    ZW_NETW_REQ = 0x00,
    ZW_NETW_NOTNW = 0x01,
    ZW_NETW_INNW = 0x02
} ZW_NETW_STATUS;

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
 * @func   GetZwNetwStatus
 * @brief  None
 * @param  None
 * @retval None 
 */
BYTE
GetZwNetwStatus(void);

/**
 * @func   SetZwNetwStatus 
 * @brief  None
 * @param  None
 * @retval None 
 */
void
SetZwNetwStatus(
    ZW_NETW_STATUS status
);

void
SetZwNetwReqCount(
    BYTE count
);

BYTE 
GetZwNetwReqCount(void);

#endif /* END FILE */
