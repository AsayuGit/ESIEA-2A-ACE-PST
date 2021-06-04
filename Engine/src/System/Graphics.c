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

#include "Graphics.h"

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    /* Declaration */
    int bpp;
    Uint8 *p;
    
    /* Init */
    if (!surface){
        printf("ERROR: (getpixel) No surface provided !\n");
        return 0; /* Failsafe */
    } else if (!surface->pixels){
        /*printf("ERROR: (getpixel) No pixel data !\n");*/
        return 0; /* Failsafe */
    }

    bpp = surface->format->BytesPerPixel;
    p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp; /* Here p is the address to the pixel we want to retrieve */

    /*printf("%x %d %d %d %d\n", surface->pixels, y, surface->pitch, x, bpp);*/

    /* Logic */
    switch(bpp) {
    case 1:
        /*printf("DEBUG 1)\n");*/
        return *p;
        break;

    case 2:
        /*printf("DEBUG 2)\n");*/
        return *(Uint16 *)p;
        break;

    case 3:
        /*printf("DEBUG 3)\n");*/
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        /*printf("DEBUG 4)\n");*/
        /*printf("Nyan 0x%.6x\n", *(Uint32*)p);*/
        return *((Uint32*)p);
        break;

    default:
        return 0;
        break;
    }
    return 0;
}

SDL_Surface* CreateEmptySurface(Uint32 flags, int Width, int Height){
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        return SDL_CreateRGBSurface(flags, Width, Height, 32, 0x00000000, 0x00000000, 0x00000000, 0x000000FF);
    #else
        return SDL_CreateRGBSurface(flags, Width, Height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    #endif
}

void FlipBlitSurface(SDL_Surface* SourceSurface, const SDL_Rect* SrcRect, SDL_Surface* ScreenTarget, const SDL_Rect* DstRect, bool flipFlag){
    SDL_Rect LocalSrcRect;
    SDL_Rect LocalDstRect;
    
    /* Changing */
    SDL_Rect Src;
    SDL_Rect Dst;

    if (flipFlag){
        if (SrcRect == NULL){
            LocalSrcRect.x = LocalSrcRect.y = 0;
            LocalSrcRect.h = SourceSurface->h;
            LocalSrcRect.w = SourceSurface->w;
        }else{
            LocalSrcRect = *SrcRect;
        }
        if (DstRect == NULL){
            LocalDstRect.x = LocalDstRect.y = 0;
            LocalDstRect.h = ScreenTarget->h;
            LocalDstRect.w = ScreenTarget->w;
        }else{
            LocalDstRect = *DstRect;
        }

        Src.x = LocalSrcRect.x;
        Src.y = LocalSrcRect.y;
        Src.h = LocalSrcRect.h;
        Src.w = 1;

        Dst.x = LocalDstRect.x + LocalDstRect.w;
        Dst.y = LocalDstRect.y;
        Dst.h = LocalDstRect.h;
        Dst.w = 1;


        for (; Src.x < LocalSrcRect.x + LocalSrcRect.w; Src.x++){
            SDL_BlitSurface(SourceSurface, &Src, ScreenTarget, &Dst);
            if (Dst.x > LocalDstRect.x){
                Dst.x--;
            }else{
                break;
            }
        }
    }else{
        if (SrcRect){
            LocalSrcRect = *SrcRect;
        } else {
            LocalSrcRect = InitRect(0, 0, SourceSurface->w, SourceSurface->h);
        }
        if (DstRect){
            LocalDstRect = *DstRect;
        } else {
            LocalDstRect = InitRect(0, 0, BASE_RESOLUTION_X, BASE_RESOLUTION_Y);
        }
        SDL_BlitSurface(SourceSurface, &LocalSrcRect, ScreenTarget, &LocalDstRect);
    }
}

bool RectOnRect(const SDL_Rect* SrcRect, const SDL_Rect* DstRect){
    if ((SrcRect->x + SrcRect->w) < DstRect->x)
        return false;
    if (SrcRect->x > (DstRect->x + DstRect->w))
        return false;
    if ((SrcRect->y + SrcRect->h) < DstRect->y)
        return false;
    if (SrcRect->y > (DstRect->y + DstRect->h))
        return false;
    return true;
}

bool RectOnScreen(DisplayDevice* DDevice, const SDL_Rect* Rect){
    const SDL_Rect BaseRect = {0, 0, BASE_RESOLUTION_X, BASE_RESOLUTION_Y};

    return RectOnRect(Rect, &BaseRect);
}

void DrawFrame(DisplayDevice* DDevice){
    #ifdef _SDL
        SDL_FillRect(DDevice->Renderer, &DDevice->Frame[0], 0x000000);
        SDL_FillRect(DDevice->Renderer, &DDevice->Frame[1], 0x000000);
        SDL_FillRect(DDevice->Renderer, &DDevice->Frame[2], 0x000000);
        SDL_FillRect(DDevice->Renderer, &DDevice->Frame[3], 0x000000);
    #else
        SDL_RenderFillRect(DDevice->Renderer, &DDevice->Frame[0]);
        SDL_RenderFillRect(DDevice->Renderer, &DDevice->Frame[1]);
        SDL_RenderFillRect(DDevice->Renderer, &DDevice->Frame[2]);
        SDL_RenderFillRect(DDevice->Renderer, &DDevice->Frame[3]);
    #endif
}