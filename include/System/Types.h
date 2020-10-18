#ifndef _TYPES
#define _TYPES

typedef enum{
    false,
    true
} bool;

typedef struct{
    int x;
    int y;
} Vector2i;

typedef struct{
    SDL_Window *Screen;
    SDL_Renderer *Renderer;
    Vector2i ScreenResolution;
} DisplayDevice;

typedef struct{
    const Uint8* KeyStates; // Pointer to the keypresses
    SDL_Joystick* Joy1; // Pointers to the Joypad
    bool JoyEnabled;
} InputDevice;

typedef struct{
    SDL_Surface* FontSurface;
    #ifndef SDL
    SDL_Texture* FontTexture;
    #endif
} BitmapFont;

// Uniformity between SDL1.2 and SDL2
#ifdef SDL
typedef SDL_Surface Surface;
#define SDL_GetKeyState SDL_GetKeyboardState
#else
typedef SDL_Texture Surface;
#endif

#endif