/******************************************************************
 *
 *
 * FILE        : input.c
 * PROJECT     : chip8
 * AUTHOR      : danpham
 * DESCRIPTION : Input keyboard
 *
 ******************************************************************/

/******************************************************************
 * 1. Included files (microcontroller ones then user defined ones)
 ******************************************************************/
#include <stdio.h>
#include "input.h"

/******************************************************************
 * 2. Define declarations (macros then function macros)
 ******************************************************************/

/******************************************************************
 * 3. Typedef definitions (simple typedef, then enum and structs)
 ******************************************************************/
typedef struct
{
    SDL_Keycode keyboardMapping[INPUT_NUMBER_OF_KEYBOARD_KEYS];
    BOOL keyboardStatus[INPUT_NUMBER_OF_KEYBOARD_KEYS];
} inputType;

/******************************************************************
 * 4. Variable definitions (static then global)
 ******************************************************************/
inputType s_input;

/******************************************************************
 * 5. Functions prototypes (static only)
 ******************************************************************/



/******************************************************************
 * FUNCTION : InputInit()
 *    Description: Init keyboard mapping
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
void InputInit()
{
    s_input.keyboardMapping[0U] = SDLK_KP_0;
    s_input.keyboardMapping[1U] = SDLK_KP_1;
    s_input.keyboardMapping[2U] = SDLK_KP_2;
    s_input.keyboardMapping[3U] = SDLK_KP_3;
    s_input.keyboardMapping[4U] = SDLK_KP_4;
    s_input.keyboardMapping[5U] = SDLK_KP_5;
    s_input.keyboardMapping[6U] = SDLK_KP_6;
    s_input.keyboardMapping[7U] = SDLK_KP_7;
    s_input.keyboardMapping[8U] = SDLK_KP_8;
    s_input.keyboardMapping[9U] = SDLK_KP_9;
    s_input.keyboardMapping[10U] = SDLK_q;
    s_input.keyboardMapping[11U] = SDLK_w;
    s_input.keyboardMapping[12U] = SDLK_e;
    s_input.keyboardMapping[13U] = SDLK_r;
    s_input.keyboardMapping[14U] = SDLK_t;
    s_input.keyboardMapping[15U] = SDLK_z;

    /* At startup all keys are up */
    memset(s_input.keyboardStatus, (S32)FALSE, INPUT_NUMBER_OF_KEYBOARD_KEYS);
}

/******************************************************************
 * FUNCTION : InputUpdateKeyboardUp()
 *    Description: Set the key that is up
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
void InputUpdateKeyboardUp(SDL_Keycode sym)
{
    U8 i;

    for (i = 0U; i < INPUT_NUMBER_OF_KEYBOARD_KEYS; i++)
    {
        if (sym == s_input.keyboardMapping[i])
        {
            s_input.keyboardStatus[i] = FALSE;
            break;
        }
    }
}

/******************************************************************
 * FUNCTION : InputUpdateKeyboardDown()
 *    Description: Set the key that is up
 *    Parameters:  None
 *    Return:      key pressed
 ******************************************************************/
U8 InputUpdateKeyboardDown(SDL_Keycode sym)
{
    U8 i;

    for (i = 0U; i < INPUT_NUMBER_OF_KEYBOARD_KEYS; i++)
    {
        if (sym == s_input.keyboardMapping[i])
        {
            s_input.keyboardStatus[i] = TRUE;
            break;
        }
    }

    return i;
}

/******************************************************************
 * FUNCTION : InputKeyboardStatus()
 *    Description: Return keyboard status
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
BOOL * InputKeyboardStatus()
{
    return s_input.keyboardStatus;
}

/******************************************************************
 * FUNCTION : InputWaitKeyboardPressed()
 *    Description: Wait for a keyboard key to be pressed
 *    Parameters:  None
 *    Return:      Value of key pressed
 ******************************************************************/
U8 InputWaitKeyboardPressed()
{
    BOOL keyPressed = FALSE;
    U8 keyIndex = 0U;
    SDL_Event event;

    while (TRUE != keyPressed)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                keyIndex = InputUpdateKeyboardDown(event.key.keysym.sym);
                break;
            }
        }

        SDL_Delay(1U);
    }

    return keyIndex;
}