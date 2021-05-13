#ifndef _TYPES_H
#define _TYPES_H

    #include <stdbool.h>

    #ifdef _SDL
        #include <SDL/SDL.h>
    #else
        #include <SDL2/SDL.h>
    #endif

    /* Uniformity between SDL1.2 and SDL2 */
    #ifdef _SDL
        typedef SDL_Surface SDL_Window;
        typedef SDL_Surface SDL_Texture;
        #define SDL_GetKeyboardState SDL_GetKeyState
        #define PADKEY key.keysym.sym
    #else
        /*typedef SDL_Texture Surface;*/
        #define SDL_HWSURFACE SDL_SWSURFACE
        #define PADKEY key.keysym.scancode
    #endif

    typedef struct{
        int x;
        int y;
    } Vector2i;

    typedef struct{
        double x;
        double y;
    } Vector2d;

    typedef struct{
        char* PicsPath;
        SDL_Texture* PicsSurface;
        unsigned int SizeX;
        unsigned int SizeY;
    } Pics;

    typedef struct{
        SDL_Window *Screen;
        #ifdef _SDL
            SDL_Surface* Renderer;
        #else
            SDL_Renderer *Renderer;
        #endif
        SDL_Rect InternalResolution;
        Vector2i ScreenResolution;
    } DisplayDevice;

    typedef struct{
        /* Events */
        SDL_Event event;
        bool EventEnabled;

        /* Key Presses */
        const Uint8* KeyStates; /* Pointer to the keypresses */
        
        /* Joystick */
        SDL_Joystick* Joy1; /* Pointers to the Joypad */
        bool JoyEnabled;
    } InputDevice;

    typedef struct{
        char FOO;
    } SoundDevice;

    typedef struct{
        SDL_Texture* FontSurface;
        SDL_Rect Rects[95]; /* Array containing one rect per printable ascii character */
    } BitmapFont;

    SDL_Rect InitRect(int x, int y, int w, int h);
    Vector2d InitVector2d(double x, double y);
    Vector2i InitVector2i(int x, int y);

#endif