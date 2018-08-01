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
#include "configboard.h"
#include "si7020.h"
#include "i2c.h"
#include "debug.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef SI7020_DEBUG
#define DBG_SI7020_SEND_STR(x)               Debug_SendStr(x)
#define DBG_SI7020_SEND_NUM(x)               Debug_SendNum(x)
#define DBG_SI7020_SEND_HEX(x)               Debug_SendHex(x)
#else
#define DBG_SI7020_SEND_STR(x)
#define DBG_SI7020_SEND_NUM(x)
#define DBG_SI7020_SEND_HEX(x)
#endif

#define SI7020_ADDR                          0x80
#define CMDR_MEASURE_VALUE                   0xE0
#define CMDW_HEATERCTR                       0x51
#define ADC_RES_RH12_T14     				           	0
#define ADC_RES_RH08_T12      				           1
#define ADC_RES_RH10_T13                     2
#define ADC_RES_RH11_T11                     3
/* Serial Number */
#define ID_SAMPLE                            0xFF
#define ID_SI7013                            0x0D
#define ID_SI7020                            0x14
#define ID_SI7021                            0x15
/* Firmware */
#define FIRMWARE_V11                         0xFF
#define FIRMWARE_V20                         0x20
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BYTE g_byTimeWaitGetTemp = 4;
static BYTE g_byTimeWaitGetHumi = 8;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static BYTE CalCRC8(BYTE_p pByData, BYTE byLength);
static void Si7020SetAdcResolution(BYTE byAdcRes);
static void SI7020SenCmdRead(
    BYTE_p pCmd,
    BYTE_p pDataRead,
    BYTE byLenData,
    WORD wDelay
);

static void
Si7020SenCmdWrite(
    BYTE byCmd,
    BYTE_p pData,
    BYTE byLenData
);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   Si7020Init
 * @brief  None
 * @param  None
 * @retval None
 */
void
Si7020Init(void) {
    #ifdef SI7020_DEBUG
    BYTE pReg[8];
    BYTE CMDR_USERREG1[2] = { 2, 0xE7 };
    BYTE CMDR_HEATERCTR[2] = { 2, 0x11 };
    #endif

    Si7020SetAdcResolution(ADC_RES_RH11_T11);

    #ifdef SI7020_DEBUG
    SI7020SenCmdRead(CMDR_USERREG1, pReg, 1, 0);  
    DBG_SI7020_SEND_STR("SI7020 UREG1: ");
    DBG_SI7020_SEND_HEX(pReg[0]);
    DBG_SI7020_SEND_STR("\n");

    SI7020SenCmdRead(CMDR_HEATERCTR, pReg, 1, 0);
    DBG_SI7020_SEND_STR("SI7020 HEATCTR: ");
    DBG_SI7020_SEND_HEX(pReg[0]);
    DBG_SI7020_SEND_STR("\n");

    Si7020GetVerFW();

    Si7020GetSerialNum(pReg);
    #endif /* SI7020_DEBUG */
}

/**
 * @func   Si7020SetAdcResolution
 * @brief  None
 * @param  None
 * @retval
 */
void
Si7020SetAdcResolution(
    BYTE byAdcRes
) {
    BYTE CMDW_USERREG1 = 0xE6;
    BYTE pByUserReg[1] = {0};

    switch(byAdcRes) {
    case ADC_RES_RH12_T14:
        g_byTimeWaitGetTemp = 15;
        g_byTimeWaitGetHumi = 25;
        pByUserReg[0] = 0x00;
        break;
        
    case ADC_RES_RH08_T12:
        g_byTimeWaitGetTemp = 3;
        g_byTimeWaitGetHumi = 4;
        pByUserReg[0] = 0x01;
        break;
        
    case ADC_RES_RH10_T13:
        g_byTimeWaitGetTemp = 10;
        g_byTimeWaitGetHumi = 8;
        pByUserReg[0] = 0x80;
        break;
        
    case ADC_RES_RH11_T11:
        g_byTimeWaitGetTemp = 4;
        g_byTimeWaitGetHumi = 8;
        pByUserReg[0] = 0x81;
        break;

    default :
        break;
    }
 
    Si7020SenCmdWrite(CMDW_USERREG1, pByUserReg, 1);
}

/**
 * @func   Si7020Reset
 * @brief  None
 * @param  None
 * @retval
 */
