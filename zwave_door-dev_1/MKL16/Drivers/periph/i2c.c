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
#include "configapp.h"
#include "configboard.h"
#include "sim.h"
#include "pincommon.h"
#include "pinmap.h"
#include "i2c.h"
#include "gpio.h"
#include "debug.h"
#include "timer.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef I2C_DEBUG
#define DBG_I2C_SEND_STR(x)         Debug_SendStr(x)
#define DBG_I2C_SEND_NUM(x)         Debug_SendNum(x)
#define DBG_I2C_SEND_HEX(x)         Debug_SendHex(x)
#else
#define DBG_I2C_SEND_STR(x)
#define DBG_I2C_SEND_NUM(x)
#define DBG_I2C_SEND_HEX(x)
#endif

#define SIZE_SCL_DIVIDER            0x3F
#define FLAG_CHANGE_OFF             2
#define I2C_WAIT_ACK_TIMEOUT        50000  
  
#define I2C_CMDBUFF_SIZE            32
#define I2C_CMD_SIZE                3
#define I2C_TIMEOUT                 30  /* ms */
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typedef struct _i2c_pin_ {
    DWORD scl;
    DWORD sda;
} i2c_t;
   
const i2c_divider_t I2CDividerTable[SIZE_SCL_DIVIDER] = 
    {{ 0x00, 20 } , { 0x01, 22 } , { 0x02, 24 } , { 0x03, 26 },
     { 0x04, 28 } , { 0x05, 30 } , { 0x09, 32 } , { 0x06, 34 },
     { 0x0a, 36 } , { 0x07, 40 } , { 0x0c, 44 } , { 0x10, 48 }, 
     { 0x11, 56 } , { 0x12, 64 } , { 0x0f, 68 } , { 0x13, 72 }, 
     { 0x14, 80 } , { 0x15, 88 } , { 0x19, 96 } , { 0x16, 104 },
     { 0x1a, 112 }, { 0x17, 128 }, { 0x1c, 144 }, { 0x1d, 160 }, 
     { 0x1e, 192 }, { 0x22, 224 }, { 0x1f, 240 }, { 0x23, 256 },
     { 0x24, 288 }, { 0x25, 320 }, { 0x26, 384 }, { 0x2a, 448 },
     { 0x27, 480 }, { 0x2b, 512 }, { 0x2c, 576 }, { 0x2d, 640 },
     { 0x2e, 768 }, { 0x32, 896 }, { 0x2f, 960 }, { 0x33, 1024 }, 
     { 0x34, 1152}, { 0x35, 1280 },{ 0x36, 1536 },{ 0x3a, 1792 },
     { 0x37, 1920 },{ 0x3b, 2048 },{ 0x3c, 2304 },{ 0x3d, 2560 }, 
     { 0x3e, 3072 },{ 0x3f, 3840 }};
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
static i2c_t const g_i2cPin[I2C_COUNT] = { I2C_PIN };
static IRQn_Type const g_i2cIRQ[I2C_COUNT] = { I2C0_IRQn, I2C1_IRQn};
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
i2cStatus_t
I2C_WAIT(
    BYTE byI2Cnum
) {
    DWORD dwTimeTimeout = GetMilSecTick() + I2C_TIMEOUT;
    while(((g_i2cbase[byI2Cnum])->S & I2C_S_IICIF_MASK)==0) {
        if (GetMilSecTick() >= dwTimeTimeout) return i2cTimeout;
    };

    g_i2cbase[byI2Cnum]->S |= I2C_S_IICIF_MASK;

    return i2cSuccess;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C_START(
    BYTE byI2Cnum
) {
    g_i2cbase[byI2Cnum]->C1 |= I2C_C1_TX_MASK;
    g_i2cbase[byI2Cnum]->C1 |= I2C_C1_MST_MASK;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C_STOP(
    BYTE byI2Cnum
) {
    g_i2cbase[byI2Cnum]->C1 &= ~I2C_C1_MST_MASK;
    g_i2cbase[byI2Cnum]->C1 &= ~I2C_C1_TX_MASK;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C_RSTART(
    BYTE byI2Cnum
) {
    g_i2cbase[byI2Cnum]->C1 |= I2C_C1_RSTA_MASK;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void 
I2C_WD (
    BYTE byI2Cnum,
    BYTE bydata
) {
    g_i2cbase[byI2Cnum]->D = bydata;    
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
I2C_RD (
    BYTE byI2Cnum
) {
    return (g_i2cbase[byI2Cnum]->D);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C_SetDirMode(
    BYTE byI2Cnum,
    BYTE byMode 
) {    
    if (byMode == i2cSend)
         g_i2cbase[byI2Cnum]->C1 |= I2C_C1_TX_MASK; 
    else 
         g_i2cbase[byI2Cnum]->C1 &= ~ I2C_C1_TX_MASK;  
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C_ACK(
    BYTE byI2Cnum
) {       
    g_i2cbase[byI2Cnum]->C1 &= ~I2C_C1_TXAK_MASK; 
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C_NACK(
    BYTE byI2Cnum
) { 	
    g_i2cbase[byI2Cnum]->C1 |= I2C_C1_TXAK_MASK; 
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL 
I2C_CHECK_TXCOMPLETE(
    BYTE byI2Cnum
) {
    if (g_i2cbase[byI2Cnum]->S & I2C_S_TCF_MASK) return TRUE;
     
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C_CLR_ARBL(
    BYTE i2cNum
) {
    g_i2cbase[i2cNum]->S |= I2C_S_ARBL_MASK;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval FALSE: No acknowledge signal detected 
 */
BOOL
I2C_CHECK_RXACK(
    BYTE byI2Cnum
) {
    if (g_i2cbase[byI2Cnum]->S & I2C_S_RXAK_MASK)  return FALSE;
    
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval TRUE: address as a slave
 */
BOOL 
I2C_CHECK_ADDR(
    BYTE byI2Cnum
) {
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval TRUE: Bus is busy, FALSE: bus is ide
 */
BOOL 
I2C_CHECK_BUSBUSY(
    BYTE byI2Cnum
) {
    if (g_i2cbase[byI2Cnum]->S & I2C_S_BUSY_MASK)  return TRUE;

    return FALSE;
}

/**
 * @func
 * @brief  Delay
 * @param  None
 * @retval None
 */
void
PAUSE() {
    delay_ms(2);
}

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
) {
    DWORD mult, i, multiplier, computedRate, absError;
    DWORD hz = baudKbps * 1000;
    DWORD bestError = 0xffffffffu;
    DWORD bestMult = 0u;
    DWORD bestIcr = 0u;

    /* Search for the settings with the lowest error.
     * mult is the MULT field of the I2C_F register, and ranges from 0-2. It selects the
     * multiplier factor for the divider. */
    for (mult = 0u; (mult <= 2u) && (bestError != 0); ++mult)
    {
        multiplier = 1u << mult;

        /* Scan table to find best match.*/
        for (i = 0u; i < SIZE_SCL_DIVIDER; ++i)
        {
            computedRate = SystemBusClock / (multiplier * I2CDividerTable[i].sclDivider);
            absError = (hz > computedRate) ? (hz - computedRate) : (computedRate - hz);

            if (absError < bestError)
            {
                bestMult = mult;
                bestIcr = I2CDividerTable[i].icr;
                bestError = absError;

                /* If the error is 0, then we can stop searching
                 * because we won't find a better match.*/
                if (absError == 0)
                {
                    break;
                }
            }
        }
    }

    /* Set the resulting error.*/
    if (absoluteError_Hz)
    {
        *absoluteError_Hz = bestError;
    }
    /* Set frequency register based on best settings.*/
    g_i2cbase[byI2Cnum]->F |= I2C_F_MULT(bestMult) | I2C_F_ICR(bestIcr);
}

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
) {
    if (byI2Cnum >= I2C_COUNT) return;    
    CLOCK_EnablePortClock(EXTRACT_PORT(g_i2cPin[byI2Cnum].scl));
    pinmap_peripheral(g_i2cPin[byI2Cnum].scl, PinMap_I2C_SCL);  
    CLOCK_EnablePortClock(EXTRACT_PORT(g_i2cPin[byI2Cnum].sda));
    pinmap_peripheral(g_i2cPin[byI2Cnum].sda, PinMap_I2C_SDA);
    CLOCK_SetI2CClock(byI2Cnum, ENABLE);
    g_i2cbase[byI2Cnum]->A1 = byAddr << 1;          /* Set slave addr */ 
    g_i2cbase[byI2Cnum]->C2 &= ~ I2C_C2_ADEXT_MASK; /* Addr 7bit */
    g_i2cbase[byI2Cnum]->FLT |= I2C_FLT_FLT(0);
    g_i2cbase[byI2Cnum]->A2 = 0x00;
    g_i2cbase[byI2Cnum]->C1 |= I2C_C1_IICEN_MASK;   /* Enable i2c module */
    g_i2cbase[byI2Cnum]->C1 |= I2C_C1_IICIE_MASK;   /* Enable i2c interrup */
    NVIC_EnableIRQ(g_i2cIRQ[byI2Cnum]);             /* Enable NVIC */

    #ifdef I2C_HAS_STOP_DETECT
    g_i2cbase[byI2Cnum]->FLT |= I2C_FLT_STOPIE_MASK;
    #endif

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C_MasterInit(
    BYTE byI2Cnum,
    DWORD baudKbps
) {
    DWORD errbaud = 0;

    if (byI2Cnum >= I2C_COUNT) return;    
    CLOCK_EnablePortClock(EXTRACT_PORT(g_i2cPin[byI2Cnum].scl));
    pinmap_peripheral(g_i2cPin[byI2Cnum].scl, PinMap_I2C_SCL);
    CLOCK_EnablePortClock(EXTRACT_PORT(g_i2cPin[byI2Cnum].sda));
    pinmap_peripheral(g_i2cPin[byI2Cnum].sda, PinMap_I2C_SDA);
    CLOCK_SetI2CClock(byI2Cnum, ENABLE);
    I2C_SetBaudRate(byI2Cnum, baudKbps, &errbaud); 
    g_i2cbase[byI2Cnum]->C1 |= I2C_C1_IICEN_MASK; /* Enable I2C module */
//   g_i2cbase[byI2Cnum]->C1 |= I2C_C1_IICIE_MASK; /* Enable i2c interrup */
//   NVIC_EnableIRQ(g_i2cIRQ[byI2Cnum]);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void 
I2C0_IRQHandler(void) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
I2C1_IRQHandler() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
i2cStatus_t
I2C_MasterWriteReg(
    BYTE byI2Cnum,
    BYTE byAddrSlave,
    BYTE byRegAdd,
    BYTE data
) {
    BYTE byTemp = 0;
    i2cStatus_t status;

    g_i2cbase[byI2Cnum]->S |= I2C_S_ARBL_MASK;
    I2C_START(byI2Cnum);
    byTemp = (byAddrSlave << 1) | WRITE;
    I2C_WD(byI2Cnum, byTemp);
    status = I2C_WAIT(byI2Cnum);
    if (status != i2cSuccess) return status;
    if (!I2C_CHECK_RXACK(byI2Cnum)) {
        I2C_STOP(byI2Cnum);
    }
    byTemp =  byRegAdd & (~ AUTO_INCRE_MASK); 
    I2C_WD(byI2Cnum, byTemp);
    status = I2C_WAIT(byI2Cnum);
    I2C_WD(byI2Cnum, data);
    status = I2C_WAIT(byI2Cnum);   
    I2C_STOP(byI2Cnum);
    PAUSE();
    return i2cSuccess;
}

/**
 * @func   I2C_MasterReadReg
 * @brief  None
 * @param  None
 * @retval
 */
i2cStatus_t
I2C_MasterReadReg(
    BYTE byI2Cnum,
    BYTE byAddrSlave,
    BYTE byRegAdd,
    BYTE_p byRead
) {
    BYTE byTemp = 0;

    g_i2cbase[byI2Cnum]->S |= I2C_S_ARBL_MASK;   /* Clear ARBL */
    I2C_START(byI2Cnum);                         /* Start */
    byTemp = (byAddrSlave << 1) | WRITE;         /* Addr slave + write */
    I2C_WD(byI2Cnum, byTemp);
    I2C_WAIT(byI2Cnum);
    byTemp =  byRegAdd & (~ AUTO_INCRE_MASK);    /* Disable auto increment + reg addr */
    I2C_WD(byI2Cnum, byTemp);
    I2C_WAIT(byI2Cnum);
    I2C_RSTART(byI2Cnum);                        /* Repeated start */
    byTemp = (byAddrSlave << 1) | READ;          /* Read register from slave */
    I2C_WD(byI2Cnum, byTemp);
    I2C_WAIT(byI2Cnum);
    I2C_SetDirMode(byI2Cnum, i2cReceiver);       /* RX MODE */
    I2C_NACK(byI2Cnum);
    *byRead = I2C_RD(byI2Cnum);                  /* Dummy read */
    I2C_WAIT(byI2Cnum);
    I2C_STOP(byI2Cnum);                          /* Send stop since about to read last byte */
    *byRead = I2C_RD(byI2Cnum);                  /* Read byte */ 
    PAUSE();
    return i2cSuccess;
}

/**
 * @func   I2C_DeInit
 * @brief  DeInit i2c pin, ungate clock
 * @param  None
 * @retval None
 */
void 
I2C_DeInit(
    BYTE i2cNum
) {
    GPIO_DeInitPin(g_i2cPin[i2cNum].scl);
    GPIO_DeInitPin(g_i2cPin[i2cNum].sda);
    CLOCK_SetI2CClock(i2cNum, DISABLE);;
}

/* END FILE */
