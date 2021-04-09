#include "CharactersStructures.h"
/* Character ID 0 */
Animation ANIM_PhoenixWright[5] = { /* Framerate are temporary until the anim varia-speed rework */
    {
        {14, 24, 125, 142}, /* SrcRect Idle */
        {0, 18, 125, 142}, /* DstRect */
        4,   /* NbOfFrames */
        500 /* Framerate */
    },
    {
        {600, 24, 125, 142}, /* SrcRect Talk */
        {0, 18, 125, 142}, /* DstRect */
        4,   /* NbOfFrames */
        125 /* Framerate */
    },
    {
        {14, 185, 107, 136}, /* SrcRect Glup */
        {12, 24, 107, 136}, /* DstRect */
        4,   /* NbOfFrames */
        125 /* Framerate */
    },
    {
        {15, 536, 132, 142}, /* SrcRect Scratching Chin Talk */
        {0, 18, 132, 142}, /* DstRect */
        4,   /* NbOfFrames */
        125 /* Framerate */
    },
    {
        {12, 366, 132, 142}, /* SrcRect Scratching Chin Idle */
        {0, 18, 132, 142}, /* DstRect */
        10,   /* NbOfFrames */
        500 /* Framerate */
    }
};

Characters CHAR_PhoenixWright = { /* Phoenix Wright */
    "Phoenix",
    NULL,                                                   /* Surface */
    ROOT""TEXTURES"Characters"SL"Phoenix_Wright"TEX_EXT,    /* SurfacePath */
    0x00b5a5d5,                                             /* ColorKey */
    NULL,
    0,                                                      /* LastFrame */
    0,                                                      /* CurrentFrame */
    0,                                                      /* PlayingAnimation */
    (Animation*)&ANIM_PhoenixWright                         /* Anim */
};