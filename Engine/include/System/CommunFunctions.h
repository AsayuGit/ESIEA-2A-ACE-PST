#ifndef _COMMUNFUNCTIONS_H
#define _COMMUNFUNCTIONS_H

    #include "include.h"

    Vector2i RectTileToCorrdinate(SDL_Rect SrcRect, Vector2i SurfaceBounds, int TileX, int TileY);
    unsigned int getFontHeight(BitmapFont* Font);
    SDL_Rect gputc(DisplayDevice* DDevice, BitmapFont* Font, char c, unsigned int x, unsigned int y, unsigned int intLineSpce, int intCharSpce, SDL_Rect* Bounds);
    SDL_Rect gprintf(DisplayDevice* DDevice, BitmapFont* Font, char* text, int intCharSpce, SDL_Rect* Bounds);
    int      gstrlen(BitmapFont* Font, char* text, int intCharSpce);
    int      map(int SrcA, int SrcB, int DstA, int DstB, int Value);
    double   dmap(double DstA, double DstB, double SrcA, double SrcB, double Value);
#endif