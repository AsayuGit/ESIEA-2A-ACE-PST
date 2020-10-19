#ifndef _GRAPHICS
#define _GRAPHICS

    #include "include.h"

    Uint32 getpixel(SDL_Surface *surface, int x, int y);
    SDL_Surface* CreateEmptySurface(int Width, int Height);
    void FlipBlitSurface(SDL_Surface* SourceSurface, SDL_Rect* SrcRect, SDL_Surface* ScreenTarget, SDL_Rect* DstRect, bool flipFlag);

#endif