#include "Load.h"



SDL_Surface* LoadSDLSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey){
    SDL_Surface* LoadingSurface;
    SDL_Surface* AcceleratedSurface;

    LoadingSurface = SDL_LoadBMP(FilePath);

    #ifdef _SDL
    AcceleratedSurface = SDL_DisplayFormat(LoadingSurface);
    SDL_FreeSurface(LoadingSurface);
    if (ColorKey != NULL)
        SDL_SetColorKey(AcceleratedSurface, true, *ColorKey);
    return AcceleratedSurface;
    #else
    if (ColorKey != NULL)
        SDL_SetColorKey(LoadingSurface, true, *ColorKey);
    return LoadingSurface;
    #endif
}


Surface* LoadSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey){
    /*
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
    */
   #ifdef SDL
   return LoadSDLSurface(FilePath, Device, ColorKey);
   #else
   return SDL_CreateTextureFromSurface(Device->Renderer, LoadSDLSurface(FilePath, Device, ColorKey));
   #endif
}