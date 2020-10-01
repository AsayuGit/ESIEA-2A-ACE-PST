#include "Dialogue.h"

// Fonction non bloquante gérant les dialogues
void Dialogue(DisplayDevice* DisplayDevice, InputDevice* InputDevice, DialogueContext* Context){

    if (Context->DialogBox == NULL){
        Context->DialogBox = LoadSurface(ROOT""TEXTURES"Menus"SL"Dialog"TEX_EXT, DisplayDevice, NULL);
        if (Context->DialogBox == NULL){
            fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
        }
    }


    SDL_RenderCopy(DisplayDevice->Renderer, Context->DialogBox, NULL, NULL);

}