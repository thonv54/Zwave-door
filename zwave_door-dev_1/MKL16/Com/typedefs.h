/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: TrungTQ
 *
 * Last Changed By:  $ThangDH $
 * Revision:         $Revision:  $
 * Last Changed:     $Date: 5/2017 $
 *
 ******************************************************************************/
#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef unsigned char               BOOL;
typedef unsigned char               BYTE;   // 1 byte
typedef unsigned short              WORD;   // 2 byte
typedef unsigned long               DWORD;  // 4 byte

typedef BOOL*                       BOOL_p;
typedef BYTE*                       BYTE_p;
typedef WORD*                       WORD_p;
typedef DWORD*                      DWORD_p;

typedef void*                       void_p;
typedef void                        (*VOID_CALLBACKFUNC)(void);
typedef void                        (*BYTE_CALLBACKFUNC)(BYTE);
typedef void                        (*WORD_CALLBACKFUNC)(WORD);

typedef unsigned char               u8;       // 1 byte
typedef unsigned short              u16;      // 2 byte
typedef unsigned long               u32;      // 4 byte
typedef unsigned long long          u64;

typedef unsigned char               u8_t;     // 1 byte
typedef unsigned short              u16_t;    // 2 byte
typedef unsigned long               u32_t;    // 4 byte
typedef unsigned long long          u64_t;

typedef signed char                 i8;       // 1 byte
typedef signed short                i16;      // 2 byte
typedef signed long                 i32;      // 4 byte
typedef signed long long            i64;

typedef unsigned char*              u8_p;     // 1 byte
typedef unsigned short*             u16_p;    // 2 byte
typedef unsigned long*              u32_p;    // 4 byte
typedef unsigned long long*         u64_p; 

typedef signed char*                i8_p;     // 1 byte
typedef signed short*               i16_p;    // 2 byte
typedef signed long*                i32_p;    // 4 byte
typedef signed long long*           i64_p;
 
#ifndef TRUE
#define FALSE                       0
#define TRUE                        1
#endif // TRUE

#ifndef NULL
#define NULL                        0
#endif // NULL

#ifndef MAX_U8
#define MAX_U8                      0xFF
#define MAX_U16                     0xFFFF
#define MAX_U32                     0xFFFFFFFF
#endif // MAX_U8

typedef enum {
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;

typedef enum {
    RESET = 0, 
    SET = !RESET
} FlagStatus, ITStatus;

#define IS_FUNCTIONAL_STATE(STATE)  (((STATE) == DISABLE) || ((STATE) == ENABLE))

#define GET_BIT_0_7(value)          ((BYTE)((value) & 0xFFU))
#define GET_BIT_8_15(value)         ((BYTE)(((value)>>8) & 0xFFU))
#define GET_BIT_16_23(value)        ((BYTE)(((value)>>16) & 0xFFU))
#define GET_BIT_24_31(value)        ((BYTE)((value)>>24))

#define GET_BIT_0_15(value)         ((WORD)((value) & 0xFFFFU))
#define GET_BIT_16_31(value)        ((WORD)(((value)>>16) & 0xFFFFU))

#define JOIN_BIT_U16(x,y)           ((DWORD)((x<<16) + (y)))
#define JOIN_BIT_U8(x,y,z,t)        ((DWORD)((x<<24) + (y<<16) + (z<<8) + (t)))

#define READ32(address)             ((DWORD)(*(DWORD*)(address)))
#define READ16(address)             ((WORD)(*(WORD*)(address)))
#define READ8(address)              ((BYTE)(*(BYTE*)(address)))
#define REG_WRITE(address, value)   (*(vuint8_t*)(address) = (value))

#ifndef BIT
#define BIT(x) (1U << (x))  // Unsigned avoids compiler warnings re BIT(15)
#endif
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

#endif /* _TYPEDEFS_H_ */
