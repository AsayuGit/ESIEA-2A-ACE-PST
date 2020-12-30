#ifndef _COURTROOM
#define _COURTROOM

    #include "include.h"

    typedef struct{
        BitmapFont* Font;
    } CourtroomContext;

    int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context);

#endif