/******************************************************************
 * 
 *
 * FILE        : cpu.h
 * PROJECT     : chip8
 * AUTHOR      : danpham
 * DESCRIPTION : Cpu emulator
 *
 ******************************************************************/

/******************************************************************
 * 1. Included files (microcontroller ones then user defined ones)
 ******************************************************************/
#include "../headers/typedef.h"

/******************************************************************
 * 2. Define declarations (macros then function macros)
 ******************************************************************/
#define CPU_MEMORY_SIZE                                        4096U
#define CPU_NUMBER_OF_VX_REGISTER                                16U
#define CPU_STACK_DEPTH_LEVEL                                    16U
#define CPU_START_ADDRESS                                     0x200U
#define CPU_MAX_PROGRAM_SIZE     CPU_MEMORY_SIZE - CPU_START_ADDRESS
/******************************************************************
 * 3. Typedef definitions (simple typedef, then enum and structs)
 ******************************************************************/

/******************************************************************
 * 4. Variable definitions (static then global)
 ******************************************************************/

/******************************************************************
 * 5. Functions prototypes (static only)
 ******************************************************************/
extern Std_ReturnType CpuInit(void);
extern Std_ReturnType CpuMain(void);
