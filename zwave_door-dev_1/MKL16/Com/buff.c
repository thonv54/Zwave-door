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
#include "buff.h"
#include "utilities.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

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
 * @func   bufInit
 * @brief  None
 * @param  None
 * @retval None
 */
void
bufInit(
    void_p pBuffer, 
    buffqueue_p pQueue, 
    BYTE sizeofElement, 
    BYTE numberOfElement
) {
    pQueue->wSize = numberOfElement;
    pQueue->eleType = sizeofElement;
    pQueue->pData = (BYTE_p)pBuffer;
    bufErase(pQueue);
}

/**
 * @func   bufClear
 * @brief  None
 * @param  None
 * @retval None
 */
void
bufClear(
    buffqueue_p buf
) {
    buf->wCountEle = 0;
    buf->in = 0;
    buf->out = 0;
}

/**
 * @func   bufNumEle
 * @brief  None
 * @param  None
 * @retval 
 */
WORD 
bufNumEle(
    buffqueue_p buf
) {
    return buf->wCountEle;
}

/**
 * @func   bufIsFull
 * @brief  None
 * @param  None
 * @retval
 */
BOOL
bufIsFull(
    buffqueue_p buf
) {
    if (buf->wCountEle >= buf->wSize) return TRUE;
    else return FALSE;   
}

/**
 * @func   bufIsEmpty
 * @brief  None
 * @param  None
 * @retval
 */
BOOL
bufIsEmpty(
    buffqueue_p buf
) {
    if (buf->wCountEle <= 0) return TRUE;
    else return FALSE;  
}

/**
 * @func   bufErase
 * @brief  None
 * @param  None
 * @retval None
 */
void
bufErase(
    buffqueue_p buf
) {
    bufClear(buf);
    for (WORD i = 0; i < buf->wSize; i++) buf->pData[i] = 0;
//    memset((void*)buf->pData, 0, buf->wSize); /* more flash size */
}

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
) {
    if (buf->wCountEle >= buf->wSize) {
        return ERR_BUF_FUL;
    }
    else {
        BYTE i;
        BYTE_p pEl = pElement;
        BYTE_p pAr = buf->pData + (buf->in)*(buf->eleType); /* point to element in array */
        for (i = 0; i < buf->eleType; i++) {
            *pAr++ = *pEl++;
        }

        buf->in = (buf->in +1) % buf->wSize;
        buf->wCountEle++;
    }

    return ERR_OK;
}

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
) {
    if (bufNumEle(buf) <= 0) {
        return ERR_BUF_EMPTY;
    }
    else {
        BYTE i;
        BYTE_p pEl = pElement;
        BYTE_p pAr = buf->pData + (buf->out) * (buf->eleType);
        for (i = 0; i < buf->eleType; i++) {
            *pEl++ = *pAr++;
        }
 
        buf->out = (buf->out + 1) % buf->wSize;
        buf->wCountEle--;
    } 
 
    return ERR_OK;
}

/* END FILE */
