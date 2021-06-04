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

#ifndef _BUTTONSSTRUCTS_H
#define _BUTTONSSTRUCTS_H

    #include "include.h"
    #include "SceneCommonStructs.h"

    typedef struct{
        SDL_Texture* ButtonsSurface;    /* Store the button's spritesheet */
        char* Label[4];                 /* Text printed on the buttons */
        BitmapFont* Font;               /* Font used to display the text */
        Mix_Chunk* ButtonUpDown;        /* Sound effect played when moving the cursor */
        Mix_Chunk* ButtonClicked;       /* Sound effects played when clicked (Default sound effect) */
        Mix_Chunk* ClickedSndEffect[4]; /* Sound effects played when clicked (Per button) */

        unsigned char nbOfButtons;      /* Number of button on screen */
        unsigned char selButtonID;      /* Currently selected button */
        unsigned char clkdButton;       /* Clicked Button (AKA the button that the user last selected) */

        unsigned int buttonLength;      /* Store the length of all buttons */
        unsigned int buttonHeight;      /* Store the height of all buttons */
        
        SDL_Rect ObjectDimensions;      /* Bounds of the whole button object */

        SDL_Rect ButtonLeft[2];         /* Store the rect for the left side of the button [Activated/Desactivated] */
        SDL_Rect ButtonRight[2];        /* Store the rect for the right side of the button [Activated/Desactivated] */
        SDL_Rect ButtonMiddle;          /* Store the rect for the Mille Part of the button [Activated/Desactivated] */

        SDL_Rect ButtonDstRect;         /* Whole button dst rect */
        SDL_Rect LeftDstRect;           /* Where to put the left button part ON SCREEN */
        SDL_Rect RightDstRect;          /* Where to put the right button part ON SCREEN */
        SDL_Rect CenterDstRect;         /* Where to put the center button part ON SCREEN [Variable length] */
        #ifdef _SDL
        int CenterPartLength;
        #endif

        SDL_Rect* Viewport;             /* Scene viewport (Reference) */
        
        DisplayDevice* DDevice;         /* The Engine's display device (Reference) */
    } ButtonsContext;

    typedef struct{
        char* Text;
        int YCoordinate;
    } ButtonItem;

#endif