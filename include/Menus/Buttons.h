#ifndef _BUTTONS_H
#define _BUTTONS_H

    #include "include.h"
    #include "SceneCommon.h"

    typedef struct{
        Surface* ButtonsSurface;
        char* Label[4];
        BitmapFont* Font;

        char nbOfButtons;
        char selButtonID;
        char clkdButton;
        
        SDL_Rect ObjectDimensions;
        SDL_Rect ButtonState[2];
        Vector2i Coordinates;
        SDL_Rect* Viewport;
        
        Surface* ButtonsLayer;
        DisplayDevice* DDevice;
    } ButtonsContext;

    typedef struct{
        char* Text;
        int YCoordinate;
    } ButtonItem;


    ButtonsContext* InitButtons(DisplayDevice* DDevice, SceneContext* SContext, BitmapFont* Font);
    void AddButton(ButtonsContext* ButtonObject, const char* Label);
    void MoveButtonsToCoordinates(ButtonsContext* ButtonObject, int x, int y);
    void HandleButtonsEvents(ButtonsContext* ButtonObject, SDL_Event* event);
    char GetClkdButtonID(ButtonsContext* ButtonObject);
    void SetSlkdButtonID(ButtonsContext* ButtonObject, char ButtonID);
    void DrawButtons(ButtonsContext* ButtonObject);

#endif