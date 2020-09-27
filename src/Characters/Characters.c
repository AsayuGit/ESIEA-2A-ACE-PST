#include "Characters.h"

// Characters constants
Characters Cast[CharactersCount] = {
    { // Phoenix Wright
        NULL, // CSurface
        "Hello",
        {0, 0, 0, 0}, // CSrcRects
        {0, 0, 0, 0}, // CDstRects
        0xFFb5a5d5  //CColorKey
    },
    { // Mia Fey
        NULL, // CSurface
        ROOT""TEXTURES"Characters"SL"Mia_Fey"TEX_EXT,
        {54, 1045, 73, 169}, // CSrcRects
        {91, 23, 73, 169}, // CDstRects
        0xFFb5a5d5  //CColorKey
    }
};

void InitCharacter(int CharacterID, DisplayDevice* DisplayDevice){
    Cast[CharacterID].Surface = LoadSurface(Cast[CharacterID].SurfacePath, DisplayDevice, &Cast[CharacterID].ColorKey);
}