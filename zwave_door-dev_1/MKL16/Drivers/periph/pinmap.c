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
#include "pinmap.h"
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
const PinMap PinMap_RTC[] = {
    {PTC1,  1},
    {NC,    0}
};

const PinMap PinMap_ADC[] = {
    {PTB0,  0},
    {PTB1,  0},
    {PTC1,  0},
    {PTC2,  0},
    {PTD1,  0},
    {PTD5,  0},
    {PTD6,  0},
    {PTE16, 0},
    {PTE17, 0},
    {PTE18, 0},
    {PTE19, 0},
    {PTE20, 0},
    {PTE21, 0},
    {PTE22, 0},
    {PTE23, 0},
    {PTE30, 0},
    {NC,    0}
};

const PinMap PinMap_DAC[] = {
    {PTE30, 0},
    {NC   , 0}
};

const PinMap PinMap_I2C_SDA[] = {
    {PTE0,  6},
    {PTB1,  2},
    {PTC2,  2},
    {PTA4,  2},
    {NC  ,  0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PTE1,  6},
    {PTB0,  2},
    {PTC1,  2},
    {NC  ,  0}
};

const PinMap PinMap_UART_TX[] = {
    {PTA2,  2},
    {PTA19, 3},
    {PTB17, 3},
    {PTC4,  3},
    {PTD3,  3},
    {PTD5,  3},
    {PTD7,  3},
    {PTE0,  3},
    {PTE16, 3},
    {PTE20, 4},  
    {PTE22, 4},
    {NC  ,  0}
};

const PinMap PinMap_UART_RX[] = {
    {PTA1,  2},
    {PTA18, 3},
    {PTB16, 3},
    {PTC3,  3},
    {PTA1,  2},
    {PTD4,  3},
    {PTD6,  3},
    {PTE1,  3},
    {PTE17, 3},
    {PTE21, 4},  
    {PTE23, 4},
    {NC  ,  0}
};

const PinMap PinMap_SPI_SCLK[] = {
    {PTD5,  2},
    {PTC5,  2},
    {NC  ,  0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {PTD6,  2},
    {PTD7,  5},
    {PTE1,  2},
    {PTC6,  2},
    {PTC7,  5},
    {NC  ,  0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PTD6,  5},
    {PTD7,  2},
    {PTC6,  5},
    {PTC7,  2},
    {PTE1,  5},
    {NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PTD4,  2},
    {PTC4,  2},
    {NC  ,  0}
};

const PinMap PinMap_PWM[] = {
    {PTA0,  3}, // PTA0 , TPM0 CH5    
    {PTA1,  3}, // PTA1 , TPM2 CH0
    {PTA2,  3}, // PTA2 , TPM2 CH1
    {PTA3,  3}, // PTA3 , TPM0 CH0
    {PTA4,  3}, // PTA4 , TPM0 CH1  
    {PTB0,  3}, // PTB0 , TPM1 CH0
    {PTB1,  3}, // PTB1 , TPM1 CH1
    {PTC1,  4}, // PTC1 , TPM0 CH0
    {PTC2,  4}, // PTC2 , TPM0 CH1
    {PTC3,  4}, // PTC3 , TPM0 CH2
    {PTC4,  4}, // PTC4 , TPM0 CH3
    {PTD4,  4}, // PTD4 , TPM0 CH4
    {PTD5,  4}, // PTD5 , TPM0 CH5
    {PTE30, 3}, // PTE30, TPM0 CH3
    {NC   , 0}
};
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/* END FILE */
