/******************************************************************
 * 
 *
 * FILE        : typedef.h
 * PROJECT     : chip8
 * AUTHOR      : danpham
 * DESCRIPTION : Generic types for embedded C.
 *
 ******************************************************************/
#ifndef TYPEDEF_H_
#define TYPEDEF_H_

/******************************************************************
 * 1. Included files (microcontroller ones then user defined ones)
 ******************************************************************/
#include <stdbool.h>

/******************************************************************
 * 2. Define declarations (macros then function macros)
 ******************************************************************/

/* Boolean definition */
#define TRUE                                                     1U
#define FALSE                                                    0U

/******************************************************************
 * 3. Typedef definitions (simple typedef, then enum and structs)
 ******************************************************************/

/* Std_ReturnType - Standard return type of the functions */
typedef enum {
    E_OK,      /* The function returns with no error */
    E_NOT_OK   /* The function returns with an error */
} Std_ReturnType;

typedef enum {
    E_BITCLEAR = 0U,
    E_BITSET   = 1U
} Std_BitType;

/* 8 bits typedefs */
typedef           unsigned char U8;
typedef volatile unsigned char VU8;
typedef           signed char S8;
typedef volatile signed char VS8;

/* 16 bits typedefs */
typedef           unsigned short U16;
typedef  volatile unsigned short VU16;
typedef           signed short S16;
typedef  volatile signed short VS16;

/* 32 bits typedefs */
typedef           unsigned long U32;
typedef  volatile unsigned long VU32;
typedef           signed long S32;
typedef  volatile signed long VS32;

/* 1 bit typedefs */
typedef           unsigned char BOOL;

#endif /* TYPEDEF_H_ */