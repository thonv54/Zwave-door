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
#ifndef _DRIVER_COMMON_H_
#define _DRIVER_COMMON_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
#include "MKL16Z4.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define NO_PIN                      0xFF                 
#define NO_PORT                     0xFF
#define NC                          0xFFFFU
#define SHIFT_PORT                  8u
#define MAKE_PIN(r,p)               (((r) << SHIFT_PORT) | (p))
#define EXTRACT_PORT(v)             ((v) >> SHIFT_PORT)
#define EXTRACT_PIN(v)              ((v) & 0xFFU)

#define GPIO_PORT_COUNT             5
#define GPIO_PIN_COUNT              32

#define I2C_COUNT                   2
#define I2C0_IDX                    0
#define I2C1_IDX                    1

#define GPIOA_IDX                   0
#define GPIOB_IDX                   1
#define GPIOC_IDX                   2
#define GPIOD_IDX                   3
#define GPIOE_IDX                   4

#define SPI_COUNT                   2
#define SPI0_IDX                    0
#define SPI1_IDX                    1

#define UART_COUNT                  3
#define UART0_IDX                   0
#define UART1_IDX                   1
#define UART2_IDX                   2

#define TPM_COUNT                   3
#define TPM0_IDX                    0
#define TPM1_IDX                    1
#define TPM2_IDX                    2

#define PIT_COUNT                   2
#define PIT0_IDX                    0
#define PIT1_IDX                    1

#define TSI_MAX_CHANEL              16

#define SCB_SCR                     ((SCB)->SCR)
#define PMC_REGSC                   ((PMC)->REGSC)

#define GPIO_CLK  { SIM_SCGC5_PORTA_MASK, SIM_SCGC5_PORTB_MASK, \
                    SIM_SCGC5_PORTC_MASK, SIM_SCGC5_PORTD_MASK, \
                    SIM_SCGC5_PORTE_MASK }

#define UART_CLK  { SIM_SCGC4_UART0_MASK, SIM_SCGC4_UART1_MASK, \
                    SIM_SCGC4_UART2_MASK }

#define I2C_CLK   { SIM_SCGC4_I2C0_MASK, SIM_SCGC4_I2C1_MASK }

#define SPI_CLK   { SIM_SCGC4_SPI0_MASK, SIM_SCGC4_SPI1_MASK }

#define TPM_CLK   { SIM_SCGC6_TPM0_MASK, SIM_SCGC6_TPM1_MASK, \
                    SIM_SCGC6_TPM2_MASK }

#define MCU_RESET             NVIC_SystemReset()
#define ENTER_CRITCAL         _enter_critical()
#define EXIT_CRITICAL         _exit_critical()

#define PTA0                        MAKE_PIN(GPIOA_IDX, 0u)
#define PTA1                        MAKE_PIN(GPIOA_IDX, 1u)
#define PTA2                        MAKE_PIN(GPIOA_IDX, 2u)
#define PTA3                        MAKE_PIN(GPIOA_IDX, 3u)
#define PTA4                        MAKE_PIN(GPIOA_IDX, 4u)
#define PTA5                        MAKE_PIN(GPIOA_IDX, 5u)
#define PTA12                       MAKE_PIN(GPIOA_IDX,12u)
#define PTA13                       MAKE_PIN(GPIOA_IDX,13u)
#define PTA18                       MAKE_PIN(GPIOA_IDX,18u)
#define PTA19                       MAKE_PIN(GPIOA_IDX,19u)
#define PTA20                       MAKE_PIN(GPIOA_IDX,20u)

#define PTB0                        MAKE_PIN(GPIOB_IDX, 0u)
#define PTB1                        MAKE_PIN(GPIOB_IDX, 1u)
#define PTB2                        MAKE_PIN(GPIOB_IDX, 2u)
#define PTB3                        MAKE_PIN(GPIOB_IDX, 3u)
#define PTB16                       MAKE_PIN(GPIOB_IDX,16u)
#define PTB17                       MAKE_PIN(GPIOB_IDX,17u)
#define PTB18                       MAKE_PIN(GPIOB_IDX,18u)
#define PTB19                       MAKE_PIN(GPIOB_IDX,19u)

