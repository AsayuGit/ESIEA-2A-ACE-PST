#include "System.h"

void InitSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "SDL Initialisation failed\n - %s\n", SDL_GetError());
        exit(EXIT_INIT);
    }
}

DisplayDevice* CreateDisplayDevice(Vector2i* Resolution, char* Title){
    DisplayDevice* Device = (DisplayDevice*)malloc(sizeof(DisplayDevice));
    Device->Screen = SDL_CreateWindow(Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Resolution->x, Resolution->y, SDL_WINDOW_SHOWN);
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

    return Device;
}