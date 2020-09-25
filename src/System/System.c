#include "System.h"

void InitSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "SDL Initialisation failed\n - %s\n", SDL_GetError());
        exit(EXIT_INIT);
    }
}

InputDevice* InitInputs(bool JoyEnabled){
    InputDevice* Inputs = (InputDevice*)malloc(sizeof(InputDevice));
    Inputs->Joy1 = NULL;
    if (SDL_NumJoysticks() && JoyEnabled){
        Inputs->Joy1 = SDL_JoystickOpen(0); // Open Joystick
    }
    Inputs->KeyStates = SDL_GetKeyboardState(NULL); // Open Keyboard
    if (Inputs->Joy1 == NULL){
        Inputs->JoyEnabled = false;
    }else{
        Inputs->JoyEnabled = true;
    }
    return Inputs;
}

DisplayDevice* CreateDisplayDevice(int ScreenWidth, int ScreenHeight, char* Title){
    DisplayDevice* Device = (DisplayDevice*)malloc(sizeof(DisplayDevice));
    Device->Screen = SDL_CreateWindow(Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
    if (Device->Screen == NULL){
        fprintf(stderr, "Can't create main window\n - %s\n", SDL_GetError());
        exit(EXIT_INIT);
    }
    Device->Renderer = SDL_CreateRenderer(Device->Screen , -1, 0);
    if (Device->Renderer == NULL){
        fprintf(stderr, "Can't create main renderer\n - %s\n", SDL_GetError());
        exit(EXIT_INIT);
    }
    SDL_GL_SetSwapInterval(1); // VSync

    Device->ScreenResolution.x = ScreenWidth;
    Device->ScreenResolution.y = ScreenHeight;

    return Device;
}