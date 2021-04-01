#include "CharactersStructures.h"
// Character ID 1
Animation ANIM_MiaFey[8] = {
    {
        {54, 1045, 73, 169}, // SrcRect // Idle [0]
        {87, 23, 73, 169}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {411, 1045, 73, 169}, // SrcRect // Talk [1]
        {87, 23, 73, 169}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    },
    {
        {56, 828, 92, 166}, // SrcRect // Forward Disbelief Idle [2]
        {76, 26, 92, 166}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {514, 827, 92, 166}, // SrcRect // Forward Disbelief Talk [3]
        {76, 26, 92, 166}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    },
    {
        {53, 1465, 73, 169}, // SrcRect // Sad Idle [4]
        {83, 23, 73, 169}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {450, 1465, 73, 169}, // SrcRect // Sad Talk [5]
        {83, 23, 73, 169}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    },
    {
        {61, 1257, 73, 170}, // SrcRect // Shocked Idle [6]
        {81, 22, 73, 170}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {419, 1257, 73, 170}, // SrcRect // Shocked Talk [7]
        {81, 22, 73, 170}, // DstRect
        3,   // NbOfFrames
        125 // Framerate
    }
};

Characters CHAR_MiaFey = { // Mia Fey
    "Mia",
    NULL,
    ROOT""TEXTURES"Characters"SL"Mia_Fey"TEX_EXT,
    0x00b5a5d5,
    NULL,
    0,
    0,
    0,
    (Animation*)&ANIM_MiaFey
};