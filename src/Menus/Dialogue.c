#include "Dialogue.h"

// Fonction non bloquante gérant les dialogues
void Dialogue(DisplayDevice* DDevice, InputDevice* InputDevice, DialogueContext* Context){

    if (Context->DialogBox == NULL){
        Context->DialogBox = LoadSurface(ROOT""TEXTURES"Menus"SL"Dialog"TEX_EXT, DDevice, NULL);
        if (Context->DialogBox == NULL){
            fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
        }
    }


    SDL_RenderCopy(DDevice->Renderer, Context->DialogBox, NULL, NULL);

}