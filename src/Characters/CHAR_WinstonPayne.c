#include "CharactersStructures.h"

Animation ANIM_Payne[2] = {
    {
        {17, 26, 85, 134}, /* SrcRect Idle */
        {158, 26, 85, 134}, /* DstRect */
        1,   /* NbOfFrames */
        500 /* Framerate */
    },
    {
        {111, 26, 85, 134}, /* SrcRect Talk */
        {158, 26, 85, 134}, /* DstRect */
        4,   /* NbOfFrames */
        125 /* Framerate */
    }
};

Characters CHAR_WinstonPayne = { /* Winston Payne */
    "Payne",
    NULL,
    ROOT""TEXTURES"Characters"SL"Winston_Payne"TEX_EXT,
    0x00b5a5d5,
    NULL,
    0,
    0,
    0,
    (Animation*)&ANIM_Payne
};