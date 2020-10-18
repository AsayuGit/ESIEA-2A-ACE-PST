#ifndef _LOAD
#define _LOAD

#include "include.h"

SDL_Surface* LoadSDLSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey);
Surface* LoadSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey);

#endif