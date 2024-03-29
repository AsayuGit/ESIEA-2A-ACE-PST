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

#include "Load.h"

#ifdef _SDL
    #include <SDL/SDL_rotozoom.h>
#endif

#ifdef _SDL
    SDL_Surface* UseAlphaChannel(SDL_Surface* AlphaSurface){
        SDL_SetAlpha(AlphaSurface, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
        AlphaSurface->format->Amask = 0xFF000000;
        /* AlphaSurface->format->Ashift = 24; */
        return AlphaSurface;
    }
#endif

#ifdef _SDL
SDL_Surface* ResizeSurface(SDL_Surface* Source, int Scalar){
    SDL_Surface* newSurface;
    SDL_Surface* convertedSurface;
    SDL_Surface* ResizedSurface;
    SDL_Surface* AcceleratedSurface;

    newSurface = CreateEmptySurface(SDL_HWSURFACE, Source->w * Scalar, Source->h * Scalar);
    if (!newSurface)
        printf("ERROR: Couldn't create new surface !\n");
    convertedSurface = SDL_ConvertSurface(newSurface, Source->format, Source->flags);
    if (!convertedSurface)
        printf("ERROR: Couldn't convert surface format !\n");
    SDL_FreeSurface(newSurface);
    SDL_BlitSurface(Source, NULL, convertedSurface, NULL);
    /*SDL_FreeSurface(LoadingSurface);*/
    ResizedSurface = zoomSurface(convertedSurface, Scalar, Scalar, false);
    if (!ResizedSurface){
        printf("ERROR: Couldn't resize surface !\n");
        ResizedSurface = convertedSurface;
    } else {
        SDL_FreeSurface(convertedSurface);
    }
    AcceleratedSurface = SDL_DisplayFormat(ResizedSurface);
    if (!AcceleratedSurface)
        printf("ERROR: Couldn't optimize surface !\n");
    SDL_FreeSurface(ResizedSurface);

    return AcceleratedSurface;
}
#endif

SDL_Surface* LoadSDLSurface(char FilePath[], DisplayDevice* Device){
    SDL_Surface* LoadingSurface;

    #ifdef _SDL
        SDL_Surface* AcceleratedSurface;
    #endif

    LoadingSurface = SDL_LoadBMP(FilePath);
    #ifdef _SDL
        AcceleratedSurface = ResizeSurface(LoadingSurface, Device->IRScalar);
        SDL_FreeSurface(LoadingSurface);

        return AcceleratedSurface;
    #else
        return LoadingSurface;
    #endif
}

void KeySurface(SDL_Surface* SurfaceToKey, Uint32 ColorKey){
    #ifdef _SDL
        SDL_SetColorKey(SurfaceToKey, SDL_SRCCOLORKEY | SDL_RLEACCEL, ColorKey);
    #else
        SDL_SetColorKey(SurfaceToKey, true, ColorKey);
    #endif
}

SDL_Texture* LoadSurface(char FilePath[], DisplayDevice* Device, Uint32 ColorKey, char flags){
    SDL_Surface* loadingSurface;
    
    if (!FilePath) /* Don't bother loading a surface if the path isn't provided */
        return NULL;
    loadingSurface = LoadSDLSurface(FilePath, Device);
    if (loadingSurface){

        if (SURFACE_KEYED & flags){
            KeySurface(loadingSurface, ColorKey);
        }

        #ifdef _SDL
            return (SURFACE_ALPHA & flags) ? UseAlphaChannel(loadingSurface) : loadingSurface;
        #else
            return SDL_CreateTextureFromSurface(Device->Renderer, loadingSurface);
        #endif

    } else {
        printf("ERROR: (loadSurface) Couldn't load %s !\n", FilePath);
    }
    return NULL;
}

BitmapFont* LoadBitmapFont(char FilePath[], DisplayDevice* DDevice, Uint32 FontColorKey){
    /* Declaration */
    BitmapFont* LoadingFont;
    SDL_Surface* LoadingSurface;
    unsigned int FontPixX, FontPixY;
    unsigned int LetterIndex;
    int FontTexW, FontTexH;

    /* Init */
    LetterIndex = 0;
    FontPixX = 1;

    /* Load font surface*/
    LoadingFont = (BitmapFont*)malloc(sizeof(BitmapFont));
    LoadingSurface = LoadSDLSurface(FilePath, DDevice);
    if (!LoadingSurface){
        printf("ERROR: (LoadBitmapFont) Can't load font %s\n", SDL_GetError());
    }
    KeySurface(LoadingSurface, FontColorKey);
    
    FontTexW = LoadingSurface->w;
    FontTexH = LoadingSurface->h;

    /* Fill-in rects for each letter */
    SDL_LockSurface(LoadingSurface); /* Ensure that the pixel data is available for hw surfaces */ /* VERRRY SLOW AND INEFICIENT */
    while ((FontPixX < FontTexW) && (LetterIndex < 95)){

        /* Get letter coordinates */
        FontPixY = 0;
        LoadingFont->Rects[LetterIndex].x = FontPixX;
        LoadingFont->Rects[LetterIndex].y = 1;

        /* Get letter width */
        while ((getpixel(LoadingSurface, FontPixX, FontPixY) != 0x0) && (FontPixX < FontTexW)){
            FontPixX++;
        }
        LoadingFont->Rects[LetterIndex].w = FontPixX - LoadingFont->Rects[LetterIndex].x;
        
        /* Get letter height */
        FontPixY = 1;
        while ((getpixel(LoadingSurface, FontPixX, FontPixY) != 0x0) && (FontPixY < FontTexH)){
            FontPixY++;
        }
        LoadingFont->Rects[LetterIndex].h = FontPixY - LoadingFont->Rects[LetterIndex].y;

        FontPixX++;
        LetterIndex++;
    }
    SDL_UnlockSurface(LoadingSurface);

    #ifdef _SDL
        LoadingFont->FontSurface = LoadingSurface;
    #else
        LoadingFont->FontSurface = SDL_CreateTextureFromSurface(DDevice->Renderer, LoadingSurface);
        SDL_FreeSurface(LoadingSurface);
    #endif

    return LoadingFont;
}

SDL_Texture* CreateTargetSurface(DisplayDevice* DDevice, int w, int h){
    SDL_Texture* LoadingSurface;

    LoadingSurface = NULL;
#ifdef _SDL
    LoadingSurface = CreateEmptySurface(SDL_HWSURFACE, w, h);
#else
    LoadingSurface = SDL_CreateTexture(DDevice->Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(LoadingSurface, SDL_BLENDMODE_BLEND);
#endif
    if (LoadingSurface == NULL)
        fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
    return LoadingSurface;
}

Mix_Music* LoadMusic(char FilePath[]){
    Mix_Music* LoadingMusic = NULL;

    if (FilePath){
        LoadingMusic = Mix_LoadMUS(FilePath);
        if (LoadingMusic == NULL)
            fprintf(stderr, "Can't load music %s\n", Mix_GetError());
    }
    
    return LoadingMusic;
}

Mix_Chunk* LoadSoundEffect(char FilePath[]){
    Mix_Chunk* LoadingSoundEffect;
    
    LoadingSoundEffect = NULL;
    LoadingSoundEffect = Mix_LoadWAV(FilePath);

    if (LoadingSoundEffect == NULL)
        fprintf(stderr, "Can't load sound effect %s\n", Mix_GetError());
    return LoadingSoundEffect;
}

xmlDoc* loadXml(char* filePath){
    xmlKeepBlanksDefault(0); /* Ignore white space */

	#ifdef _XBOX
		return xmlParseFile(filePath); /* Load File into memory */
	#else
		return xmlReadFile(filePath, NULL, 0); /* Load File into memory */
	#endif
}