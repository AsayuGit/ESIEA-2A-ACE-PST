#ifndef _BUTTONS_H
#define _BUTTONS_H

    #include "include.h"
    #include "SceneCommonStructs.h"

    typedef struct{
        Surface* ButtonsSurface; // Store the button's spritesheet
        char* Label[4];
        BitmapFont* Font;
        Mix_Chunk* ButtonUpDown;
        Mix_Chunk* ButtonClicked;
        Mix_Chunk* ClickedSndEffect[4];

        char nbOfButtons;
        char selButtonID;
        char clkdButton;
        
        SDL_Rect ObjectDimensions;
        SDL_Rect ButtonState[2]; // Store the rect for the button (Activated / Desactivated)
        Vector2i Coordinates;
        SDL_Rect* Viewport;
        
        Surface* ButtonsLayer;
        DisplayDevice* DDevice;
    } ButtonsContext;

    typedef struct{
        char* Text;
        int YCoordinate;
    } ButtonItem;


    ButtonsContext* InitButtons(DisplayDevice* DDevice, SceneContext* SContext, BitmapFont* Font, SDL_Rect* ButtonObjectDimensions);
    
    void AddButton(ButtonsContext* ButtonObject, const char* Label);
    void ClearButtons(ButtonsContext* ButtObject);
    
    char GetNbOfButtons(ButtonsContext* ButtonObject);
    
    void MoveButtonsToCoordinates(ButtonsContext* ButtonObject, int x, int y);
    void HandleButtonsEvents(ButtonsContext* ButtonObject, SDL_Event* event);
    char GetClkdButtonID(ButtonsContext* ButtonObject);
    void SetSlkdButtonID(ButtonsContext* ButtonObject, char ButtonID);
    void DrawButtons(ButtonsContext* ButtonObject);

    void SetButtonClkSndEffect(ButtonsContext* ButtonObject, char ButtonID, EffectsPlaylistID NewSndEffect);

#endif