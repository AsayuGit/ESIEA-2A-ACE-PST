#include "Characters.h"

Animation ANIM_Desk = {
    {0, 0, 256, 44}, // SrcRect // Idle
    {0, 148, 256, 44}, // DstRect
    1,   // NbOfFrames
    0 // Framerate
};

Characters CHAR_Desk = { // Courtroom Desk
    NULL,
    NULL,
    ROOT""TEXTURES"Places"SL"CourtroomDesk"TEX_EXT,
    0x00ff00ff,
    NULL,
    0,
    0,
    0,
    (Animation*)&ANIM_Desk
};