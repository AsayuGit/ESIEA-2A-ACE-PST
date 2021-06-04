/*
    Mia's Legacy is a Ace Attorney fangame taking place directly
    after the first game in the serie. All code in this repo have
    been written from scratch in ANSI C using SDL and libxml2.
    This game is designed to run on Linux Windows and the og Xbox

    Copyright (C) 2021 Killian RAIMBAUD (killian.rai@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef _INCLUDE
#define _INLCUDE

    #include <stdio.h>
    #include <stdlib.h>
#ifdef _XBOX
	#include <io.h>
#else
    #include <unistd.h>
#endif
    #define EXIT_INIT -2
    #ifdef _SDL
    #define SDLMAIN <SDL/SDL.h>
    #define SDLIMAGE <SDL/SDL_image.h>
    #define SDLMIXER <SDL/SDL_mixer.h>
    #elif defined(_XBOX)
	#define SDLMAIN <SDL.h>
    /*#define SDLIMAGE <SDL2/SDL_image.h>*/
    #define SDLMIXER <SDL_mixer.h>
	#else
    #define SDLMAIN <SDL2/SDL.h>
    #define SDLIMAGE <SDL2/SDL_image.h>
    #define SDLMIXER <SDL2/SDL_mixer.h>
    #endif

    #ifdef _XBOX
    #define ROOT "D:\\"
    #define SL "\\"
    #else /* Linux */
    #define ROOT "./"
    #define SL "/"
    #endif

    #include SDLMAIN
    /*#include SDLIMAGE*/
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

    #define MAX(x, y) (((x) > (y)) ? (x) : (y))
    #define MIN(x, y) (((x) < (y)) ? (x) : (y))

    #define BASE_RESOLUTION_X 256
    #define BASE_RESOLUTION_Y 192

    #ifdef _XBOX
        #define DEFAULT_RESOLUTION_X 640
        #define DEFAULT_RESOLUTION_Y 480
    #else
        #define DEFAULT_RESOLUTION_X 1280
        #define DEFAULT_RESOLUTION_Y 960
    #endif

#endif