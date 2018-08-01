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
#ifndef _I2C_H_
#define _I2C_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "driver_common.h"
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define WRITE                       0x00
#define READ                        0x01
#define AUTO_INCRE_MASK             0x80

typedef struct __i2c_divider__ {
    BYTE icr;
    u16_t sclDivider;
} i2c_divider_t;

typedef enum __i2c_status__ {
    i2cSuccess            = 0x0U,  /*!< I2C operation has no error. */
    i2cInitialized        = 0x1U,  /*!< Current I2C is already initialized by one task.*/
    i2cFail               = 0x2U,  /*!< I2C operation failed. */
    i2cBusy               = 0x3U,  /*!< The master is already performing a transfer.*/
    i2cTimeout            = 0x4U,  /*!< The transfer timed out.*/
    i2cReceivedNak        = 0x5U,  /*!< The slave device sent a NAK in response to a byte.*/
    i2cSlaveTxUnderrun    = 0x6U,  /*!< I2C Slave TX Underrun error.*/
    i2cSlaveRxOverrun     = 0x7U,  /*!< I2C Slave RX Overrun error.*/
    i2cAribtrationLost    = 0x8U,  /*!< I2C Arbitration Lost error.*/
    i2cStopSignalFail     = 0x9U,  /*!< I2C STOP signal could not release bus. */
    i2cIdle               = 0xAU,  /*!< I2C Slave Bus is Idle. */
    i2cNoReceiveInProgress= 0xBU,  /*!< Attempt to abort a receiving when no transfer was in progress */
    i2cNoSendInProgress   = 0xCU   /*!< Attempt to abort a sending when no transfer was in progress */
} i2cStatus_t;

typedef enum __i2c_direction__ {
    i2cReceiver = 0,
    i2cSend = 1,          
} i2cDirection_t;
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
void I2C_START (BYTE byI2Cnum);
void I2C_STOP (BYTE byI2Cnum);
i2cStatus_t I2C_WAIT (BYTE byI2Cnum);
void I2C_RSTART (BYTE byI2Cnum);
void I2C_WD (BYTE byI2Cnum, BYTE bydata);
BYTE I2C_RD (BYTE byI2Cnum);
void I2C_SetDirMode (BYTE byI2Cnum, BYTE byMode);
void I2C_ACK (BYTE byI2Cnum);
void I2C_NACK (BYTE byI2Cnum);
BOOL I2C_CHECK_TXCOMPLETE (BYTE byI2Cnum);
BOOL I2C_CHECK_RXACK (BYTE byI2Cnum);
BOOL I2C_CHECK_BUSBUSY (BYTE byI2Cnum);
void I2C_CLR_ARBL( BYTE i2cNum);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C_SlaveInit(
    BYTE byI2Cnum,
    BYTE byAddr
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C_MasterInit(
    BYTE byI2Cnum,
    DWORD baud
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C_SetBaudRate(
    BYTE byI2Cnum,
    DWORD baudKbps,
    DWORD * absoluteError_Hz
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void 
I2C_DeInit(
    BYTE i2cNum
);

#endif /* END FILE */
