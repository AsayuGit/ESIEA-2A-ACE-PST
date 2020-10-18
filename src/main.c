#include "include.h"
#include "Scenes.h"

int main(int argc, char* argv){

    DisplayDevice* DDevice;
    InputDevice* IDevice;

    BitmapFont MainFont;
    Uint32 FontColorKey = 0xff00ff;
    CourtroomContext* CourtContext;

    CourtContext = (CourtroomContext*)malloc(sizeof(CourtroomContext));

    InitSDL();
    DDevice = CreateDisplayDevice(256, 192, "Ace YAY");
    IDevice = InitInputs(false);

    MainFont.FontSurface = NULL;
    MainFont.FontSurface = LoadSDLSurface(ROOT""FONTS"AceAttorneyFont"TEX_EXT, DDevice, &FontColorKey);
    if (MainFont.FontSurface == NULL){
        fprintf(stderr, "Can't load font %s\n", SDL_GetError());
    }
    MainFont.FontTexture = SDL_CreateTextureFromSurface(DDevice->Renderer, MainFont.FontSurface);
    CourtContext->Font = &MainFont;
    // We start the courtroom Scene
    Scene_Courtroom(DDevice, IDevice, CourtContext);
    
    free(CourtContext);

    free(DDevice);

    return 0;
}