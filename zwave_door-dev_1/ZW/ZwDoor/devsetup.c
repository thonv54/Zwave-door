/*******************************************************************************
 *
 * Copyright (c) 2017
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

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "eeprom.h"
#include "devsetup.h"
#include "config_app.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BYTE pDevSetup[DEVICE_SETUP_SIZE];
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
paraSetup_p pParamSetup;
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   LoadParamSetup
 * @brief  None
 * @param  None
 * @retval None
 */
void
LoadParamSetup(void) {
    MemoryGetBuffer((WORD)EEOFFSET_DEVICES_SETUP_far, pDevSetup, DEVICE_SETUP_SIZE);
    pParamSetup = (paraSetup_p)pDevSetup;
}

/**
 * @func   GetParamSetup
 * @brief  None
 * @param  None
 * @retval None
 */
void
GetParamSetup(
    BYTE param,
    void* val
) {
    switch(param) {
    case PARAM_MODE_SELECT:
        *((BYTE_P)val) =  pParamSetup->modesel;
        break;

    case PARAM_TIME_REPORT_SENS:
        *((WORD_P)val) = pParamSetup->timeRpSens;
        break;
    }   
}

/**
 * @func   SetParamSetup
 * @brief  None
 * @param  None
 * @retval None
 */
void
SetParamSetup(
    BYTE param,
    void* val
) {
    switch(param) {
    case PARAM_MODE_SELECT:
        pParamSetup->modesel = *((BYTE_P)val);
        break;
    }

    case PARAM_TIME_REPORT_SENS:
        pParamSetup->timeRpSens = *((WORD_P)val);
        break;

    MemoryPutBuffer((WORD)EEOFFSET_DEVICES_SETUP_far, pDevSetup, DEVICE_SETUP_SIZE, NULL);
}

