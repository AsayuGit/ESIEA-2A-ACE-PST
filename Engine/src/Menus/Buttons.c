#include "Buttons.h"

#define NBOFBUTTONS 4

ButtonsContext* InitButtons(DisplayDevice* DDevice, BackgroundContext* SContext, BitmapFont* Font, int buttonLength, SDL_Rect* ButtonObjectDimensions){
    ButtonsContext* BContext;
    unsigned char i;

    /* Buttons Texture */
    BContext = (ButtonsContext*)malloc(sizeof(ButtonsContext));
    BContext->ButtonsSurface = LoadSurface(ROOT""TEXTURES"Menus"SL"Buttons"TEX_EXT, DDevice, 0xff00ff, SURFACE_KEYED);

    /* Buttons Font */
    BContext->Font = Font;

    /* Buttons Sound effects */
    BContext->ButtonUpDown = LoadSoundEffect(EffectPath[CHK_ButtonUpDown]);
    BContext->ButtonClicked = LoadSoundEffect(EffectPath[CHK_ButtonClicked]);
    for (i = 0; i < 4; i++){
        BContext->ClickedSndEffect[i] = BContext->ButtonClicked;
    }

    /* Buttons Properties */
    BContext->nbOfButtons = 0;
    BContext->selButtonID = 0;
    BContext->clkdButton = 0;
    BContext->buttonHeight = 30;
    BContext->buttonLength = buttonLength;
    BContext->ButtonDstRect.w = BContext->buttonLength;
    BContext->ButtonDstRect.h = BContext->buttonHeight;

    /* Buttons Dimensions (Takes the whole screen if not specified) */
    if (ButtonObjectDimensions){
        BContext->ObjectDimensions = *ButtonObjectDimensions;
    }else {
        BContext->ObjectDimensions.x = BContext->ObjectDimensions.y = 0;
        BContext->ObjectDimensions.w = BASE_RESOLUTION_X;
        BContext->ObjectDimensions.h = BASE_RESOLUTION_Y;
    }

    /* Buttons Src rects / States */
    BContext->ButtonLeft[0] = InitRect(0, 0, 11, BContext->buttonHeight);
    BContext->ButtonLeft[1] = InitRect(23, 0, 11, BContext->buttonHeight);
    BContext->ButtonRight[0] = InitRect(12, 0, 11, BContext->buttonHeight);
    BContext->ButtonRight[1] = InitRect(35, 0, 11, BContext->buttonHeight);

    BContext->ButtonMiddle = InitRect(
        BContext->ButtonLeft[0].x + BContext->ButtonLeft[0].w + 1,
        0, 1, BContext->buttonHeight
    );

    /* Buttons Dst rects */
    BContext->LeftDstRect.w = BContext->ButtonLeft[0].w;
    BContext->LeftDstRect.h = BContext->ButtonLeft[0].h;

    BContext->RightDstRect.w = BContext->ButtonRight[0].w;
    BContext->RightDstRect.h = BContext->ButtonRight[0].h;

    BContext->CenterDstRect.h = BContext->LeftDstRect.h;
    #ifdef _SDL
        BContext->CenterDstRect.w = 1;
        BContext->CenterPartLength = BContext->buttonLength - (BContext->ButtonLeft[0].w << 1);
    #else
        BContext->CenterDstRect.w = BContext->buttonLength - (BContext->ButtonLeft[0].w << 1);
    #endif

    /* Buttons Viewport */
    BContext->Viewport = &(SContext->SrcRect);

    /* System Display device */
    BContext->DDevice = DDevice;

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

unsigned char GetNbOfButtons(ButtonsContext* ButtonObject){
    return ButtonObject->nbOfButtons;
}

void MoveButtonsToCoordinates(ButtonsContext* ButtonObject, int x, int y){
    ButtonObject->ObjectDimensions.x = x;
    ButtonObject->ObjectDimensions.y = y;
}

void HandleButtonsEvents(ButtonsContext* ButtonObject, InputDevice* IDevice){
    switch (IDevice->event.type)
    {
    case SDL_KEYDOWN:
        switch (IDevice->event.PADKEY)
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
        case PAD_SELECT:
            ButtonObject->clkdButton = ButtonObject->selButtonID;
            Mix_PlayChannel(-1, ButtonObject->ClickedSndEffect[ButtonObject->clkdButton], 0);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

unsigned char GetClkdButtonID(ButtonsContext* ButtonObject){
    return ButtonObject->clkdButton;
}

void SetSlkdButtonID(ButtonsContext* ButtonObject, unsigned char ButtonID){
    ButtonObject->selButtonID = ButtonID;
}

void DrawButtons(ButtonsContext* ButtonObject){ /* Ran each frame */
    SDL_Rect TextPosition;
    unsigned char Selected;
    unsigned char i;
    #ifdef _SDL
        unsigned int j;
    #endif
    int YButtonOffset;

    YButtonOffset = ButtonObject->buttonHeight + 5;
    /* Whole button coordinates */
    ButtonObject->LeftDstRect.x = ButtonObject->ButtonDstRect.x = ButtonObject->ObjectDimensions.x + ((ButtonObject->ObjectDimensions.w - ButtonObject->ButtonDstRect.w) >> 1) - ButtonObject->Viewport->x;
    ButtonObject->ButtonDstRect.y = ButtonObject->ObjectDimensions.y + ((ButtonObject->ObjectDimensions.h - (YButtonOffset * ButtonObject->nbOfButtons)) >> 1) - ButtonObject->Viewport->y;

    ButtonObject->RightDstRect.x = ButtonObject->ButtonDstRect.x + ButtonObject->ButtonDstRect.w - ButtonObject->RightDstRect.w;

    for (i = 0; i < ButtonObject->nbOfButtons; i++){ /* For Each button to draw */

        Selected = (unsigned char)(i == ButtonObject->selButtonID); /* If the button is selected */
        /* Draw button */
        ButtonObject->LeftDstRect.y = ButtonObject->RightDstRect.y = ButtonObject->CenterDstRect.y = ButtonObject->ButtonDstRect.y; /* Update Position */
        ButtonObject->CenterDstRect.x = ButtonObject->LeftDstRect.x + ButtonObject->LeftDstRect.w;
        
        ScaledDraw(ButtonObject->DDevice, ButtonObject->ButtonsSurface, &(ButtonObject->ButtonLeft[Selected]), &ButtonObject->LeftDstRect); /* Left part */
        ScaledDraw(ButtonObject->DDevice, ButtonObject->ButtonsSurface, &(ButtonObject->ButtonRight[Selected]), &ButtonObject->RightDstRect); /* Right part */
        #ifdef _SDL
            for (j = 0; j < ButtonObject->CenterPartLength; j++){
        #endif
                ScaledDraw(ButtonObject->DDevice, ButtonObject->ButtonsSurface, &(ButtonObject->ButtonMiddle), &ButtonObject->CenterDstRect); /* Center part */
        #ifdef _SDL
                ButtonObject->CenterDstRect.x++;
            }
        #endif
        
        TextPosition = ButtonObject->ButtonDstRect;
        TextPosition.x += (ButtonObject->ButtonDstRect.w - gstrlen(ButtonObject->Font, ButtonObject->Label[i], 1).x) >> 1;
        TextPosition.y += (ButtonObject->ButtonDstRect.h - ButtonObject->Font->Rects[0].h) >> 1;

        gprintf(ButtonObject->DDevice, ButtonObject->Font, ButtonObject->Label[i], 1, &(TextPosition));

        ButtonObject->ButtonDstRect.y += YButtonOffset; /* Go to next button */
    }
}

void SetButtonClkSndEffect(ButtonsContext* ButtonObject, unsigned char ButtonID, EffectsPlaylistID NewSndEffect){
    if (ButtonObject->ClickedSndEffect[ButtonID] != ButtonObject->ButtonClicked){
        Mix_FreeChunk(ButtonObject->ClickedSndEffect[ButtonID]);
    }
    ButtonObject->ClickedSndEffect[ButtonID] = LoadSoundEffect(EffectPath[NewSndEffect]);
}