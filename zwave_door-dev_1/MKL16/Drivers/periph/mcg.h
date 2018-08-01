/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: 
 *
 * Last Changed By:  $ thangdh $
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date: 22/08/17 09:00 $
 *
 ******************************************************************************/
#ifndef _MCG_H_
#define _MCG_H_ 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "driver_common.h"
#include "typedefs.h"
#include "macro.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define NO_OSCINIT                  0
#define OSCINIT                     1
#define OSC_0                       0
#define OSC_1                       1
#define LOW_POWER                   0
#define HIGH_GAIN                   1
#define CANNED_OSC                  0
#define CRYSTAL                     1
#define PLL_0                       0
#define PLL_1                       1
#define PLL_ONLY                    0
#define MCGOUT                      1

#define SLOW_IRC                    0
#define FAST_IRC                    1

#define CPU_FASTIRC_CLK               4000000
#define CPU_SLOWIRC_CLK               32768

/*! @brief MCG mode definitions */
typedef enum _mcg_modes {
    McgModeFEI    = 0x01 << 0U,   /*!< FEI   - FLL Engaged Internal         */
    McgModeFBI    = 0x01 << 1U,   /*!< FBI   - FLL Bypassed Internal        */
    McgModeBLPI   = 0x01 << 2U,   /*!< BLPI  - Bypassed Low Power Internal  */
    McgModeFEE    = 0x01 << 3U,   /*!< FEE   - FLL Engaged External         */
    McgModeFBE    = 0x01 << 4U,   /*!< FBE   - FLL Bypassed External        */
    McgModeBLPE   = 0x01 << 5U,   /*!< BLPE  - Bypassed Low Power External  */
    McgModePBE    = 0x01 << 6U,   /*!< PBE   - PLL Bypassed Enternal        */
    McgModePEE    = 0x01 << 7U,   /*!< PEE   - PLL Engaged External         */
    McgModeSTOP   = 0x01 << 8U,   /*!< STOP  - Stop                         */
    McgModeError  = 0x01 << 9U    /*!< Unknown mode                         */
} mcgModes;
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
 * @func   CLOCK_SetFastIrcDivClock
 * @brief  None
 * @param  None
 * @retval None
 */
void
CLOCK_SetFastIrcDivClock(
    BYTE outDiv
);

/**
 * @func   MCG_GetMode
 * @brief  None
 * @param  None
 * @retval Current mcg mode
 */
mcgModes 
MCG_GetMode(void);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
MCG_ClkMonitor(
    FunctionalState newState
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
DWORD
MCG_FEI2FBI(
    DWORD irc_freq,
    BYTE irc_select
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
int 
MCG_FBI2BLPI(
    int irc_freq, 
    unsigned char irc_select
);

#endif /* _MCG_H_ */
