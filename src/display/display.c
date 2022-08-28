/******************************************************************
 *
 *
 * FILE        : display.c
 * PROJECT     : chip8
 * AUTHOR      : danpham
 * DESCRIPTION : Screen implementation of the chip8
 *
 ******************************************************************/

/******************************************************************
 * 1. Included files (microcontroller ones then user defined ones)
 ******************************************************************/
#include <stdio.h>
#include "display.h"
#include "../cpu/cpu.h"

/******************************************************************
 * 2. Define declarations (macros then function macros)
 ******************************************************************/
#define DISPLAY_HEIGHT                                           32U
#define DISPLAY_WIDTH                                            64U

#define DISPLAY_PIXEL_HEIGH_IN_PIXELS                             8U
#define DISPLAY_PIXEL_WIDTH_IN_PIXELS                             8U
#define DISPLAY_HEIGHT_SIZED                                        DISPLAY_HEIGHT * DISPLAY_PIXEL_HEIGH_IN_PIXELS
#define DISPLAY_WIDTH_SIZED                                         DISPLAY_WIDTH * DISPLAY_PIXEL_WIDTH_IN_PIXELS
#define DISPLAY_GET_BIT_MASK                                    0x80

/******************************************************************
 * 3. Typedef definitions (simple typedef, then enum and structs)
 ******************************************************************/
typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    BOOL screen[DISPLAY_WIDTH][DISPLAY_HEIGHT];
} displayType;

/******************************************************************
 * 4. Variable definitions (static then global)
 ******************************************************************/
static displayType display;
static void displayUpdate(void);
static BOOL displayGetBit(U8 byte, U8 n);

/******************************************************************
 * 5. Functions prototypes (static only)
 ******************************************************************/

/******************************************************************
 * FUNCTION : DisplayInit()
 *    Description: Import a chip8 rom
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
void DisplayInit()
{
    U8 i;
    U8 j;

    SDL_Init(SDL_INIT_EVERYTHING);

    display.window = SDL_CreateWindow("Chip8 emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH_SIZED, DISPLAY_HEIGHT_SIZED, 0);

    if (NULL == display.window)
    {
        printf("Error SDL_CreateWindow");
    }

    /* Initialize screen */
    for (i = 0U; i < DISPLAY_WIDTH; i++)
    {
        for (j = 0U; j < DISPLAY_HEIGHT; j++)
        {
            if (((i % 2U) == 0U) && ((j % 2U) == 0U))
            {
                display.screen[i][j] = FALSE;
            }
            else
            {
                display.screen[i][j] = TRUE;
            }
        }
    }
}

/******************************************************************
 * FUNCTION : DisplayClearScreen()
 *    Description: Clear screen instruction
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
void DisplayClearScreen()
{
    /* Clear screen array */
    (void)memset(display.screen, TRUE, DISPLAY_WIDTH * DISPLAY_HEIGHT);
}

/******************************************************************
 * FUNCTION : DisplayDraw()
 *    Description: Draw instruction
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
void DisplayDraw(U8 *memory, U8 x, U8 y, U8 n)
{
    U8 i;
    U8 j;

    /* Number of bytes in the sprite */
    for (i = 0U; i < n; i++)
    {
        /* Go on bits */
        for (j = 0U; j < 8U; j++)
        {
            if (displayGetBit(memory[i], j))
            {
                display.screen[x+j][y+i] ^= TRUE;
            }
            else
            {
                display.screen[x+j][y+i] ^= FALSE;
            }
        }
    }
}

/******************************************************************
 * FUNCTION : displayGetBit()
 *    Description: Find bit to display in a byte
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
static BOOL displayGetBit(U8 byte, U8 n)
{
    U8 value = byte & (U8)(DISPLAY_GET_BIT_MASK >> n); 

    return (BOOL)value;
}

/******************************************************************
 * FUNCTION : DisplayUpdate()
 *    Description: Refresh screen
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
void DisplayUpdate(void)
{
    display.renderer = SDL_CreateRenderer(display.window, -1, SDL_RENDERER_ACCELERATED);

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

        /* Run Cpu main loop */
        (void)CpuMain();

        displayUpdate();

        /* 50 FPS */
        SDL_Delay(20U);
    }
}

/******************************************************************
 * FUNCTION : displayUpdate()
 *    Description: Draw pixels on screen from display.screen array
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
static void displayUpdate(void)
{
    U8 i;
    U8 j;

    /* Clear display buffer */
    SDL_RenderClear(display.renderer);

    /* Set back color */
    SDL_SetRenderDrawColor(display.renderer, 0U, 0U, 0U, 255U);

    SDL_Rect rect = {0, 0, DISPLAY_WIDTH_SIZED, DISPLAY_HEIGHT_SIZED};

    SDL_RenderFillRect(display.renderer, &rect);

    /* Set color for white pixel */
    SDL_SetRenderDrawColor(display.renderer, 255U, 255U, 255U, 255U);

    /* Initialize screen */
    for (i = 0U; i < DISPLAY_WIDTH; i++)
    {
        for (j = 0U; j < DISPLAY_HEIGHT; j++)
        {
            /* FALSE is white pixel */
            if (display.screen[i][j] == FALSE)
            {
                SDL_Rect rect2 = {
                    (int)(DISPLAY_PIXEL_WIDTH_IN_PIXELS * i), (int)(DISPLAY_PIXEL_HEIGH_IN_PIXELS * j),
                    DISPLAY_PIXEL_WIDTH_IN_PIXELS, DISPLAY_PIXEL_HEIGH_IN_PIXELS
                };
                SDL_RenderFillRect(display.renderer, &rect2);
            }
        }
    }

    SDL_RenderPresent(display.renderer);
}

/******************************************************************
 * FUNCTION : DisplayExit()
 *    Description: Clean display
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
void DisplayExit()
{
    SDL_DestroyRenderer(display.renderer);
    SDL_DestroyWindow(display.window);
    SDL_Quit();
}