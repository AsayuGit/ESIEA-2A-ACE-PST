#include "CharactersStructures.h"

Animation ANIM_DefendantDesk = {
    {0, 0, 178, 34}, /* SrcRect Idle */
    {39, 158, 178, 34}, /* DstRect */
    1,   /* NbOfFrames */
    0 /* Framerate */
};

Characters CHAR_DefendantDesk = { /* Defendant Desk */
    NULL,
    NULL,
    ROOT""TEXTURES"Places"SL"DefendantDesk"TEX_EXT,
    0x00ff00ff,
    NULL,
    0,
    0,
    0,
    (Animation*)&ANIM_DefendantDesk
};