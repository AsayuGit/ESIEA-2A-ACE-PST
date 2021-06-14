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

#ifndef _KEYMAP_H
#define _KEYMAP_H

	#ifdef _XBOX
		#define JOY_A 0
		#define JOY_B 1
		#define JOY_X 2
		#define JOY_Y 3
		#define JOY_BLACK 4
		#define JOY_WHITE 5
		#define JOY_LTRIGGER 6
		#define JOY_RTRIGGER 7
		#define JOY_START 8
		#define JOY_BACK 9
		#define JOY_LSTICK 10
		#define JOY_RSTICK 11
		#define JOY_UP 12
		#define JOY_DOWN 13
		#define JOY_LEFT 14
		#define JOY_RIGHT 15
	#else
		#define JOY_A 2 // Temp Values 
		#define JOY_B 1
		#define JOY_X 3
		#define JOY_Y 0
		#define JOY_BLACK 4
		#define JOY_WHITE 5
		#define JOY_LTRIGGER 6
		#define JOY_RTRIGGER 7
		#define JOY_START 9
		#define JOY_BACK 8
		#define JOY_LSTICK 10
		#define JOY_RSTICK 11
		#define JOY_UP 12
		#define JOY_DOWN 13
		#define JOY_LEFT 14
		#define JOY_RIGHT 15
	#endif

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
        #define PAD_MAINMENU SDLK_f4

	#elif defined (_XBOX)
        /* Actions */
        #define PAD_SELECT JOY_A
        #define PAD_BACK JOY_B
        #define PAD_COURTRECORD JOY_Y
        #define PAD_PRESS JOY_X

        /* Directions */
        #define PAD_UP JOY_UP
        #define PAD_DOWN JOY_DOWN
        #define PAD_LEFT JOY_LEFT
        #define PAD_RIGHT JOY_RIGHT

        /* System */
        #define PAD_QUIT SDL_SCANCODE_ESCAPE
        #define PAD_FULLSCREEN SDL_SCANCODE_F
        #define PAD_SNAP SDL_SCANCODE_F1
        #define PAD_MAINMENU SDL_SCANCODE_F4

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
        #define PAD_MAINMENU SDL_SCANCODE_F4

    #endif


#endif