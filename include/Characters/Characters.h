#ifndef _CHARACTERS
#define _CHARACTERS

#include "include.h"

typedef enum{
    Phoenix_Wright,
    Mia_Fey,
    CharactersCount
} Characters;

extern Surface* CSurfaces[CharactersCount]; // Character Surfaces
extern SDL_Rect CSrcRects[CharactersCount]; // Character source rect
extern SDL_Rect CDstRects[CharactersCount]; // Character destination rect
extern Uint32 CColorKey[CharactersCount];   // Character color keys

#endif