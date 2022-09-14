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
#include "../input/input.h"
#include "../sound/sound.h"

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
    DisplayClearScreen();
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
    (void)memset(display.screen, FALSE, DISPLAY_WIDTH * DISPLAY_HEIGHT);
}

/******************************************************************
 * FUNCTION : DisplayDraw()
 *    Description: Draw instruction
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
void DisplayDraw(U8 *memory, U8 *vf, U8 x, U8 y, U8 n)
{
    U8 i;
    U8 j;
    U8 xMod;
    U8 yMod;

    /* Clear vf */
    *vf = 0U;

    /* Number of bytes in the sprite (height) */
    for (i = 0U; i < n; i++)
    {
        yMod = (y+i) % DISPLAY_HEIGHT;

        /* Go on bits (width) */
        for (j = 0U; j < 8U; j++)
        {
            xMod = (x+j) % DISPLAY_WIDTH;

            /* Check if edge of screen is not reach */
            if ((x+j) < DISPLAY_WIDTH)
            {
                /* If the current pixel in the sprite row is on */
                if (displayGetBit(memory[i], j))
                {
                    /* If pixel on screen is on set vf to 1 */
                    if (display.screen[xMod][yMod] == TRUE)
                    {
                        *vf = 1U;
                        display.screen[xMod][yMod] = FALSE;
                    }
                    else
                    {
                        display.screen[xMod][yMod] = TRUE;
                    }
                }
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

    value = value >> (7U - n);

    return value;
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
                (void)InputUpdateKeyboardDown(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                InputUpdateKeyboardUp(event.key.keysym.sym);
                break;
            }
        }

        /* Run Cpu main loop */
        (void)CpuMain();

        displayUpdate();

        /* 50 FPS */
        SDL_Delay(1U);
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
            /* TRUE is white pixel */
            if (display.screen[i][j] == TRUE)
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
    SoundExit();
    SDL_DestroyRenderer(display.renderer);
    SDL_DestroyWindow(display.window);
    SDL_Quit();
}