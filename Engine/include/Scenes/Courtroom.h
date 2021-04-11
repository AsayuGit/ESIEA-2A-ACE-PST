#ifndef _COURTROOM
#define _COURTROOM

    #include "include.h"
    #include "CharactersStructures.h"

    typedef struct{
        BitmapFont* MainFont;
        BitmapFont* NameFont;
        BitmapFont* ButtonFont;
    } CourtroomContext;

    int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context, Characters** CharacterIndex, int NbOfCharacters, char* DialogPath);

#endif