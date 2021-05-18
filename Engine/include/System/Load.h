#ifndef _LOAD_H
#define _LOAD_H

    #include "include.h"
	
	#ifdef _XBOX
		#include <libxml/parser.h>
	#else
		#include <libxml2/libxml/parser.h>
	#endif
    enum {
        SURFACE_OPAQUE = 0,
        SURFACE_ALPHA = 1,
        SURFACE_KEYED = 2
    };

    SDL_Surface* LoadSDLSurface(char FilePath[], DisplayDevice* Device);
    void KeySurface(SDL_Surface* SurfaceToKey, Uint32 ColorKey);
    SDL_Texture* LoadSurface(char FilePath[], DisplayDevice* Device, Uint32 ColorKey, char flags);
    BitmapFont* LoadBitmapFont(char FilePath[], DisplayDevice* DDevice, Uint32 FontColorKey);

    SDL_Texture* CreateTargetSurface(DisplayDevice* DDevice, int w, int h);

    Mix_Music* LoadMusic(char FilePath[]);
    Mix_Chunk* LoadSoundEffect(char FilePath[]);

    xmlDoc* loadXml(char* filePath);

#endif