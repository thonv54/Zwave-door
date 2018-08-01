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
#ifndef _BUFF_H_
#define _BUFF_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "macro.h"
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define ERR_OK                         0x00 
#define ERR_BUF_FUL                    0x01
#define ERR_BUF_EMPTY                  0x02     
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
typedef struct __buff_queue__ {
    WORD wSize;
    WORD wCountEle;
    WORD in;
    WORD out;
    BYTE eleType;
    BYTE_p pData;
} buffqueue, *buffqueue_p;
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
 * @func   bufInit
 * @brief  None
 * @param  None
 * @retval 
 */
void
bufInit(
    void_p pBuffer, 
    buffqueue_p pQueue, 
    BYTE sizeofElement, 
    BYTE numberOfElement
);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval
 */
WORD 
bufNumEle(
    buffqueue_p buf
);

/**
 * @func   bufClear
 * @brief  None
 * @param  None
 * @retval
 */
void
bufClear(
    buffqueue_p buf
);

/**
 * @func   bufIsFull
 * @brief  None
 * @param  None
 * @retval
 */
BOOL
bufIsFull(
    buffqueue_p buf
);

/**
 * @func   bufIsEmpty
 * @brief  None
 * @param  None
 * @retval
 */
BOOL
bufIsEmpty(
    buffqueue_p buf
);

/**
 * @func   bufErase
 * @brief  None
 * @param  None
 * @retval
 */
void
bufErase(
    buffqueue_p buf
);

/**
 * @func   bufEnDat
 * @brief  None
 * @param  None
 * @retval 
 */
BYTE 
bufEnDat(
    buffqueue_p buf,
    void_p pElement
);

/**
 * @func   bufDeDat
 * @brief  None
 * @param  None
 * @retval
 */
BYTE
bufDeDat(
    buffqueue_p buf,   
    void_p pElement
);

#endif /* END FILE */
