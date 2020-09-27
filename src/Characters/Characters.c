#include "Characters.h"

// Characters constants
Characters Cast[CharactersCount] = {
    { // Phoenix Wright
        NULL, // Surface
        ROOT""TEXTURES"Characters"SL"Phoenix_Wright"TEX_EXT,
        {26, 1197, 248, 151}, // SrcRect
        {0, 41, 248, 151}, // DstRect
        0xFFb5a5d5  // ColorKey
    },
    { // Mia Fey
        NULL, // Surface
        ROOT""TEXTURES"Characters"SL"Mia_Fey"TEX_EXT,
        {54, 1045, 73, 169}, // SrcRect
        {87, 23, 73, 169}, // DstRect
        0xFFb5a5d5  // ColorKey
    }
};

void InitCharacter(int CharacterID, DisplayDevice* DisplayDevice){
    Cast[CharacterID].Surface = LoadSurface(Cast[CharacterID].SurfacePath, DisplayDevice, &Cast[CharacterID].ColorKey);
}