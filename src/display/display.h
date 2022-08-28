/******************************************************************
 *
 *
 * FILE        : display.h
 * PROJECT     : chip8
 * AUTHOR      : danpham
 * DESCRIPTION : Screen implementation of the chip8
 *
 ******************************************************************/

/******************************************************************
 * 1. Included files (microcontroller ones then user defined ones)
 ******************************************************************/
#include <SDL2/SDL.h>
#include "../headers/typedef.h"

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
extern void DisplayInit();
extern void DisplayUpdate();
extern void DisplayExit();
extern void DisplayClearScreen();
extern void DisplayDraw(U8 *memory, U8 x, U8 y, U8 n);