#ifndef _INCLUDE
#define _INLCUDE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EXIT_INIT -2

#ifdef SDL
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

#include "Types.h"
#include "System.h"
#include "Load.h"
#include "Graphics.h"

#define TEXTURES "Assets" SL "Textures" SL
#define FONTS "Assets" SL "Fonts" SL
#define TEX_EXT ".bmp"

#endif