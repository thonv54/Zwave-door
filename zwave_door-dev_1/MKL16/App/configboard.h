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
#ifndef _CONFIG_BOARD_H_
#define _CONFIG_BOARD_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "driver_common.h"
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
#define WAKEUP_MKL              PTB0  /* LLWU_P5 */
#define WAKUP_ZWAVE             PTE0
#define LEDB                    PTD4
#define LEDR                    PTD5
#define DOOR_SENS               PTC6  /* LLWU_P10 */

#define PIN_CONFIG              PTC5  /* LLWU_P9 */
#define KEY_CONFIG              { PIN_CONFIG, 0 }
#define KEY_MAX                 1

#define UART_PIN                { PTD7, PTD6 },{ PTC4, PTC3 },{ PTE16, PTE17}    
    
#define UART_ZW                 0                                                       
#define UART_DEBUG              2
#endif
/******************************************************************************/
/*                                 END FILE                                   */
/******************************************************************************/
