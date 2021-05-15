#ifndef _KEYMAP_H
#define _KEYMAP_H

    #ifdef _SDL
        /* Actions */
        #define PAD_SELECT SDLK_SPACE
        #define PAD_BACK SDLK_LCTRL
        #define PAD_COURTRECORD SDLK_e
        #define PAD_PRESS SDLK_a

        /* Directions */
        #define PAD_UP SDLK_UP
        #define PAD_DOWN SDLK_DOWN
        #define PAD_LEFT SDLK_LEFT
        #define PAD_RIGHT SDLK_RIGHT

        /* System */
        #define PAD_QUIT SDLK_ESCAPE
        #define PAD_FULLSCREEN SDLK_f
        #define PAD_SNAP SDLK_f1

    #else
        /* Actions */
        #define PAD_SELECT SDL_SCANCODE_SPACE
        #define PAD_BACK SDL_SCANCODE_LCTRL
        #define PAD_COURTRECORD SDL_SCANCODE_E
        #define PAD_PRESS SDL_SCANCODE_Q

        /* Directions */
        #define PAD_UP SDL_SCANCODE_UP
        #define PAD_DOWN SDL_SCANCODE_DOWN
        #define PAD_LEFT SDL_SCANCODE_LEFT
        #define PAD_RIGHT SDL_SCANCODE_RIGHT

        /* System */
        #define PAD_QUIT SDL_SCANCODE_ESCAPE
        #define PAD_FULLSCREEN SDL_SCANCODE_F
        #define PAD_SNAP SDL_SCANCODE_F1

    #endif


#endif