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

#include "Types.h"

SDL_Rect InitRect(int x, int y, int w, int h){
    SDL_Rect rect;
    
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    return rect;
}

Vector2d InitVector2d(double x, double y){
    Vector2d vector;

    vector.x = x;
    vector.y = y;

    return vector;
}

Vector2i InitVector2i(int x, int y){
    Vector2i vector;

    vector.x = x;
    vector.y = y;

    return vector;
}

SDL_Rect RectPlusVector2i(SDL_Rect* Rect, Vector2i* Vect){
    SDL_Rect Result;

    Result.x = Rect->x + Vect->x;
    Result.y = Rect->y + Vect->y;
    Result.w = Rect->w;
    Result.h = Rect->h;

    return Result;
}

SDL_Rect RectMinusVector2i(SDL_Rect* Rect, Vector2i* Vect){
    SDL_Rect Result;

    Result.x = Rect->x - Vect->x;
    Result.y = Rect->y - Vect->y;
    Result.w = Rect->w;
    Result.h = Rect->h;

    return Result;
}