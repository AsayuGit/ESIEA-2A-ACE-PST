#ifndef _BUTTONS_H
#define _BUTTONS_H

    #include "include.h"
    #include "SceneCommonStructs.h"
    #include "ButtonsStructs.h"

    ButtonsContext* InitButtons(DisplayDevice* DDevice, BackgroundContext* SContext, BitmapFont* Font, int buttonLength, SDL_Rect* ButtonObjectDimensions);
    
    void AddButton(ButtonsContext* ButtonObject, const char* Label);
    void ClearButtons(ButtonsContext* ButtObject);
    
    unsigned char GetNbOfButtons(ButtonsContext* ButtonObject);
    
    void MoveButtonsToCoordinates(ButtonsContext* ButtonObject, int x, int y);
    void HandleButtonsEvents(ButtonsContext* ButtonObject, InputDevice* IDevice);
    unsigned char GetClkdButtonID(ButtonsContext* ButtonObject);
    void SetSlkdButtonID(ButtonsContext* ButtonObject, unsigned char ButtonID);
    void DrawButtons(ButtonsContext* ButtonObject);

    void SetButtonClkSndEffect(ButtonsContext* ButtonObject, unsigned char ButtonID, EffectsPlaylistID NewSndEffect);

#endif