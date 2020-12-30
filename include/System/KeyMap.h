#ifndef _KEYMAP
#define _KEYMAP

    #ifdef _SDL

        #define PAD_A SDLK_SPACE
        #define PAD_UP SDLK_UP
        #define PAD_DOWN SDLK_DOWN
        #define PAD_LEFT SDLK_LEFT
        #define PAD_RIGHT SDLK_RIGHT

    #else

        #define PAD_A SDL_SCANCODE_SPACE
        #define PAD_UP SDL_SCANCODE_UP
        #define PAD_DOWN SDL_SCANCODE_DOWN
        #define PAD_LEFT SDL_SCANCODE_LEFT
        #define PAD_RIGHT SDL_SCANCODE_RIGHT

    #endif


#endif