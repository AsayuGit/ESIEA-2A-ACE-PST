#ifndef _COURTROOM
#define _COURTROOM

#include "include.h"

typedef struct{
    BitmapFont* Font;
} CourtroomContext;

int Scene_Courtroom(DisplayDevice* DisplayDevice, InputDevice* InputDevice, CourtroomContext* Context);

#endif