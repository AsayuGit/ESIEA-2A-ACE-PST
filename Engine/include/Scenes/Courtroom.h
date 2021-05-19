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
        char EventSelect;

        char* ButtonJumpLabels[4];
    } CourtroomContext;

    void FreeCourtroom(CourtroomContext* CourtContext);
    CourtroomContext* InitCourtroom(DisplayDevice* DDevice);
    int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context, char* DialogPath);

#endif