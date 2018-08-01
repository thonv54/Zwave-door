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
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "pincommon.h"
#include "driver_common.h"
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
) {
    g_gpioport[port]->PCR[pin] = \
                  (g_gpioport[port]->PCR[pin] & ~PORT_PCR_MUX_MASK) \
                  | PORT_PCR_MUX(altFunc);
}

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
) {
    BYTE pin_n;
    BYTE port_n;

    while (map->pin != NC) {
        if (map->pin == IoPin) {
            port_n = EXTRACT_PORT(IoPin);
            pin_n = EXTRACT_PIN(IoPin);
            pin_func(pin_n, port_n, map->altfunc);

            return TRUE;
        }            
        map++;
    }

    return FALSE;
}

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
) {
    if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) 
        return;

    if (mode == PIN_FLOATING) {
        g_gpioport[port]->PCR[pin] &= ~PORT_PCR_PE_MASK;
    }
    else {
        g_gpioport[port]->PCR[pin] |= PORT_PCR_PE_MASK;

        if (mode == PIN_PULL_UP) {
            g_gpioport[port]->PCR[pin] |= PORT_PCR_PS_MASK;
        }
        else if (mode == PIN_PULL_DOWN) {
            g_gpioport[port]->PCR[pin] &= ~PORT_PCR_PS_MASK;
        }
    }   
}

/* END FILE */
