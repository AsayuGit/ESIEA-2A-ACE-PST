#ifndef _DIALOGUE
#define _DIALOGUE

#include "include.h"

typedef struct{
    bool shown;
    int progress;
    char* Text;
    Surface* DialogBox;
} DialogueContext;

void Dialogue(DisplayDevice* DisplayDevice, InputDevice* InputDevice, DialogueContext* Context);




#endif