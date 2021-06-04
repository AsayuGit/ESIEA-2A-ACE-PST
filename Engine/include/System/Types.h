/*
    Mia's Legacy is a Ace Attorney fangame taking place directly
    after the first game in the serie. All code in this repo have
    been written from scratch in ANSI C using SDL and libxml2.
    This game is designed to run on Linux Windows and the og Xbox

    Copyright (C) 2021 Killian RAIMBAUD (killian.rai@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef _TYPES_H
#define _TYPES_H
	
	#ifdef _XBOX
		typedef enum{
			false,
			true
		} bool;
	#else
		#include <stdbool.h>
	#endif

    #ifdef _SDL
        #include <SDL/SDL.h>
    #elif defined(_XBOX)
		#include <SDL.h>
	#else
        #include <SDL2/SDL.h>
    #endif

    /* Uniformity between SDL1.2 and SDL2 */
    #ifdef _SDL
        typedef SDL_Surface SDL_Window;
        typedef SDL_Surface SDL_Texture;
        #define SDL_GetKeyboardState SDL_GetKeyState
        #define PADKEY key.keysym.sym
		#define PAD_KEYDOWN SDL_KEYDOWN
	#elif defined(_XBOX)
		#define SDL_HWSURFACE SDL_SWSURFACE
        #define PADKEY jbutton.button
		#define PAD_KEYDOWN SDL_JOYBUTTONDOWN
    #else
        #define SDL_HWSURFACE SDL_SWSURFACE
        #define PADKEY key.keysym.scancode
		#define PAD_KEYDOWN SDL_KEYDOWN
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

        SDL_Rect Frame[4];

        SDL_Rect InternalResolution;
        Vector2i ScreenResolution;
        unsigned int IRScalar;
        bool OffScreenRender;
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
    SDL_Rect RectPlusVector2i(SDL_Rect* Rect, Vector2i* Vect);
    SDL_Rect RectMinusVector2i(SDL_Rect* Rect, Vector2i* Vect);

#endif