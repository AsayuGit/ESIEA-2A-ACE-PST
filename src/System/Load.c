#include "Load.h"

#ifdef _SDL
    SDL_Surface* UseAlphaChannel(SDL_Surface* AlphaSurface){
        SDL_SetAlpha(AlphaSurface, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
        AlphaSurface->format->Amask = 0xFF000000;
        //AlphaSurface->format->Ashift = 24;
        return AlphaSurface;
    }
#endif

SDL_Surface* LoadSDLSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey){
    SDL_Surface* LoadingSurface;
    SDL_Surface* AcceleratedSurface;
    LoadingSurface = SDL_LoadBMP(FilePath);
    #ifdef _SDL
        AcceleratedSurface = SDL_DisplayFormat(LoadingSurface);
        SDL_FreeSurface(LoadingSurface);
        if (ColorKey != NULL){
            SDL_SetColorKey(AcceleratedSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL, *ColorKey);
        }else{
            
        }
        return AcceleratedSurface;
    #else
        if (ColorKey != NULL)
            SDL_SetColorKey(LoadingSurface, true, *ColorKey);
        return LoadingSurface;
    #endif
}

Surface* LoadSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey, char AlphaChannel){
    #ifdef _SDL
        if (AlphaChannel)
            return UseAlphaChannel(LoadSDLSurface(FilePath, Device, ColorKey));
        return LoadSDLSurface(FilePath, Device, ColorKey);
    #else
        return SDL_CreateTextureFromSurface(Device->Renderer, LoadSDLSurface(FilePath, Device, ColorKey));
    #endif
}