#ifndef _COURTROOM_H
#define _COURTROOM_H

    #include "include.h"
    #include "CharactersStructures.h"

    typedef struct{
        BitmapFont* MainFont;
        BitmapFont* NameFont;
        BitmapFont* ButtonFont;

        char ButtonActivated;
        int IdleAnimation;
        int CurrentCharacter;
        size_t ReturnToDefault;

        bool diagRewind;

        char* ButtonJumpLabels[4];
    } CourtroomContext;

    int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context, char* DialogPath);

#endif