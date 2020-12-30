#ifndef _COMMUNFUNCTIONS_H
#define _COMMUNFUNCTIONS_H

    #include "include.h"

    Vector2i RectTileToCorrdinate(SDL_Rect SrcRect, Vector2i SurfaceBounds, int TileX, int TileY);
    SDL_Rect gputc(DisplayDevice* DDevice, BitmapFont* Font, char c, int x, int y, SDL_Rect* Bounds);
    SDL_Rect gprintf(DisplayDevice* DDevice, BitmapFont* Font, char* text, SDL_Rect* Bounds);
#endif