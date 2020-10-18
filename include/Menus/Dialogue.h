#ifndef _DIALOGUE
#define _DIALOGUE

#include "include.h"

typedef struct{
    // Meta
    bool shown;
    int progress;
    char* Text;

    // Dialog Box
    Surface* DialogBox;
    SDL_Rect DialogBoxBounds;
    
    // Font
    BitmapFont* Font;
    SDL_Rect DstLetter;
    Surface* textLayer;
} DialogueContext;

DialogueContext* InitDialog(BitmapFont* Font);
void ClearDialogueText(DisplayDevice* DDevice, DialogueContext* Context);
void SetDialogueText(DialogueContext* Context, char* Text);
void Dialogue(DisplayDevice* DisplayDevice, InputDevice* InputDevice, DialogueContext* Context);




#endif