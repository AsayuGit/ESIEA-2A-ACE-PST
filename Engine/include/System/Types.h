#ifndef _TYPES
#define _TYPES

    #include <stdbool.h>

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

#endif