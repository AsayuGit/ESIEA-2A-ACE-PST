#ifndef _TYPES
#define _TYPES

typedef struct{
    int x;
    int y;
} Vector2i;


typedef struct{
    SDL_Window *Screen;
    SDL_Renderer *Renderer;
} DisplayDevice;

#endif