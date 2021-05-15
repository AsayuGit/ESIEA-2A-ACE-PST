#ifndef _GRAPHICS_H
#define _GRAPHICS_H

    #include "include.h"

    Uint32 getpixel(SDL_Surface *surface, int x, int y);
    SDL_Surface* CreateEmptySurface(int Width, int Height);
    void FlipBlitSurface(SDL_Surface* SourceSurface, const SDL_Rect* SrcRect, SDL_Surface* ScreenTarget, const SDL_Rect* DstRect, bool flipFlag);
    bool RectOnRect(const SDL_Rect* SrcRect, const SDL_Rect* DstRect);
    bool RectOnScreen(DisplayDevice* DDevice, const SDL_Rect* Rect);
    void DrawFrame(DisplayDevice* DDevice);

#endif