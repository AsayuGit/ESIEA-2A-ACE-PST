#include "Buttons.h"

ButtonsContext* InitButtons(DisplayDevice* DDevice, SceneContext* SContext, BitmapFont* Font){
    ButtonsContext* BContext;
    Uint32 ColorKey;
    char i;

    // Buttons Texture
    ColorKey = 0xff00ff;
    BContext = (ButtonsContext*)malloc(sizeof(ButtonsContext));
    BContext->ButtonsSurface = LoadSurface(ROOT""TEXTURES"Menus"SL"Buttons"TEX_EXT, DDevice, &ColorKey, false);

    // Buttons Font
    BContext->Font = Font;

    // Buttons Sound effects
    BContext->ButtonUpDown = LoadSoundEffect(EffectPath[CHK_ButtonUpDown]);
    BContext->ButtonClicked = LoadSoundEffect(EffectPath[CHK_ButtonClicked]);
    for (i = 0; i < 4; i++){
        BContext->ClickedSndEffect[i] = BContext->ButtonClicked;
    }


    // Buttons Properties
    BContext->nbOfButtons = 0;
    BContext->selButtonID = 0;
    BContext->clkdButton = -1;

    // Buttons Dimensions
    BContext->ObjectDimensions.x = BContext->ObjectDimensions.y = 0;
    BContext->ObjectDimensions.w = DDevice->ScreenResolution.x;
    BContext->ObjectDimensions.h = DDevice->ScreenResolution.y;
    
    // Buttons States
    BContext->ButtonState[0].x = BContext->ButtonState[1].x = BContext->ButtonState[0].y = 0;
    BContext->ButtonState[0].h = BContext->ButtonState[1].h = BContext->ButtonState[1].y = 30;
    BContext->ButtonState[0].w = BContext->ButtonState[1].w = 148;
    
    // Buttons Position
    BContext->Coordinates.x = BContext->Coordinates.y = 0;
    BContext->Viewport = &(SContext->SrcRect);

    // System Display device
    BContext->DDevice = DDevice;

    // Create the button layer;
    BContext->ButtonsLayer = NULL;
    BContext->ButtonsLayer = CreateTargetSurface(DDevice, BContext->ButtonState[0].w, BContext->ButtonState[0].h);

    return BContext;
}

void AddButton(ButtonsContext* ButtonObject, const char* Label){
    size_t LabelLength;
    if (ButtonObject->nbOfButtons < 4){
        LabelLength = strlen(Label);
        ButtonObject->Label[ButtonObject->nbOfButtons] = (char*)malloc(sizeof(char)*(LabelLength + 1));
        strcpy(ButtonObject->Label[ButtonObject->nbOfButtons], Label);
        ButtonObject->nbOfButtons++;
    }
}

void ClearButtons(ButtonsContext* ButtObject){
    ButtObject->nbOfButtons = 0;
}

char GetNbOfButtons(ButtonsContext* ButtonObject){
    return ButtonObject->nbOfButtons;
}

void MoveButtonsToCoordinates(ButtonsContext* ButtonObject, int x, int y){
    ButtonObject->Coordinates.x = x;
    ButtonObject->Coordinates.y = y;
}

void HandleButtonsEvents(ButtonsContext* ButtonObject, SDL_Event* event){
    switch (event->type)
    {
    case SDL_KEYDOWN:
        switch (P_PADKEY)
        {
        case PAD_UP:
            if (ButtonObject->selButtonID > 0){
                ButtonObject->selButtonID--;
            }else{
                ButtonObject->selButtonID = ButtonObject->nbOfButtons - 1;
            }
            Mix_PlayChannel(-1, ButtonObject->ButtonUpDown, 0);
            break;
        case PAD_DOWN:
            if (ButtonObject->selButtonID < ButtonObject->nbOfButtons - 1){
                ButtonObject->selButtonID++;
            }else{
                ButtonObject->selButtonID = 0;
            }
            Mix_PlayChannel(-1, ButtonObject->ButtonUpDown, 0);
            break;
        case PAD_A:
            ButtonObject->clkdButton = ButtonObject->selButtonID;
            Mix_PlayChannel(-1, ButtonObject->ClickedSndEffect[ButtonObject->clkdButton], 0);
            break;
        }
        break;
    default:
        break;
    }
}