void
Si7020Reset(void) {
    BYTE pData[1];
    BYTE CMD_RESET = 0xFE;

    Si7020SenCmdWrite(CMD_RESET, pData, 0);   
}

/**
 * @func   Si7020GetSerialNum
 * @brief  None
 * @param  None
 * @retval
 */
void
Si7020GetSerialNum(
    u8_p pbySerial
) {    
    BYTE j = 0;
    BYTE pData[8];
    BYTE CMD_ELECID1[3] = {3, 0xFA, 0x0F };
    BYTE CMD_ELECID2[3] = {3, 0xFC, 0xC9 };

    SI7020SenCmdRead(CMD_ELECID1, pData, 8, 0);
    pbySerial[j++] = pData[0]; 
    pbySerial[j++] = pData[2]; 
    pbySerial[j++] = pData[4]; 
    pbySerial[j++] = pData[6]; 

    SI7020SenCmdRead(CMD_ELECID2, pData, 6, 0); 

    pbySerial[j++] = pData[0]; 
    pbySerial[j++] = pData[1]; 
    pbySerial[j++] = pData[3]; 
    pbySerial[j++] = pData[4]; 

    #ifdef SI7020_DEBUG
    DBG_SI7020_SEND_STR("SI7020 SERIAL: ");
    for (j = 0; j < 8; j++) {
        DBG_SI7020_SEND_HEX(pbySerial[j]);
        DBG_SI7020_SEND_STR(" ");
    }
    DBG_SI7020_SEND_STR("\n");
    #endif
}

/**
 * @func   Si7020GetVerFW
 * @brief  None
 * @param  None
 * @retval
 */
void 
Si7020GetVerFW(void) {
    BYTE CMD_VERFW[3]  = { 3, 0x84, 0xB8 };
    BYTE pData[1];

    SI7020SenCmdRead(CMD_VERFW, pData, 1, 0);
   
    DBG_SI7020_SEND_STR("SI7020 FW VER: ");
    DBG_SI7020_SEND_HEX(pData[0]);
    DBG_SI7020_SEND_STR("\n");
}

/**
 * @func   Si7020GetTemp
 * @brief  None
 * @param  None
 * @retval
 */
DWORD
Si7020GetTemp(void) {
    DWORD wRetval;
    BYTE pByRetval[3] = { 0 };
    BYTE byCheckCRC = 0;

    BYTE CMD_MEASURE_TEMP[2] =  { 2, 0xF3 };

    SI7020SenCmdRead(CMD_MEASURE_TEMP, pByRetval, 3, g_byTimeWaitGetTemp);
    byCheckCRC = CalCRC8(pByRetval, 2);

    /* Check CRC value */
    if (byCheckCRC == pByRetval[2]) {  
    }
    else {
        DBG_SI7020_SEND_STR("! ERR\n");
        return 0;
    }

    wRetval = (pByRetval[0] << 8) + pByRetval[1];
    wRetval = ((wRetval * 21965) >> 13) - 46850;

    DBG_SI7020_SEND_STR("~ Temp = ");
    DBG_SI7020_SEND_NUM(wRetval);
    DBG_SI7020_SEND_STR("\n");

    return wRetval; 
} 

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
DWORD
Si7020GetHumi(void) {
    DWORD wRetval;
    BYTE pByRetval[3] = { 0 };
    BYTE byCheckCRC = 0;

    BYTE CMD_MEASURE_RH[2] =  { 2, 0xF5 };

    SI7020SenCmdRead(CMD_MEASURE_RH, pByRetval, 3, g_byTimeWaitGetHumi);
    byCheckCRC = CalCRC8(pByRetval, 2);
    /* Check CRC value */
    if (byCheckCRC == pByRetval[2]) {  
    }
    else {
        DBG_SI7020_SEND_STR("! ERR\n");
        return 0;
    }

    wRetval = (pByRetval[0] << 8) + pByRetval[1];
    wRetval = ((wRetval * 15625) >> 13) - 6000;

    DBG_SI7020_SEND_STR("~ Humi = ");
    DBG_SI7020_SEND_NUM(wRetval);
    DBG_SI7020_SEND_STR("\n");

    return wRetval; 
} 

/**
 * @func   Si7020GetTempPre
 * @brief  None
 * @param  None
 * @retval
 */
