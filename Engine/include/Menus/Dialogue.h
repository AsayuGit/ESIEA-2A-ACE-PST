#ifndef _DIALOGUE_H
#define _DIALOGUE_H

    #include "include.h"

    typedef struct{
        /* Meta */
        bool shown;
        int progress;
        char* Text;
        int TextSpeed;
        Uint32 LastLetter;

        /* Dialog Box */
        SDL_Texture* DialogBox;
        SDL_Rect DialogBoxSrcBounds;
        SDL_Rect DialogBoxBounds;   /* Region where the dialog box should be drawn */
        SDL_Rect TextBounds;        /* Region where the text should be drawn */
        SDL_Rect NameBounds;        /* Region where the name should be drawn */
        SDL_Rect ArrowDstRect;

        /* Font */
        BitmapFont* MainFont;
        BitmapFont* NameFont;
        SDL_Rect DstLetter;
        SDL_Texture* textLayer;
        SDL_Texture* nameLayer;

        char letterLag;
        Mix_Chunk* Letter;
        Mix_Chunk* NextLine;
        Mix_Chunk* LineComplete;

        /* Display */
        DisplayDevice* DDevice;
    } DialogueContext;

    void ClearDialogueText(DialogueContext* Context);
    size_t SetDialogueText(DialogueContext* Context, char* Name, char* Text, char SndEffType);
    DialogueContext* InitDialog(DisplayDevice* DDevice, BitmapFont* MainFont, BitmapFont* NameFont);
    void Dialogue(DialogueContext* Context, bool bothWay);

#endif