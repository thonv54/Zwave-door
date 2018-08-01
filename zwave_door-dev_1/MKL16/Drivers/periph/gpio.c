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
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "gpio.h"
#include "sim.h"
#include "driver_common.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define IN                          0
#define OUT                         1
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
 * @retval None
 */
static void
pin_dir(
    BYTE pin,
    BYTE port,
    BYTE dir    
) {
    if (dir == OUT) {
        g_gpiobase[port]->PDDR |= 1 << pin;
    }
    else if (dir == IN) {
        g_gpiobase[port]->PDDR &= ~(1 << pin);
    }
}

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_SetPinIsOutput(
    WORD IoPin
) {
     WORD pin, port;
     port = EXTRACT_PORT(IoPin);
     pin = EXTRACT_PIN(IoPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return;

     CLOCK_EnablePortClock(port);
     pin_func(pin, port, 1);
     pin_dir(pin, port, OUT);    
}

/**
 * @func   GPIO_SetPinIsInput
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_SetPinIsInput(
    WORD IoPin,
    BYTE pullMode
) {
     WORD pin, port;
     port = EXTRACT_PORT(IoPin);
     pin = EXTRACT_PIN(IoPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return;

     CLOCK_EnablePortClock(port);
     pin_func(pin, port, 1);
     pin_dir(pin, port, IN); 
     pin_mode(pin, port, pullMode); 
}

/**
 * @func   GPIO_DeInitPin
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_DeInitPin(
    WORD IoPin
) {
     WORD pin, port;
     port = EXTRACT_PORT(IoPin);
     pin = EXTRACT_PIN(IoPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return;

     g_gpiobase[port]->PCOR = (1 << pin);
     pin_func(pin, port, 0);
}

/**
 * @func   GPIO_PinHigh
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_PinHigh (
    WORD IoPin
) {
     WORD pin, port;
     port = EXTRACT_PORT(IoPin);
     pin = EXTRACT_PIN(IoPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return; 
     g_gpiobase[port]->PSOR = (1 << pin);
}

/**
 * @func   GPIO_PinLow
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_PinLow (
    WORD IoPin
) {
     WORD pin, port;
     port = EXTRACT_PORT(IoPin);
     pin = EXTRACT_PIN(IoPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return;
     g_gpiobase[port]->PCOR = (1 << pin);
}

/**
 * @func   GPIO_PinToggle
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_PinToggle(
    WORD IoPin
) {
     WORD pin, port;
     port = EXTRACT_PORT(IoPin);
     pin = EXTRACT_PIN(IoPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return;
     g_gpiobase[port]->PTOR = (1 << pin);
}

/**
 * @func   GPIO_PinReadInput 
 * @brief  None
 * @param  None
 * @retval BOOL
 */
BOOL
GPIO_PinReadInput(
    WORD IoPin
) {
     BOOL bState = TRUE;    
     WORD pin, port;
     port = EXTRACT_PORT(IoPin);
     pin = EXTRACT_PIN(IoPin);

     if(g_gpiobase[port]->PDIR & (1 << pin)) bState = TRUE ;
     else bState = FALSE;

     return bState;
}

/**
 * @func   GPIO_SetPinIrq
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_SetPinIrq(
    WORD IoPin,
    BYTE byModeIrq
) {
    WORD pin, port;
    port = EXTRACT_PORT(IoPin);
    pin = EXTRACT_PIN(IoPin); 

    g_gpioport[port]->PCR[pin] &= ~PORT_PCR_IRQC_MASK;
    g_gpioport[port]->PCR[pin] |= PORT_PCR_IRQC(byModeIrq); 
}

/**
 * @func   GPIO_GetITStatus  
 * @brief  None
 * @param  None
 * @retval SET OR RESET
 */
ITStatus
GPIO_GetITStatus(
    WORD IoPin
) {
     WORD pin, port;
     port = EXTRACT_PORT(IoPin);
     pin = EXTRACT_PIN(IoPin); 

    if (g_gpioport[port]->PCR[pin] & PORT_PCR_ISF_MASK) 
        return SET;
    else 
        return RESET;   
}

/**
 * @func   GPIO_ClearITPendingBit
 * @brief  None
 * @param  None
 * @retval None
 */
void
GPIO_ClearITPendingBit(
    WORD IoPin
) {
     WORD pin, port;
     port = EXTRACT_PORT(IoPin);
     pin = EXTRACT_PIN(IoPin); 

    g_gpioport[port]->PCR[pin] |= PORT_PCR_ISF_MASK; 
}

/* END FILE */
