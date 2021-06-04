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

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

    #include "include.h"

    Uint32 getpixel(SDL_Surface *surface, int x, int y);
    SDL_Surface* CreateEmptySurface(Uint32 flags, int Width, int Height);
    void FlipBlitSurface(SDL_Surface* SourceSurface, const SDL_Rect* SrcRect, SDL_Surface* ScreenTarget, const SDL_Rect* DstRect, bool flipFlag);
    bool RectOnRect(const SDL_Rect* SrcRect, const SDL_Rect* DstRect);
    bool RectOnScreen(DisplayDevice* DDevice, const SDL_Rect* Rect);
    void DrawFrame(DisplayDevice* DDevice);

#endif