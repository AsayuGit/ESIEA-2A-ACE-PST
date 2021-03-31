#ifndef _INCLUDE
#define _INLCUDE

    //#define _GNU_SOURCE // Maybe temporary

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>

    #define EXIT_INIT -2
    #ifdef _SDL
    #define SDLMAIN <SDL/SDL.h>
    #define SDLIMAGE <SDL/SDL_image.h>
    #define SDLMIXER <SDL/SDL_mixer.h>
    #else
    #define SDLMAIN <SDL2/SDL.h>
    #define SDLIMAGE <SDL2/SDL_image.h>
    #define SDLMIXER <SDL2/SDL_mixer.h>
    #endif

    #ifdef _XBOX
    #define ROOT "D:\\"
    #define SL "\\"
    #else // Linux
    #define ROOT "./"
    #define SL "/"
    #endif

    #include SDLMAIN
    #include SDLIMAGE
    #include SDLMIXER

    #include "Flags.h"
    #include "Types.h"
    #include "KeyMap.h"
    #include "System.h"
    #include "Jukebox.h"
    #include "Load.h"
    #include "Graphics.h"
    #include "CommunFunctions.h"

    #define TEXTURES "Assets" SL "Textures" SL
    #define FONTS "Assets" SL "Fonts" SL
    #define TEX_EXT ".bmp"

    #define MUSIC "Assets" SL "Sound" SL "Music" SL
    #define EFFECTS "Assets" SL "Sound" SL "Effects" SL
    #define MUS_EXT ".wav"

#endif