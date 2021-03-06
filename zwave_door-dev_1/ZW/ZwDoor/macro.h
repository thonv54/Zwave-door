/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: TrungTQb
 *
 * Last Changed By:  $Author: trungtq $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date: 2016-05-20 10:20:00 (Fri, 20 May 2016) $
 *
 ******************************************************************************/
#ifndef _MACRO_H_
#define _MACRO_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "ZW_typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifndef BV
#define BV(n)                   (1 << (n))
#endif /* BV */

#ifndef max
#define MAX(n,m)            (((n) < (m)) ? (m) : (n))
#define MIN(n,m)            (((n) < (m)) ? (n) : (m))
#endif /* max */

#ifndef HI_UINT16
#define HI_UINT16(a)            (((a) >> 8) & 0xFF)
#define LO_UINT16(a)            ((a) & 0xFF)
#define HI_UINT8(a)             (((a) >> 4) & 0x0F)
#define LO_UINT8(a)             ((a) & 0x0F)
#endif /* HI_UINT16 */

#define GET_BIT_0_7(value)          ((BYTE)((value) & 0xFFU))
#define GET_BIT_8_15(value)         ((BYTE)(((value)>>8) & 0xFFU))
#define GET_BIT_16_23(value)        ((BYTE)(((value)>>16) & 0xFFU))
#define GET_BIT_24_31(value)        ((BYTE)((value)>>24))

#ifndef USE_ASSERT
#define assert_param(expr)   ((void)0)
#else /* USE_ASSERT */
#define assert_param(expr)   ((expr) ? (void)0 : assert_func (__FILE__, \
                                                              __LINE__, \
                                                              NULL, #expr))
#endif /* USE_ASSERT */

#define ASSERT(x)   assert_param(x)
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void
assert_func(
    const char *file,
    int line, 
    const char *func,
    const char *expr
);

#endif /* _TYPEDEFS_H_ */
