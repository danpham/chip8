/******************************************************************
 *
 *
 * FILE        : import.c
 * PROJECT     : chip8
 * AUTHOR      : danpham
 * DESCRIPTION : Rom file import
 *
 ******************************************************************/

/******************************************************************
 * 1. Included files (microcontroller ones then user defined ones)
 ******************************************************************/
#include <stdio.h>
#include "import.h"
#include "../cpu/cpu.h"

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
 * FUNCTION : ImportRom()
 *    Description: Import a chip8 rom
 *    Parameters:  memory
 *    Return:      E_OK if import succeed, E_NOT_OK
 *                 otherwise.
 ******************************************************************/
Std_ReturnType ImportRom(U8 *memory)
{
    Std_ReturnType returnValue = E_OK;
    FILE *filePtr;

    if (NULL == (filePtr = fopen("build/IBMLogo.ch8", "rb")))
    {
        printf("Unable to open file.");
        returnValue = E_NOT_OK;
    }
    else
    {
        /* Load memory starting from CPU_START_ADDRESS index (CPU_START_ADDRESS - 1) */
        fread(&memory[CPU_START_ADDRESS], 1U, CPU_MAX_PROGRAM_SIZE, filePtr);
        fclose(filePtr);
    }

    return returnValue;
}