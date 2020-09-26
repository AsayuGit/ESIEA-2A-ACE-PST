#include "Characters.h"

// Characters constants

Surface* CSurfaces[CharactersCount];

SDL_Rect CSrcRects[CharactersCount] = {
    {0, 0, 0, 0},
    {54, 1045, 73, 169}//{20, 0, 111, 192}
};

SDL_Rect CDstRects[CharactersCount] = {
    {0, 0, 0, 0},
    {91, 23, 73, 169}
};

Uint32 CColorKey[CharactersCount] = {
    0xFFb5a5d5,
    0xFFb5a5d5
};