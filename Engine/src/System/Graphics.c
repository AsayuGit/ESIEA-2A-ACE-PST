#include "Graphics.h"

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

SDL_Surface* CreateEmptySurface(int Width, int Height){
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        return SDL_CreateRGBSurface(SDL_HWSURFACE, Width, Height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    #else
        return SDL_CreateRGBSurface(SDL_HWSURFACE, Width, Height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    #endif
}

void FlipBlitSurface(SDL_Surface* SourceSurface, SDL_Rect* SrcRect, SDL_Surface* ScreenTarget, SDL_Rect* DstRect, bool flipFlag){
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
        SDL_BlitSurface(SourceSurface, SrcRect, ScreenTarget, DstRect);
    }
}