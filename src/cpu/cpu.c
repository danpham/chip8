/******************************************************************
 *
 *
 * FILE        : cpu.c
 * PROJECT     : chip8
 * AUTHOR      : danpham
 * DESCRIPTION : Cpu emulator
 *
 ******************************************************************/

/******************************************************************
 * 1. Included files (microcontroller ones then user defined ones)
 ******************************************************************/
#include <string.h>
#include "cpu.h"
#include "../import/import.h"

/******************************************************************
 * 2. Define declarations (macros then function macros)
 ******************************************************************/

/******************************************************************
 * 3. Typedef definitions (simple typedef, then enum and structs)
 ******************************************************************/
typedef struct
{
    U8 memory[CPU_MEMORY_SIZE];
    U8 i;
    U8 vx[CPU_NUMBER_OF_VX_REGISTER];
    U16 pc;
    U16 stack[CPU_STACK_DEPTH_LEVEL];
    U8 stackLevel;
    U8 sysCounter;
    U8 soundCounter;
} cpuType;

/******************************************************************
 * 4. Variable definitions (static then global)
 ******************************************************************/
static cpuType s_cpu;

/******************************************************************
 * 5. Functions prototypes (static only)
 ******************************************************************/
static void cpuCounters(void);

/******************************************************************
 * FUNCTION : CpuInit()
 *    Description: Initialize cpu
 *    Parameters:  None
 *    Return:      E_OK if initialization succeed, E_NOT_OK
 *                 otherwise.
 ******************************************************************/
Std_ReturnType CpuInit(void)
{
    cpuType *returnPtr = NULL;
    Std_ReturnType returnValue = E_NOT_OK;

    returnPtr = (cpuType *)memset((void *)&s_cpu, 0U, sizeof(cpuType));

    if (&s_cpu == returnPtr)
    {
        s_cpu.pc = CPU_START_ADDRESS;

        /* Load ROM */
        returnValue = ImportRom(s_cpu.memory);
    }

    return returnValue;
}

/******************************************************************
 * FUNCTION : cpuCounters()
 *    Description: Handle time counters
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
static void cpuCounters(void)
{
    if (s_cpu.sysCounter > 0)
    {
        s_cpu.sysCounter--;
    }

    if (s_cpu.soundCounter > 0)
    {
        s_cpu.soundCounter--;
    }

    return;
}

/******************************************************************
 * FUNCTION : CpuMain()
 *    Description: Main cpu loop
 *    Parameters:  None
 *    Return:      E_OK if loop succeed, E_NOT_OK
 *                 otherwise.
 ******************************************************************/
Std_ReturnType CpuMain(void)
{
    /* Update cpu counters */
    cpuCounters();

    return E_OK;
}