DWORD 
Si7020GetTempPre(void) {
    DWORD wRetval;
    BYTE pByRetval[3] = { 0 };

    BYTE CMD_MEASURE_TEMP_PRE[2] =  { 2, 0xE0 };

    SI7020SenCmdRead(CMD_MEASURE_TEMP_PRE, pByRetval, 2, 0);

    wRetval = (pByRetval[0] << 8) + pByRetval[1];
    wRetval = ((wRetval * 21965) >> 13) - 46850;

    DBG_SI7020_SEND_STR("~ Temp priv= ");
    DBG_SI7020_SEND_NUM(wRetval);
    DBG_SI7020_SEND_STR("\n"); 

    return wRetval;    
}

/**
 * @func   Si7020SenCmdWrite
 * @brief  None
 * @param  None
 * @retval
 */
void
Si7020SenCmdWrite(
    BYTE byCmd,
    BYTE_p pData,
    BYTE byLenData
) {
    BYTE i;

    I2C_CLR_ARBL(I2C_SI7020);

    I2C_START(I2C_SI7020);
  
    I2C_WD(I2C_SI7020, SI7020_ADDR | WRITE);
    I2C_WAIT(I2C_SI7020);  

    I2C_WD(I2C_SI7020, byCmd);
    I2C_WAIT(I2C_SI7020);      

    for (i = 0; i < byLenData; i++) {
        I2C_WD(I2C_SI7020, pData[i]);
        I2C_WAIT(I2C_SI7020); 
    }

    I2C_STOP(I2C_SI7020);
    delay_ms(1);
}

/**
 * @func   SI7020SenCmdRead
 * @brief  None
 * @param  None
 * @retval
 */
void
SI7020SenCmdRead(
    BYTE_p pCmd,
    BYTE_p pDataRead,
    BYTE byLenData,
    WORD wDelay
) {
    BYTE i;
    BYTE byLenCmd = pCmd[0];

    I2C_CLR_ARBL(I2C_SI7020);

    I2C_START(I2C_SI7020);
  
    I2C_WD(I2C_SI7020, SI7020_ADDR | WRITE);
    I2C_WAIT(I2C_SI7020); 

    for (i = 1; i < byLenCmd; i++) {
        I2C_WD(I2C_SI7020, pCmd[i]);
        I2C_WAIT(I2C_SI7020); 
    }

    if (wDelay > 0) {
        delay_ms(wDelay);
    }

    I2C_RSTART(I2C_SI7020); 

    I2C_WD(I2C_SI7020, SI7020_ADDR | READ); 
    I2C_WAIT(I2C_SI7020);
 
    I2C_SetDirMode(I2C_SI7020, i2cReceiver);
    I2C_ACK(I2C_SI7020);

    /* Dummy read */
    I2C_RD(I2C_SI7020);

    if (byLenData <= 1) {
        I2C_NACK(I2C_SI7020);
        I2C_WAIT(I2C_SI7020);
        I2C_STOP(I2C_SI7020);
        pDataRead[0] = I2C_RD(I2C_SI7020); 
    } 
    else 
    {
        for (i = 0; i < byLenData - 1; i++) {
            I2C_WAIT(I2C_SI7020); 
            pDataRead[i] = I2C_RD(I2C_SI7020);
        }

        I2C_NACK(I2C_SI7020);
        I2C_WAIT(I2C_SI7020);
        I2C_STOP(I2C_SI7020);

        pDataRead[i] = I2C_RD(I2C_SI7020);
    }
  
    delay_ms(1);
}

/**
 * @func   
 * @brief  CRC-8 use x^8 + x^5 + x^4 + 1
 * @param  None
 * @retval None
 */
BYTE 
CalCRC8(
    BYTE_p pByData,
    BYTE byLength
) {
    BYTE i, j = 0;
    WORD wCrc = 0;
    BYTE_p pByCurrData = pByData;
    
    for (j = 0; j < byLength; j++, pByCurrData++) {
        wCrc ^= ((WORD)(*pByCurrData) << 8);
        for (i = 0; i < 8; i++) {
            if ((wCrc & 0x8000) != 0)
                wCrc ^= (0x1310 << 3);
            wCrc <<= 1;
        }
    }
    return ((wCrc >> 8) & 0xFF);
}
/* END FILE */
