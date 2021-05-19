#ifndef _COMMUNFUNCTIONS_H
#define _COMMUNFUNCTIONS_H

    #include "include.h"

    int      gputc(DisplayDevice* DDevice, BitmapFont* Font, char c, unsigned int x, unsigned int y);
    Vector2i gprintf(DisplayDevice* DDevice, BitmapFont* Font, char* text, int intCharSpce, const SDL_Rect* Bounds);
    Vector2i gstrlen(BitmapFont* Font, char* text, int intCharSpce);
    int      map(int SrcA, int SrcB, int DstA, int DstB, int Value);
    double   dmap(double DstA, double DstB, double SrcA, double SrcB, double Value);
    char*    astrcpy(char** dst, char* src);
    SDL_Rect RectToVieport(const SDL_Rect* InputRect, const SDL_Rect* ViewPort, const Vector2i* Position);

#endif