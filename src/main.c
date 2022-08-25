/******************************************************************
 * 
 *
 * FILE        : main.c
 * PROJECT     : 
 * AUTHOR      : 
 * DESCRIPTION : Chip8 emulator
 *
 ******************************************************************/

/******************************************************************
 * 1. Included files (microcontroller ones then user defined ones)
 ******************************************************************/
#include <SDL2/SDL.h>
#include "cpu/cpu.h"

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
int main(int argv, char** args)
{
    /* Init Cpu */
    (void)CpuInit();

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Chip8 emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    BOOL isRunning = TRUE;
    SDL_Event event;

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = FALSE;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    isRunning = FALSE;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderPresent(renderer);

        /* Run Cpu main loop */
        (void)CpuMain();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
