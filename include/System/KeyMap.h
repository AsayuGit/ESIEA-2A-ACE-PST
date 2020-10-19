#ifndef _KEYMAP
#define _KEYMAP

    #ifdef _SDL

        #define PAD_A SDLK_SPACE

    #else

        #define PAD_A SDL_SCANCODE_SPACE

    #endif


#endif