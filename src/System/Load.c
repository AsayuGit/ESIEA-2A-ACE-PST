#include "Load.h"


Surface* LoadSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey){
    SDL_Surface* LoadingSurface;
    Surface* AcceleratedSurface;

    LoadingSurface = SDL_LoadBMP(FilePath);

    #ifdef _SDL
    AcceleratedSurface = SDL_DisplayFormat(LoadingSurface);
    if (ColorKey != NULL)
        SDL_SetColorKey(LoadingSurface, true, *ColorKey);
    #else
    if (ColorKey != NULL)
        SDL_SetColorKey(LoadingSurface, true, *ColorKey);
    AcceleratedSurface = SDL_CreateTextureFromSurface(Device->Renderer, LoadingSurface);
    #endif
    SDL_FreeSurface(LoadingSurface);
    
    return AcceleratedSurface;
}