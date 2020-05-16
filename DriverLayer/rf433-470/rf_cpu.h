#ifndef  RF_CPU_H
#define  RF_CPU_H

#define  RF_CPU_EXT  extern
	
#include "stm32f10x.h"

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/
typedef unsigned char  rf_uint8;                /* Unsigned  8 bit quantity                           */
typedef signed	 char  rf_int8;                 /* Signed    8 bit quantity                          */
typedef unsigned short rf_uint16;               /* Unsigned 16 bit quantity                           */
typedef          short rf_int16;                /* Signed   16 bit quantity                           */
typedef unsigned int   rf_uint32;               /* Unsigned 32 bit quantity                           */
typedef 	       int   rf_int32;                /* Signed   32 bit quantity                           */
typedef float          rf_fp32;                 /* Single precision floating point                    */
typedef double         rf_fp64;                 /* Double precision floating point                    */
#define rf__IO         volatile 
	
/* Bits */
#define BIT0            0x01
#define BIT1            0x02
#define BIT2            0x04
#define BIT3            0x08
#define BIT4            0x10
#define BIT5            0x20
#define BIT6            0x40
#define BIT7            0x80

#endif
