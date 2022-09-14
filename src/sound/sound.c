/******************************************************************
 *
 *
 * FILE        : sound.c
 * PROJECT     : chip8
 * AUTHOR      : danpham
 * DESCRIPTION : Sound
 *
 ******************************************************************/

/******************************************************************
 * 1. Included files (microcontroller ones then user defined ones)
 ******************************************************************/
#include <SDL2/SDL.h>
#include "sound.h"

/******************************************************************
 * 2. Define declarations (macros then function macros)
 ******************************************************************/

/******************************************************************
 * 3. Typedef definitions (simple typedef, then enum and structs)
 ******************************************************************/

/******************************************************************
 * 4. Variable definitions (static then global)
 ******************************************************************/
static SDL_AudioSpec s_wavSpec;
static uint32_t s_wavLength;
static uint8_t * s_wavBuffer;
static SDL_AudioDeviceID s_deviceId;

/******************************************************************
 * 5. Functions prototypes (static only)
 ******************************************************************/

/******************************************************************
 * FUNCTION : SoundInit()
 *    Description: Initialize sound
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
void SoundInit(void)
{
    /* Sound init should be after SDL_Init */
	SDL_LoadWAV("build/sound.wav", &s_wavSpec, &s_wavBuffer, &s_wavLength);

    /* Opening the audio device */
	s_deviceId = SDL_OpenAudioDevice(NULL, 0, &s_wavSpec, NULL, 0);

}

/******************************************************************
 * FUNCTION : SoundPlay()
 *    Description: Play a sound
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
void SoundPlay(void)
{
	U32 success = SDL_QueueAudio(s_deviceId, s_wavBuffer, s_wavLength);
	SDL_PauseAudioDevice(s_deviceId, 0);
}

/******************************************************************
 * FUNCTION : SoundExit()
 *    Description: Free sound ressources
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
void SoundExit(void)
{
	SDL_CloseAudioDevice(s_deviceId);
	SDL_FreeWAV(s_wavBuffer);
}
