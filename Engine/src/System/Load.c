#include "Load.h"

#ifdef _SDL
    SDL_Surface* UseAlphaChannel(SDL_Surface* AlphaSurface){
        SDL_SetAlpha(AlphaSurface, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
        AlphaSurface->format->Amask = 0xFF000000;
        /* AlphaSurface->format->Ashift = 24; */
        return AlphaSurface;
    }
#endif

SDL_Surface* LoadSDLSurface(char FilePath[], DisplayDevice* Device){
    SDL_Surface* LoadingSurface;
    
    #ifdef _SDL
        SDL_Surface* AcceleratedSurface;
    #endif

    LoadingSurface = SDL_LoadBMP(FilePath);
    #ifdef _SDL
        AcceleratedSurface = SDL_DisplayFormat(LoadingSurface); /* Optimize the surface for blitting */
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
    while (FontPixX < FontTexW){

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
    LoadingSurface = CreateEmptySurface(w, h);
#else
    LoadingSurface = SDL_CreateTexture(DDevice->Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(LoadingSurface, SDL_BLENDMODE_BLEND);
#endif
    if (LoadingSurface == NULL)
        fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
    return LoadingSurface;
}

Mix_Music* LoadMusic(char FilePath[]){
    Mix_Music* LoadingMusic;
    
    LoadingMusic = NULL;
    LoadingMusic = Mix_LoadMUS(FilePath);
    
    if (LoadingMusic == NULL)
        fprintf(stderr, "Can't load music %s\n", Mix_GetError());
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