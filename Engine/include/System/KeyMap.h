#ifndef _KEYMAP
#define _KEYMAP

    #ifdef _SDL
        /* Actions */
        #define PAD_SELECT SDLK_SPACE
        #define PAD_BACK SDLK_LCTRL
        #define PAD_COURTRECORD SDLK_e

        /* Directions */
        #define PAD_UP SDLK_UP
        #define PAD_DOWN SDLK_DOWN
        #define PAD_LEFT SDLK_LEFT
        #define PAD_RIGHT SDLK_RIGHT

    #else
        /* Actions */
        #define PAD_SELECT SDL_SCANCODE_SPACE
        #define PAD_BACK SDL_SCANCODE_LCTRL
        #define PAD_COURTRECORD SDL_SCANCODE_E

        /* Directions */
        #define PAD_UP SDL_SCANCODE_UP
        #define PAD_DOWN SDL_SCANCODE_DOWN
        #define PAD_LEFT SDL_SCANCODE_LEFT
        #define PAD_RIGHT SDL_SCANCODE_RIGHT

    #endif


#endif