#define PTC0                        MAKE_PIN(GPIOC_IDX, 0u) 
#define PTC1                        MAKE_PIN(GPIOC_IDX, 1u) 
#define PTC2                        MAKE_PIN(GPIOC_IDX, 2u) 
#define PTC3                        MAKE_PIN(GPIOC_IDX, 3u)
#define PTC4                        MAKE_PIN(GPIOC_IDX, 4u) 
#define PTC5                        MAKE_PIN(GPIOC_IDX, 5u) 
#define PTC6                        MAKE_PIN(GPIOC_IDX, 6u)
#define PTC7                        MAKE_PIN(GPIOC_IDX, 7u)  
#define PTC8                        MAKE_PIN(GPIOC_IDX, 8u) 
#define PTC9                        MAKE_PIN(GPIOC_IDX, 9u) 
#define PTC10                       MAKE_PIN(GPIOC_IDX,10u)
#define PTC11                       MAKE_PIN(GPIOC_IDX,11u)  

#define PTD0                        MAKE_PIN(GPIOD_IDX, 0u) 
#define PTD1                        MAKE_PIN(GPIOD_IDX, 1u) 
#define PTD2                        MAKE_PIN(GPIOD_IDX, 2u)
#define PTD3                        MAKE_PIN(GPIOD_IDX, 3u) 
#define PTD4                        MAKE_PIN(GPIOD_IDX, 4u) 
#define PTD5                        MAKE_PIN(GPIOD_IDX, 5u) 
#define PTD6                        MAKE_PIN(GPIOD_IDX, 6u)
#define PTD7                        MAKE_PIN(GPIOD_IDX, 7u)  

#define PTE0                        MAKE_PIN(GPIOE_IDX, 0u)
#define PTE1                        MAKE_PIN(GPIOE_IDX, 1u)
#define PTE16                       MAKE_PIN(GPIOE_IDX,16u)
#define PTE17                       MAKE_PIN(GPIOE_IDX,17u)
#define PTE18                       MAKE_PIN(GPIOE_IDX,18u)
#define PTE19                       MAKE_PIN(GPIOE_IDX,19u)
#define PTE20                       MAKE_PIN(GPIOE_IDX,20u)
#define PTE21                       MAKE_PIN(GPIOE_IDX,21u)
#define PTE22                       MAKE_PIN(GPIOE_IDX,22u)
#define PTE23                       MAKE_PIN(GPIOE_IDX,23u)
#define PTE24                       MAKE_PIN(GPIOE_IDX,24u)
#define PTE25                       MAKE_PIN(GPIOE_IDX,25u)
#define PTE29                       MAKE_PIN(GPIOE_IDX,29u)
#define PTE30                       MAKE_PIN(GPIOE_IDX,30u)
#define PTE31                       MAKE_PIN(GPIOE_IDX,31u)

#define MODUL_TSI                   MAKE_PIN(WU_TSI0, NO_PIN)
#define MODUL_RTC                   MAKE_PIN(WU_RTCA, NO_PIN)
#define MODUL_LPTMR                 MAKE_PIN(WU_LPTMR,NO_PIN)

typedef enum _IRQn_prio_ {
    IRQn_LevelHighest = 0,
    IRQn_LevelHigh = 1,
    IRQn_LevelNomal = 2,
    IRQn_LevelLow = 3,
} IRQnLevel;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
extern DWORD const g_portclock[GPIO_PORT_COUNT];
extern DWORD const g_spiclock[SPI_COUNT];
extern DWORD const g_uartclock[UART_COUNT];
extern DWORD const g_i2cclock[I2C_COUNT];
extern DWORD const g_tpmclock[TPM_COUNT];
extern GPIO_Type* const g_gpiobase[GPIO_PORT_COUNT];
extern PORT_Type* const g_gpioport[GPIO_PORT_COUNT];
extern UART_Type * const g_uartbase[UART_COUNT];
extern I2C_Type * const g_i2cbase[I2C_COUNT];
extern SPI_Type * const g_spibase[SPI_COUNT];
extern TPM_Type * const g_tpmbase[TPM_COUNT];
extern uint32_t SystemBusClock;
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   delay_ms
 * @brief  None
 * @param  None
 * @retval None
 */
void 
delay_ms(
    DWORD ms
);

/**
 * @func   delay_us
 * @brief  None
 * @param  None
 * @retval None
 */
void 
delay_us(
    DWORD us
);

extern void _enter_critical(void);
extern void _exit_critical(void);

#endif /* END FILE */
