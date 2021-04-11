#include "CharactersStructures.h"
/* Character ID 6 */
Animation ANIM_FrankShawit[4] = {
    {
        {0, 0, 95, 148}, /* SrcRect Idle [0] */
        {79, 28, 95, 148}, /* DstRect */
        4,   /* NbOfFrames */
        500 /* Framerate */
    },
    {
        {391, 0, 94, 148}, /* SrcRect Talk [1] */
        {80, 28, 94, 148}, /* DstRect */
        4,   /* NbOfFrames */
        125 /* Framerate */
    },
    {
        {0, 0, 0, 0}, /* SrcRect Stressed Idle [2] */
        {0, 0, 0, 0}, /* DstRect */
        4,   /* NbOfFrames */
        500 /* Framerate */
    },
    {
        {0, 0, 0, 0}, /* SrcRect Stressed Talk [3] */
        {0, 0, 0, 0}, /* DstRect */
        4,   /* NbOfFrames */
        125 /* Framerate */
    }
};

Characters CHAR_FrankShawit = { /* FrankShawit */
    "Shawit",
    NULL,
    ROOT""TEXTURES"Characters"SL"Frank_Shawit"TEX_EXT,
    0x00b5a5d5,
    NULL,
    0,
    0,
    0,
    (Animation*)&ANIM_FrankShawit
};