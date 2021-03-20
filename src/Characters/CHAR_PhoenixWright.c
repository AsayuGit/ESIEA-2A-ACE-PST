#include "CharactersStructures.h"

Animation ANIM_PhoenixWright[3] = {
    {
        {14, 24, 125, 142}, // SrcRect // Idle
        {0, 18, 125, 142}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {600, 24, 125, 142}, // SrcRect // Talk
        {0, 18, 125, 142}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    },
    {
        {14, 185, 107, 136}, // SrcRect // Glup
        {12, 24, 107, 136}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    }
};

Characters CHAR_PhoenixWright = { // Phoenix Wright
    "Phoenix",
    NULL,                                                   // Surface
    ROOT""TEXTURES"Characters"SL"Phoenix_Wright"TEX_EXT,    // SurfacePath
    0x00b5a5d5,                                             // ColorKey
    NULL,
    0,                                                      // LastFrame
    0,                                                      // CurrentFrame
    0,                                                      // PlayingAnimation
    (Animation*)&ANIM_PhoenixWright                          // Anim
};