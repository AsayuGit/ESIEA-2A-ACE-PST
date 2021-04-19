#ifndef _GRAPHICS_H
#define _GRAPHICS_H

    #include "include.h"

    Uint32 getpixel(SDL_Surface *surface, int x, int y);
    SDL_Surface* CreateEmptySurface(int Width, int Height);
    void FlipBlitSurface(SDL_Surface* SourceSurface, SDL_Rect* SrcRect, SDL_Surface* ScreenTarget, SDL_Rect* DstRect, bool flipFlag);

#endif