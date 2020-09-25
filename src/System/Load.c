#include "Load.h"


Surface* LoadSurface(char FilePath[], DisplayDevice* Device){
    SDL_Surface* LoadingSurface;
    Surface* AcceleratedSurface;

    LoadingSurface = SDL_LoadBMP(FilePath);

    #ifdef _SDL
    AcceleratedSurface = SDL_DisplayFormat(LoadingSurface);
    #else
    AcceleratedSurface = SDL_CreateTextureFromSurface(Device->Renderer, LoadingSurface);
    #endif
    SDL_FreeSurface(LoadingSurface);
    
    return AcceleratedSurface;
}