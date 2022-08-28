/******************************************************************
 *
 *
 * FILE        : main.c
 * PROJECT     : chip8
 * AUTHOR      : danpham
 * DESCRIPTION : Chip8 emulator
 *
 ******************************************************************/

/******************************************************************
 * 1. Included files (microcontroller ones then user defined ones)
 ******************************************************************/
#include <stdio.h>
#include <SDL2/SDL.h>
#include "cpu/cpu.h"
#include "display/display.h"

/******************************************************************
 * 2. Define declarations (macros then function macros)
 ******************************************************************/

/******************************************************************
 * 3. Typedef definitions (simple typedef, then enum and structs)
 ******************************************************************/

/******************************************************************
 * 4. Variable definitions (static then global)
 ******************************************************************/

/******************************************************************
 * 5. Functions prototypes (static only)
 ******************************************************************/

/******************************************************************
 * FUNCTION : main(int argv, char** args)
 *    Description: main
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
int main(int argv, char **args)
{
    /* Init Cpu */
    (void)CpuInit();

    DisplayInit();

    DisplayUpdate();

    DisplayExit();

    return 0;
}
