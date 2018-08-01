/****************************************************************************
 *
 * Copyright (c) 2017
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: ThangDH
 *
 * Last Changed By:  Author: ThangDH 
 * Revision:         Revision: 1.1
 * Last Changed:     Date:  AM 
 *
 ****************************************************************************/

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include "configapp.h"
#include "configboard.h"
#include "opt3001.h"
#include "i2c.h"
#include "debug.h"
/****************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                       */
/****************************************************************************/
#ifdef OPT3001_DEBUG
#define DBG_OPT3001_SEND_STR(x)     Debug_SendStr(x)
#define DBG_OPT3001_SEND_NUM(x)     Debug_SendNum(x)
#define DBG_OPT3001_SEND_HEX(x)     Debug_SendHex(x)
#else
#define DBG_OPT3001_SEND_STR(x)
#define DBG_OPT3001_SEND_NUM(x)
#define DBG_OPT3001_SEND_HEX(x)
#endif

#define OPT3001_ADDR_GND            0x88
#define OPT3001_ADDR_VDD            0x8A 
#define OPT3001_ADDR_SDA            0x8C 
#define OPT3001_ADDR_SCL            0x8E 
#define REG_RESULT                  0x00
#define REG_CONFIG                  0x01
#define REG_LOWLIM                  0x02
#define REG_HIGHLIM                 0x03
#define REG_MANUID                  0x7E
#define REG_DEVID                   0x7F
#define CONFIG_RN_MASK              (0xF << 12) /* [15..12] Range Number */
#define CONFIG_RN_AUTO              (0xC << 12)
#define CONFIG_CT                   BIT(11) /* [11] Conversion Time */
#define CONFIG_MC_MASK              (0x3 << 9) /* [10..9] Mode of Conversion */
#define CONFIG_MC_SHUTDOWN          (0x0 << 9)
#define CONFIG_MC_SINGLE            (0x1 << 9)
#define CONFIG_MC_CONTINUOS         (0x2 << 9)
#define CONFIG_OVF                  BIT(8) /* [8] Overflow */
#define CONFIG_CRF_SHIFT            7 /* [7] Conversion Ready Field */
#define CONFIG_CRF_MASK             0x80 /* [7] Conversion Ready Field */
#define CONFIG_FH                   BIT(6) /* [6] Flag High */
#define CONFIG_FL                   BIT(5) /* [5] Flag Low */
#define CONFIG_L                    BIT(4) /* [4] Latch */
#define CONFIG_POL                  BIT(3) /* [3] Polarity */
#define CONFIG_ME                   BIT(2) /* [2] Mask Exponent */
#define CONFIG_FC_MASK              (0x3 << 0)/* [1..0] Fault Count */
#define STATE_SLEEPING              0
#define STATE_ACTIVE                1
#define STATE_DATA_READY            2
#define GET_EXP(n)                  ((n) >> 12)
#define GET_MATISSA(n)              ((n) & 0xFFFF)
#define OPT3001_ADDR                OPT3001_ADDR_GND
#define SHORT_TIME_CONVERT          120
#define LONG_TIME_CONVERT           900 

typedef struct _OPT3001_H_ {
    WORD mode;
    WORD low_mantissa;
    WORD high_mantissa;
    WORD low_exp;
    WORD high_exp;
    WORD lux;
    WORD time;
} OPT3001_t, *OPT3001_p;
/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/
static OPT3001_t dev_opt3001;
static void Opt3001WriteReg(BYTE regAddr, WORD wDataWrite);
static void Opt3001ReadReg(BYTE regAddr, WORD_p pwDataRead);
/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/

/****************************************************************************/
/*                            PRIVATE FUNCTIONS                             */
/****************************************************************************/

/****************************************************************************/
/*                            EXPORTED FUNCTIONS                            */
/****************************************************************************/

/** 
 * @func   Opt3001Init
 * @brief  None
 * @param  None
 * @retval None
 */
