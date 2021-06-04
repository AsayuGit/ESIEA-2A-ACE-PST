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

#include "CommunFunctions.h"

int gputc(DisplayDevice* DDevice, BitmapFont* Font, char c, unsigned int color, unsigned int x, unsigned int y){
    /* Declaration */
    SDL_Rect DstLetter, SrcLetter;

    /* Init */
    if (!DDevice){ /* DDevice check */
        printf("ERROR: Invalid DisplayDevice !\n");
        goto Exit;
    }
    if (!Font){ /* Font check */
        printf("ERROR: No Font to print with!\n");
        goto Exit;
    }
    c -= 32; /* We only want the printable characters*/
    
    if (c < 0)
        c = 0;

    DstLetter.x = x;
    DstLetter.y = y;
    DstLetter.h = Font->Rects[(int)c].h;
    DstLetter.w = Font->Rects[(int)c].w;

    SrcLetter = Font->Rects[(int)c];
    SrcLetter.y += (SrcLetter.h + 1) * color;

    /* Logic */
    if (DDevice->OffScreenRender){
        Draw(DDevice, Font->FontSurface, &SrcLetter, &DstLetter);
    } else {
        ScaledDraw(DDevice, Font->FontSurface, &SrcLetter, &DstLetter);
    }

Exit:
    return DstLetter.w;
}

Vector2i gstrlen(BitmapFont* Font, char* text, int intCharSpce){
    return gprintf(NULL, Font, text, intCharSpce, NULL);
}

/* Could use the same engine as dialogue for bonds checking */
/* Or a letter by letter mode */
Vector2i gprintf(DisplayDevice* DDevice, BitmapFont* Font, char* text, int intCharSpce, const SDL_Rect* Bounds){
    /* Declaration */
    unsigned int CharID, sizeTmp, DimX, textColor = 0;
    size_t BufferLen = 0;
    char* Buffer;
    Vector2i CharCoords;
    Vector2i Dimensions;
 

    /* Init */
    Dimensions.x = 0;
    Dimensions.y = 0;
    DimX = 0;
    if (Bounds) { /* Bounds check */
        CharCoords.x = Bounds->x;
        CharCoords.y = Bounds->y;
    } else {
        CharCoords.x = 0;
        CharCoords.y = 0;
    }
    CharID = 0;

    /* Logic */

    if (text[CharID] == '\\') {
        /*printf("ColorChange\n");*/
        BufferLen = strcspn(&text[CharID + 1], ";");
        Buffer = malloc(sizeof(char)*BufferLen);
        strncpy(Buffer, &text[CharID + 1], BufferLen);
        textColor = atoi(Buffer);
        free(Buffer);
        CharID += BufferLen + 2;
    }

    while (text[CharID] != '\0'){
        if (text[CharID] != '\n'){
            if (DDevice){
                sizeTmp = gputc(DDevice, Font, text[CharID], textColor, CharCoords.x, CharCoords.y) + intCharSpce;
            } else {
                sizeTmp = Font->Rects[MAX(text[CharID] - 32, 0)].w + intCharSpce;
            }
            CharCoords.x += sizeTmp;
            DimX += sizeTmp;
        } else {
            /* New line */
            sizeTmp = Font->Rects[0].h + 1;
            Dimensions.y += sizeTmp;
            CharCoords.y += sizeTmp;
            CharCoords.x -= DimX;
            
            if (DimX > Dimensions.x){
                Dimensions.x = DimX - intCharSpce;
                DimX = 0;
            }
        }
        CharID++;
    }

    if (DimX > Dimensions.x) Dimensions.x = DimX - intCharSpce; /* Because we don't count the lase inter char space*/

    return Dimensions;
}

int map(int DstA, int DstB, int SrcA, int SrcB, int Value){
    int mappedValue = DstA + ((DstB - DstA) / (SrcB - SrcA)) * (Value - SrcA);
    return (mappedValue < DstB) ? mappedValue : DstB;
}

double dmap(double DstA, double DstB, double SrcA, double SrcB, double Value){
    double mappedValue = DstA + ((DstB - DstA) / (SrcB - SrcA)) * (Value - SrcA);
    return (mappedValue < DstB) ? mappedValue : DstB;
}

char* astrcpy(char** dst, char* src){
    size_t length;

    length = strlen(src)+1;                     /* First we compute how long is the input string */
    *dst = (char*)malloc(length*sizeof(char));   /* Then we allocate the right amount of memory */
    memcpy(*dst, src, length);                  /* Finally we copy the content from the source to the destination */

    return *dst;
}

SDL_Rect RectToVieport(const SDL_Rect* InputRect, const SDL_Rect* ViewPort, const Vector2i* Position){
    SDL_Rect Return;

    Return.x = InputRect->x + Position->x - ViewPort->x;
    Return.y = InputRect->y + Position->y - ViewPort->y;
    Return.w = InputRect->w;
    Return.h = InputRect->h;

    return Return;
}