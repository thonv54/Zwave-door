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

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "configapp.h"
#include "driver_common.h"
#include "irq.h"
#include "debug.h"
#include "mcg.h"
#include "smc.h"
#include "llwu.h"
#include "gpio.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef USE_DEBUG
#define DBG_SEND_STR(x)             Debug_SendStr(x)
#define DBG_SEND_NUM(x)             Debug_SendNum(x)
#define DBG_SEND_HEX(x)             Debug_SendHex(x)
#else
#define DBG_SEND_STR(x)
#define DBG_SEND_NUM(x)
#define DBG_SEND_HEX(x)
#endif
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
 * @func   LLW_IRQHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void
LLW_IRQHandler(void) {
    
}

/**
 * @func   HardFault_Handler
 * @brief  None
 * @param  None
 * @retval None
 */
void 
HardFault_Handler(void) {
    DBG_SEND_STR("HardFault\n");
    while(1) {}
}

/**
 * @func   PendSV_Handler
 * @brief  None
 * @param  None
 * @retval None
 */
void
PendSV_Handler(void) {
    DBG_SEND_STR("PendSV\n");
}

/**
 * @func   NMI_Handler
 * @brief  None
 * @param  None
 * @retval None
 */
void
NMI_Handler(void) {
    DBG_SEND_STR("NMI\n");
}

/* END FILE */
