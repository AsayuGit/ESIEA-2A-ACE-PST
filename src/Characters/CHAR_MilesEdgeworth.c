#include "CharactersStructures.h"

Animation ANIM_MilesEdgeworth[2] = {
    {
        {37, 35, 104, 143}, // SrcRect // Idle
        {136, 16, 104, 143}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {493, 35, 104, 143}, // SrcRect // Talk
        {136, 16, 104, 143}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    }
};

Characters CHAR_MilesEdgeworth = { // Miles Edgeworth
    "Edgeworth",
    NULL,
    ROOT""TEXTURES"Characters"SL"Miles_Edgeworth"TEX_EXT,
    0x00b5a5d5,
    NULL,
    0,
    0,
    0,
    (Animation*)&ANIM_MilesEdgeworth
};