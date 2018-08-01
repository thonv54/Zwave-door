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
#include "driver_common.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
DWORD const g_portclock[GPIO_PORT_COUNT] = GPIO_CLK;
DWORD const g_spiclock[SPI_COUNT] = SPI_CLK;
DWORD const g_uartclock[UART_COUNT] = UART_CLK;
DWORD const g_i2cclock[I2C_COUNT] =  I2C_CLK;
DWORD const g_tpmclock[TPM_COUNT] = TPM_CLK;

GPIO_Type * const g_gpiobase[GPIO_PORT_COUNT] = GPIO_BASES;
PORT_Type * const g_gpioport[GPIO_PORT_COUNT] = PORT_BASES;
SPI_Type * const g_spibase[SPI_COUNT] = SPI_BASES;
UART_Type * const g_uartbase[UART_COUNT] = { 0 , UART1, UART2};
I2C_Type * const g_i2cbase[I2C_COUNT] = I2C_BASES;
TPM_Type * const g_tpmbase[TPM_COUNT] = TPM_BASES;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static volatile BYTE IrqNestLevel = 0;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

void 
delay_ms(
    DWORD ms
) {
    DWORD i;
    DWORD fms = (SystemCoreClock / 4000) * ms;
    for (i = 0; i < fms; i++) {}
}

void 
delay_us(
    DWORD us
) {
    DWORD i;
    DWORD fms;

    if (SystemCoreClock > 4000000) {
        fms = (SystemCoreClock / 4000000) * us;
    }
    else {
        fms = (SystemCoreClock * us) / 4000000;
    }

    for (i = 0; i < fms; i++) {}
}

void
_enter_critical(void) {
    __disable_irq();
    IrqNestLevel++;  
}

void
_exit_critical(void) {
    IrqNestLevel--;
    if( IrqNestLevel == 0 ) {
        __enable_irq();
    }   
}

/* END FILE */
