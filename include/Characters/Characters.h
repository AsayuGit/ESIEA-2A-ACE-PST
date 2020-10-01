#ifndef _CHARACTERS
#define _CHARACTERS

#include "include.h"

typedef enum{
    Phoenix_Wright,
    Mia_Fey,
    CharactersCount
} Cid;

typedef struct{
    Surface* Surface; // Character Surface
    char *SurfacePath;// Character Surface path 
    SDL_Rect SrcRect; // Character source rect
    SDL_Rect DstRect; // Character destination rect
    Uint32 ColorKey;  // Character color key
} Characters; // Sprite ?

extern Characters Cast[CharactersCount];

void InitCharacter(int CharacterID, DisplayDevice* DisplayDevice);

#endif