char GetClkdButtonID(ButtonsContext* ButtonObject){
    char BID;

    BID = ButtonObject->clkdButton;
    ButtonObject->clkdButton = -1;
    return BID;
}

void SetSlkdButtonID(ButtonsContext* ButtonObject, char ButtonID){
    ButtonObject->selButtonID = ButtonID;
}

void DrawButtons(ButtonsContext* ButtonObject){ // TO OPTIMIZE !
    const char maxNbOfButtons = 4;
    SDL_Rect ButtonDstRect;
    SDL_Rect EndString;
    SDL_Rect TextPosition;
    int i;
    ButtonItem ButtonsList[maxNbOfButtons];
    char Selected;

    ButtonDstRect = ButtonObject->ButtonState[0]; // Temp ?
    
    ButtonDstRect.x = (ButtonObject->ObjectDimensions.x + ButtonObject->DDevice->ScreenResolution.x - ButtonObject->ButtonState[0].w) / 2;
    ButtonDstRect.y = (ButtonObject->ObjectDimensions.y + ButtonObject->DDevice->ScreenResolution.y - ((ButtonObject->ButtonState[0].h + 5) * ButtonObject->nbOfButtons)) / 2;
    ButtonDstRect.x += ButtonObject->Coordinates.x - ButtonObject->Viewport->x;
    ButtonDstRect.y += ButtonObject->Coordinates.y - ButtonObject->Viewport->y;

    for (i = 0; i < ButtonObject->nbOfButtons; i++){


        #ifdef _SDL
            SDL_FillRect(ButtonObject->ButtonsLayer, NULL, 0x000000);
        #else
            SDL_SetRenderTarget(ButtonObject->DDevice->Renderer, ButtonObject->ButtonsLayer);
            SDL_RenderClear(ButtonObject->DDevice->Renderer);
        #endif
        EndString = gprintf(ButtonObject->DDevice, ButtonObject->Font, ButtonObject->Label[i], &(ButtonObject->ButtonState[0]));
        TextPosition = ButtonDstRect;
        TextPosition.x += ((ButtonObject->ButtonState[0].w - (EndString.x)) >> 1);
        TextPosition.y += ((ButtonObject->ButtonState[0].h - (EndString.h)) >> 1);
        #ifndef _SDL
            SDL_SetRenderTarget(ButtonObject->DDevice->Renderer, NULL);
        #endif
        
        Selected = (i == ButtonObject->selButtonID);
        #ifdef _SDL
            SDL_BlitSurface(ButtonObject->ButtonsSurface, &(ButtonObject->ButtonState[Selected]), ButtonObject->DDevice->Screen, &ButtonDstRect);
        #else
            // Draw button
            SDL_RenderCopy(ButtonObject->DDevice->Renderer, ButtonObject->ButtonsSurface, &(ButtonObject->ButtonState[Selected]), &ButtonDstRect);
            
            // Draw Text
            SDL_RenderCopy(ButtonObject->DDevice->Renderer, ButtonObject->ButtonsLayer, &(ButtonObject->ButtonState[0]), &TextPosition);
        #endif
        ButtonDstRect.y += ButtonObject->ButtonState[0].h + 5; // Temp ?
    }
}

void SetButtonClkSndEffect(ButtonsContext* ButtonObject, char ButtonID, EffectsPlaylistID NewSndEffect){
    if (ButtonObject->ClickedSndEffect[ButtonID] != ButtonObject->ButtonClicked){
        Mix_FreeChunk(ButtonObject->ClickedSndEffect[ButtonID]);
    }
    ButtonObject->ClickedSndEffect[ButtonID] = LoadSoundEffect(EffectPath[NewSndEffect]);
}