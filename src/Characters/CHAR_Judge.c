#include "CharactersStructures.h"
/* Character ID 3 */
Animation ANIM_Judge[6] = {
    {
        {8, 11, 118, 96}, /* SrcRect Idle [0] */
        {69, 35, 118, 96}, /* DstRect */
        5,   /* NbOfFrames */
        500 /* Framerate */
    },
    {
        {131, 136, 118, 96}, /* SrcRect Talk [1] */
        {69, 35, 118, 96}, /* DstRect */
        4,   /* NbOfFrames */
        125 /* Framerate */
    },
    {
        {12, 706, 118, 96}, /* SrcRect Serious Idle [2] */
        {69, 36, 118, 96}, /* DstRect */
        5,   /* NbOfFrames */
        500 /* Framerate */
    },
    {
        {134, 834, 118, 96}, /* SrcRect Serious Talk [3] */
        {69, 36, 118, 96}, /* DstRect */
        4,   /* NbOfFrames */
        125 /* Framerate */
    },
    {
        {10, 477, 118, 96}, /* SrcRect Shocked Idle [4] */
        {69, 36, 118, 96}, /* DstRect */
        5,   /* NbOfFrames */
        250 /* Framerate */
    },
    {
        {134, 600, 118, 96}, /* SrcRect Shocked Talk [5] */
        {69, 36, 118, 96}, /* DstRect */
        4,   /* NbOfFrames */
        125 /* Framerate */
    }
};

Characters CHAR_Judge = { /* Judge */
    "Judge",
    NULL,
    ROOT""TEXTURES"Characters"SL"Judge"TEX_EXT,
    0x00b5a5d5,
    NULL,
    0,
    0,
    0,
    (Animation*)&ANIM_Judge
};