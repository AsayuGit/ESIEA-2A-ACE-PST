#ifndef _DIALOGUE
#define _DIALOGUE

    #include "include.h"

    typedef struct{
        /* Meta */
        bool shown;
        int progress;
        char* Text;
        int TextSpeed;
        Uint32 LastLetter;

        /* Dialog Box */
        Surface* DialogBox;
        SDL_Rect DialogBoxBounds;   /* Region where the dialog box should be drawn */
        SDL_Rect TextBounds;        /* Region where the text should be drawn */
        SDL_Rect NameBounds;        /* Region where the name should be drawn */

        /* Font */
        BitmapFont* MainFont;
        BitmapFont* NameFont;
        SDL_Rect DstLetter;
        Surface* textLayer;
        Surface* nameLayer;

        char letterLag;
        Mix_Chunk* Letter;
        Mix_Chunk* NextLine;
        Mix_Chunk* LineComplete;

        /* Display */
        DisplayDevice* DDevice;
    } DialogueContext;

    void ClearDialogueText(DialogueContext* Context);
    int SetDialogueText(DialogueContext* Context, char* Name, char* Text, char SndEffType);
    DialogueContext* InitDialog(DisplayDevice* DDevice, BitmapFont* MainFont, BitmapFont* NameFont);
    void Dialogue(DialogueContext* Context);

#endif