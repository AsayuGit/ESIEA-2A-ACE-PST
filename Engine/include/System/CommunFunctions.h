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

#ifndef _COMMUNFUNCTIONS_H
#define _COMMUNFUNCTIONS_H

    #include "include.h"

    int      gputc(DisplayDevice* DDevice, BitmapFont* Font, char c, unsigned int color, unsigned int x, unsigned int y);
    Vector2i gprintf(DisplayDevice* DDevice, BitmapFont* Font, char* text, int intCharSpce, const SDL_Rect* Bounds);
    Vector2i gstrlen(BitmapFont* Font, char* text, int intCharSpce);
    int      map(int SrcA, int SrcB, int DstA, int DstB, int Value);
    double   dmap(double DstA, double DstB, double SrcA, double SrcB, double Value);
    char*    astrcpy(char** dst, char* src);
    SDL_Rect RectToVieport(const SDL_Rect* InputRect, const SDL_Rect* ViewPort, const Vector2i* Position);

#endif