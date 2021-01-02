#ifndef _DIALOGUE
#define _DIALOGUE

    #include "include.h"

    typedef struct{
        // Meta
        bool shown;
        int progress;
        char* Text;
        int TextSpeed;
        Uint32 LastLetter;

        // Dialog Box
        Surface* DialogBox;
        SDL_Rect DialogBoxBounds;
        
        // Font
        BitmapFont* Font;
        SDL_Rect DstLetter;
        Surface* textLayer;

        char letterLag;
        Mix_Chunk* Letter;
        Mix_Chunk* NextLine;
        Mix_Chunk* LineComplete;

        // Display
        DisplayDevice* DDevice;
    } DialogueContext;

    void ClearDialogueText(DialogueContext* Context);
    int SetDialogueText(DialogueContext* Context, char* Text, char SndEffType);
    DialogueContext* InitDialog(DisplayDevice* DDevice, BitmapFont* Font);
    void Dialogue(InputDevice* InputDevice, DialogueContext* Context);

#endif