void
Opt3001Init(void) {
    dev_opt3001.high_exp = 0;
    dev_opt3001.high_mantissa = 0;
    dev_opt3001.low_exp = 0;
    dev_opt3001.low_mantissa = 0;
    dev_opt3001.lux = 0;
    dev_opt3001.mode = 0xC810;
    dev_opt3001.time = LONG_TIME_CONVERT;

    Opt3001Configure();
    Opt3001SetTimeConvert(LONG_TIME_CONVERT);
    Opt3001GetManufactorID();
    Opt3001GetDeviceID();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
void
Opt3001Configure(void) {
    WORD wRegConfig;

    Opt3001ReadReg(REG_CONFIG, &wRegConfig);

    wRegConfig &= ~CONFIG_RN_MASK;
    wRegConfig |= CONFIG_RN_AUTO;     /* Range Auto */
    
    wRegConfig &= ~CONFIG_MC_MASK;
    wRegConfig |= CONFIG_MC_SHUTDOWN; /* Mode Shutdown */

    wRegConfig |= CONFIG_L;           /* Latch Field */
    wRegConfig &= ~(CONFIG_POL | CONFIG_ME | CONFIG_FC_MASK);

    if (CONFIG_CT == (wRegConfig & CONFIG_CT))
        dev_opt3001.time = LONG_TIME_CONVERT;
    else 
        dev_opt3001.time = SHORT_TIME_CONVERT;
    
    dev_opt3001.mode = CONFIG_MC_SHUTDOWN;
 
    Opt3001WriteReg(REG_CONFIG, wRegConfig);
    /* Store Low Limit */
    Opt3001ReadReg(REG_LOWLIM, &wRegConfig);
    dev_opt3001.low_exp = GET_EXP(wRegConfig);
    dev_opt3001.low_mantissa = GET_MATISSA(wRegConfig);
    /* Store High Limit */
    Opt3001ReadReg(REG_HIGHLIM, &wRegConfig);
    dev_opt3001.high_exp = GET_EXP(wRegConfig);
    dev_opt3001.high_mantissa = GET_MATISSA(wRegConfig);
}

/**
 * @func   Opt3001GetManufactorID
 * @brief  None
 * @param  None
 * @retval ManufactorID
 */
WORD
Opt3001GetManufactorID(void) {
    WORD data = 0;

    Opt3001ReadReg(REG_MANUID, &data);
    DBG_OPT3001_SEND_STR("OPT3001 MANUFACTUREID: ");
    DBG_OPT3001_SEND_NUM(data);
    DBG_OPT3001_SEND_STR("\n");

    return data;
}

/** 
 * @func   Opt3001GetDeviceID
 * @brief  None
 * @param  None
 * @retval DeviceID
 */
WORD
Opt3001GetDeviceID(void) {
    WORD data = 0;

    Opt3001ReadReg(REG_DEVID, &data);
    DBG_OPT3001_SEND_STR("OPT3001 DEVICEID: ");
    DBG_OPT3001_SEND_NUM(data);
    DBG_OPT3001_SEND_STR("\n");

    return data;
}

/**
 * @func   Opt3001GetLux
 * @brief  None
 * @param  None
 * @retval LUX
 */
DWORD
Opt3001GetLux(void) {
    WORD LSB_Size;
    WORD wResult;
    WORD wRegConfig;
    DWORD wLux;

    Opt3001ReadReg(REG_CONFIG, &wRegConfig);

    wRegConfig &= ~CONFIG_MC_MASK;
    wRegConfig |= CONFIG_MC_SINGLE; /* Mode Single Shot */
    dev_opt3001.mode = CONFIG_MC_SINGLE;

    /* Set Configuration Register  */
    Opt3001WriteReg( REG_CONFIG, wRegConfig);
    /* Wait for conversion complete */
    delay_ms(dev_opt3001.time);

    /* Read Configuration Register */
    Opt3001ReadReg(REG_CONFIG, &wRegConfig);
    dev_opt3001.mode = CONFIG_MC_SHUTDOWN;

    if (CONFIG_CRF_MASK != (wRegConfig & CONFIG_CRF_MASK)) {
        wResult = 0;
    }
    else 
    {   /* If complete conversion */
        Opt3001ReadReg(REG_RESULT, &wResult);
    }

    LSB_Size = 1 << (u8_t) ((wResult >> 12) & 0x000f); // LSB_Size = 2 ^ exponent
    wLux = (u32_t) ((wResult & 0x0fff) * LSB_Size);

    DBG_OPT3001_SEND_STR("~ Lux = ");
    DBG_OPT3001_SEND_NUM(wLux);
    DBG_OPT3001_SEND_STR("\n");
    if (wLux > 10000000) wLux = 0xFFFFFFFF; /* Sensor Fail */
    return wLux;
}

/**
 * @func   Opt3001SetTimeConvert
 * @brief  None
 * @param  None
 * @retval None
 */
void
Opt3001SetTimeConvert(
    WORD wTime
) {
    WORD wRegTemp;

    Opt3001ReadReg(REG_CONFIG, &wRegTemp);

    dev_opt3001.time = wTime;

    if (LONG_TIME_CONVERT == wTime)
        wRegTemp |= CONFIG_CT;
    else if (SHORT_TIME_CONVERT == wTime)
        wRegTemp &= ~CONFIG_CT;
  
    Opt3001WriteReg(REG_CONFIG, wRegTemp);    
}

/**
 * @func   Opt3001StartMeasure
 * @brief  None
 * @param  None
 * @retval None
 */
void
Opt3001StartMeasure(void) {
    WORD wRegConfig;

    Opt3001ReadReg(REG_CONFIG, &wRegConfig);
//    DBG_OPT3001_SEND_STR("CFG:");
//    DBG_OPT3001_SEND_NUM(wRegConfig);
//    DBG_OPT3001_SEND_STR("\n");

    wRegConfig &= ~CONFIG_MC_MASK;
    wRegConfig |= CONFIG_MC_SINGLE; /* Mode Single Shot */
    dev_opt3001.mode = CONFIG_MC_SINGLE;

    /* Set Configuration Register  */
    Opt3001WriteReg( REG_CONFIG, wRegConfig);
}

/**
 * @func   Opt3001ReadLux
 * @brief  None
 * @param  None
 * @retval LUX
 */
DWORD
Opt3001ReadLux(void) {
    WORD LSB_Size;
    WORD wResult;
    WORD wRegConfig;
    DWORD wLux;

    Opt3001ReadReg(REG_CONFIG, &wRegConfig);
//    DBG_OPT3001_SEND_STR("R CFG:");
//    DBG_OPT3001_SEND_NUM(wRegConfig);
//    DBG_OPT3001_SEND_STR("\n");

    if (CONFIG_CRF_MASK != (wRegConfig & CONFIG_CRF_MASK)) {
        wResult = 0;
    }
    else 
    {   /* If complete conversion */
        Opt3001ReadReg(REG_RESULT, &wResult);
    }

    LSB_Size = 1 << (u8_t) ((wResult >> 12) & 0x000f); // LSB_Size = 2 ^ exponent
    wLux = (u32_t) ((wResult & 0x0fff) * LSB_Size);

    DBG_OPT3001_SEND_STR("~ LUX = ");
    DBG_OPT3001_SEND_NUM(wLux);
    DBG_OPT3001_SEND_STR("\n");
    if (wLux > 10000000) wLux = 0xFFFFFFFF; /* Sensor Fail */
    return wLux;

}

/**
 * @func   Opt3001ReadReg
 * @brief  None
 * @param  None
 * @retval
 */
void
Opt3001ReadReg(
    BYTE regAddr,
    WORD_p pwDataRead
) {
    BYTE byteMsb = 0, byteLsb = 0;

    I2C_CLR_ARBL(I2C_OPT3001);                    /* */
    I2C_START(I2C_OPT3001);                       /* Start */
    I2C_WD(I2C_OPT3001, OPT3001_ADDR | WRITE);    /* Addr slave + write */
    I2C_WAIT(I2C_OPT3001);                        /* */
    I2C_WD(I2C_OPT3001, regAddr);                 /* Reg addr */
    I2C_WAIT(I2C_OPT3001);                        /* */
    I2C_RSTART(I2C_OPT3001);                      /* Repeated start */
    I2C_WD(I2C_OPT3001, OPT3001_ADDR | READ);     /* Read register from slave */
    I2C_WAIT(I2C_OPT3001);                        /* */
    I2C_SetDirMode(I2C_OPT3001, i2cReceiver);     /* Rx mode */
    I2C_ACK(I2C_OPT3001);                         /* */
    I2C_RD(I2C_OPT3001);                          /* Dummy read */
    I2C_WAIT(I2C_OPT3001);                        /* */
    I2C_NACK(I2C_OPT3001);                        /* */
    byteMsb = I2C_RD(I2C_OPT3001);                /* Read byte Msb */
    I2C_WAIT(I2C_OPT3001);                        /* */
    I2C_STOP(I2C_OPT3001);                        /* Send stop since about to read last byte */
    byteLsb = I2C_RD(I2C_OPT3001);                /* Read byte Lsb */
    *pwDataRead = (byteMsb << 8) + byteLsb;       /* */
    delay_ms(1);
}

/**
 * @func   Opt3001WriteReg
 * @brief  None
 * @param  None
 * @retval Opt3001WriteReg
 */
void
Opt3001WriteReg(
    BYTE regAddr,
    WORD wDataWrite
) {
    I2C_CLR_ARBL(I2C_OPT3001);

    I2C_START(I2C_OPT3001);

    I2C_WD(I2C_OPT3001, OPT3001_ADDR | WRITE);
    I2C_WAIT(I2C_OPT3001);

    I2C_WD(I2C_OPT3001, regAddr);
    I2C_WAIT(I2C_OPT3001);

    I2C_WD(I2C_OPT3001, (wDataWrite >> 8) & 0xFF);
    I2C_WAIT(I2C_OPT3001);   
 
    I2C_WD(I2C_OPT3001, (wDataWrite & 0xFF));
    I2C_WAIT(I2C_OPT3001);  
  
    I2C_STOP(I2C_OPT3001);

    delay_ms(1);
}

/* END FILE */
