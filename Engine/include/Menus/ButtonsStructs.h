#ifndef _BUTTONSSTRUCTS_H
#define _BUTTONSSTRUCTS_H

    #include "include.h"
    #include "SceneCommonStructs.h"

    typedef struct{
        SDL_Texture* ButtonsSurface;        /* Store the button's spritesheet */
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

        SDL_Rect* Viewport;             /* Scene viewport */
        
        DisplayDevice* DDevice;         /* The Engine's display device */
    } ButtonsContext;

    typedef struct{
        char* Text;
        int YCoordinate;
    } ButtonItem;

#endif