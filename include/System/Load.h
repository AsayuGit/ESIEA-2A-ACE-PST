#ifndef _LOAD
#define _LOAD

    #include "include.h"

    SDL_Surface* LoadSDLSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey);
    Surface* LoadSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey, char AlphaChannel);
    BitmapFont* LoadBitmapFont(char FilePath[], DisplayDevice* DDevice, Uint32 FontColorKey);

    Surface* CreateTargetSurface(DisplayDevice* DDevice, int w, int h);

#endif