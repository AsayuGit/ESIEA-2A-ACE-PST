#ifndef _TYPES
#define _TYPES

    // Uniformity between SDL1.2 and SDL2
    #ifdef _SDL
        typedef SDL_Surface SDL_Window;
        typedef SDL_Surface Surface;
        #define SDL_GetKeyboardState SDL_GetKeyState
        #define PADKEY event.key.keysym.sym
        #define P_PADKEY event->key.keysym.sym
    #else
        typedef SDL_Texture Surface;
        #define SDL_HWSURFACE SDL_SWSURFACE
        #define PADKEY event.key.keysym.scancode
        #define P_PADKEY event->key.keysym.scancode
    #endif

    typedef enum{
        false,
        true
    } bool;

    typedef struct{
        int x;
        int y;
    } Vector2i;

    typedef struct{
        double x;
        double y;
    } Vector2d;

    typedef struct{
        SDL_Window *Screen;
        #ifndef _SDL
            SDL_Renderer *Renderer;
        #endif
        Vector2i ScreenResolution;
    } DisplayDevice;

    typedef struct{
        const Uint8* KeyStates; // Pointer to the keypresses
        SDL_Joystick* Joy1; // Pointers to the Joypad
        bool JoyEnabled;
    } InputDevice;

    typedef struct{
        char FOO;
    } SoundDevice;

    typedef struct{
        SDL_Surface* FontSurface;
        #ifndef _SDL
            SDL_Texture* FontTexture;
        #endif
        unsigned int FontHeight;
    } BitmapFont;

#endif