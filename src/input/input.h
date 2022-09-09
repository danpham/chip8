/******************************************************************
 * 
 *
 * FILE        : input.h
 * PROJECT     : chip8
 * AUTHOR      : danpham
 * DESCRIPTION : Input keyboard
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
#define INPUT_NUMBER_OF_KEYBOARD_KEYS                            16U

/******************************************************************
 * 3. Typedef definitions (simple typedef, then enum and structs)
 ******************************************************************/

/******************************************************************
 * 4. Variable definitions (static then global)
 ******************************************************************/

/******************************************************************
 * 5. Functions prototypes (static only)
 ******************************************************************/
extern void InputInit();
extern void InputUpdateKeyboardUp(SDL_Keycode sym);
extern U8 InputUpdateKeyboardDown(SDL_Keycode sym);
extern BOOL * InputKeyboardStatus();
extern U8 InputWaitKeyboardPressed();