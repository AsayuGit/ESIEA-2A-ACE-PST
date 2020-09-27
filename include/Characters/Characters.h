#ifndef _CHARACTERS
#define _CHARACTERS

#include "include.h"

typedef enum{
    Phoenix_Wright,
    Mia_Fey,
    CharactersCount
} Cid;

typedef struct{
    Surface* Surface; // Character Surfaces
    SDL_Rect SrcRect; // Character source rect
    SDL_Rect DstRect; // Character destination rect
    Uint32 ColorKey;  // Character color keys
} Characters;

extern Characters Cast[CharactersCount];

#endif