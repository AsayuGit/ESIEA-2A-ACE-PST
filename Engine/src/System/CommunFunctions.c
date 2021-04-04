#include "CommunFunctions.h"

Vector2i RectTileToCorrdinate(SDL_Rect SrcRect, Vector2i SurfaceBounds, int TileX, int TileY){
    Vector2i Coordinates;
/*
    SrcRect.y = ((TileID * SrcRect.w) / SurfaceBounds.x);
    SrcRect.x = (TileID - (SrcRect.y * (SurfaceBounds.x / SrcRect.w))) * SrcRect.w;
    SrcRect.y *= SrcRect.h;

    Coordinates.y = SrcRect.y;
    Coordinates.x = SrcRect.x; */

    Coordinates.x = TileX * SrcRect.w;
    Coordinates.y = TileY * SrcRect.h;

    return Coordinates;
}

unsigned int getFontHeight(BitmapFont* Font){
    unsigned int FontHeight = 1;
    while(getpixel(Font->FontSurface, 0, FontHeight) != 0x0){
        FontHeight++;
    }

    return FontHeight - 1;
}

SDL_Rect gputc(DisplayDevice* DDevice, BitmapFont* Font, char c, int x, int y, int NewLineJump, SDL_Rect* Bounds){
    SDL_Rect SrcLetter, DstLetter;
    int letterX, letterY, letterW, letterID;

    DstLetter.x = x;
    DstLetter.y = y;
    DstLetter.h = NewLineJump;

    // Text processing
    if (c != '\n'){
        letterX = 0;
        letterY = 0;
        
        for (letterID = 0; letterID < c - 31; letterX++){
            if (getpixel(Font->FontSurface, letterX, letterY) == 0x0){
                letterID++;
            }
        }
        letterW = letterX;
        while (letterID < c - 30){
            if (getpixel(Font->FontSurface, letterW, letterY) == 0x0){
                letterID++;
            }
            letterW++;
        }
        letterW -= letterX + 1;
        letterX--;
        letterY = 1;
        while(getpixel(Font->FontSurface, letterX, letterY) != 0x0){
            letterY++;
        }

        // Text Rendering
        SrcLetter.x = letterX + 1;
        SrcLetter.y = 1;
        SrcLetter.h = DstLetter.h = letterY - 1;
        SrcLetter.w = DstLetter.w = letterW;

        #ifdef _SDL
            SDL_BlitSurface(Context->Font->FontSurface, &SrcLetter, Context->textLayer, &Context->DstLetter);
        #else
            SDL_RenderCopy(DDevice->Renderer, Font->FontTexture, &SrcLetter, &DstLetter);
        #endif

        if ((Bounds) && (DstLetter.x + SrcLetter.w > Bounds->w + Bounds->x)){
            DstLetter.y += SrcLetter.h + 1;
            DstLetter.x = Bounds->x;
        } else {
            DstLetter.x += SrcLetter.w + 1;
        }
    }else{
        if (Bounds){
            DstLetter.y += DstLetter.h + 1; // May change for a standard offset in the future
            DstLetter.x = Bounds->x;
        }
    }

    return DstLetter;
}

SDL_Rect gprintf(DisplayDevice* DDevice, BitmapFont* Font, char* text, SDL_Rect* Bounds){
    int CharID;
    SDL_Rect DstLetter;

    if (Bounds) {
        DstLetter.x = Bounds->x;
        DstLetter.y = Bounds->y;
    } else {
        DstLetter.x = 0;
        DstLetter.y = 0;

    }

    CharID = 0;
    while (text[CharID] != '\0'){
        DstLetter = gputc(DDevice, Font, text[CharID], DstLetter.x, DstLetter.y, DstLetter.h, Bounds);
        CharID++;
    }

    return DstLetter;
}

int gstrlen(BitmapFont* Font, char* text){
    int LastLineLen, StringLen;
    int i;

    SDL_Rect SrcLetter;
    int letterX, letterY, letterW, letterID;

    i = 0;
    StringLen = 0;
    LastLineLen = 0;
    while (text[i] != '\0'){
        if (text[i] != '\n'){
            letterX = 0;
            letterY = 0;
            
            for (letterID = 0; letterID < text[i] - 31; letterX++){
                if (getpixel(Font->FontSurface, letterX, letterY) == 0x0){
                    letterID++;
                }
            }
            letterW = letterX;
            while (letterID < text[i] - 30){
                if (getpixel(Font->FontSurface, letterW, letterY) == 0x0){
                    letterID++;
                }
                letterW++;
            }
            letterW -= letterX + 1;
            letterX--;
            letterY = 1;
            while(getpixel(Font->FontSurface, letterX, letterY) != 0x0){
                letterY++;
            }

            // Text Rendering
            SrcLetter.x = letterX + 1;
            SrcLetter.y = 1;
            SrcLetter.h = letterY - 1;
            SrcLetter.w = letterW;

            // Display
            StringLen += SrcLetter.w + 1;
        } else {
            if (StringLen > LastLineLen)
                LastLineLen = StringLen;
            StringLen = 0;
        }
        i++;
    }

    return (StringLen > LastLineLen) ? StringLen : LastLineLen;
}