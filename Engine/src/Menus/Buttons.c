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

    /* Buttons Dimensions (Takes the whole screen if not specified) */
    if (ButtonObjectDimensions){
        BContext->ObjectDimensions = *ButtonObjectDimensions;
    }else {
        BContext->ObjectDimensions.x = BContext->ObjectDimensions.y = 0;
        BContext->ObjectDimensions.w = DDevice->ScreenResolution.x;
        BContext->ObjectDimensions.h = DDevice->ScreenResolution.y;
    }

    /* Buttons Parts / States */
    BContext->ButtonLeft[0].x = 0;
    BContext->ButtonLeft[0].y = 0;
    BContext->ButtonLeft[0].w = 11;
    BContext->ButtonLeft[0].h = BContext->buttonHeight;

    BContext->ButtonLeft[1].x = 23;
    BContext->ButtonLeft[1].y = 0;
    BContext->ButtonLeft[1].w = 11;
    BContext->ButtonLeft[1].h = BContext->buttonHeight;

    BContext->ButtonRight[0].x = 12;
    BContext->ButtonRight[0].y = 0;
    BContext->ButtonRight[0].w = 11;
    BContext->ButtonRight[0].h = BContext->buttonHeight;

    BContext->ButtonRight[1].x = 35;
    BContext->ButtonRight[1].y = 0;
    BContext->ButtonRight[1].w = 11;
    BContext->ButtonRight[1].h = BContext->buttonHeight;

    BContext->ButtonMiddle.x = BContext->ButtonLeft[0].x + BContext->ButtonLeft[0].w + 1;
    BContext->ButtonMiddle.y = 0;
    BContext->ButtonMiddle.w = 1;
    BContext->ButtonMiddle.h = BContext->buttonHeight;

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

/* TO OPTIMIZE ! (Maybe its about the render to texture thingy) (Thats not more) */
void DrawButtons(ButtonsContext* ButtonObject){
    SDL_Rect ButtonDstRect; /* Where to put the button ON SCREEN */
    SDL_Rect LeftDstRect;   /* Where to put the left button part ON SCREEN */
    SDL_Rect RightDstRect;  /* Where to put the right button part ON SCREEN */
    SDL_Rect CenterDstRect; /* Where to put the center button part ON SCREEN [Variable length] */

    SDL_Rect TextPosition;
    /*ButtonItem ButtonsList[NBOFBUTTONS]; */
    unsigned char Selected;
    unsigned char i;
    #ifdef _SDL
        unsigned int j;
        unsigned int CenterPartLength;
    #endif

    /* Whole button coordinates */
    ButtonDstRect.x = ButtonObject->ObjectDimensions.x + ((ButtonObject->ObjectDimensions.w - ButtonObject->buttonLength) / 2) - ButtonObject->Viewport->x;
    ButtonDstRect.y = ButtonObject->ObjectDimensions.y + ((ButtonObject->ObjectDimensions.h - ((ButtonObject->buttonHeight + 5) * ButtonObject->nbOfButtons)) / 2) - ButtonObject->Viewport->y;
    ButtonDstRect.w = ButtonObject->buttonLength;
    ButtonDstRect.h = ButtonObject->buttonHeight;

    /* Left button part rect */
    LeftDstRect.x = ButtonDstRect.x;
    LeftDstRect.w = ButtonObject->ButtonLeft[0].w;
    LeftDstRect.h = ButtonObject->ButtonLeft[0].h;

    /* Right button part rect */
    RightDstRect.w = ButtonObject->ButtonRight[0].w;
    RightDstRect.h = ButtonObject->ButtonRight[0].h;
    RightDstRect.x = ButtonDstRect.x + ButtonObject->buttonLength - RightDstRect.w;


    /* Center button part rect */
    CenterDstRect.h = LeftDstRect.h;
    #ifdef _SDL
        CenterDstRect.w = 1;
        CenterPartLength = ButtonObject->buttonLength - (ButtonObject->ButtonLeft[0].w << 1);
    #else
        CenterDstRect.w = ButtonObject->buttonLength - (ButtonObject->ButtonLeft[0].w << 1);
    #endif

    for (i = 0; i < ButtonObject->nbOfButtons; i++){ /* For Each button to draw */

        Selected = (unsigned char)(i == ButtonObject->selButtonID); /* If the button is selected */
        /* Draw button */
        LeftDstRect.y = RightDstRect.y = CenterDstRect.y =  ButtonDstRect.y; /* Update Position */
        CenterDstRect.x = LeftDstRect.x + LeftDstRect.w;
        #ifdef _SDL
            for (j = 0; j < CenterPartLength; j++){
                SDL_BlitSurface(ButtonObject->ButtonsSurface, &(ButtonObject->ButtonLeft[Selected]), ButtonObject->DDevice->Screen, &LeftDstRect); /* Left part */
                SDL_BlitSurface(ButtonObject->ButtonsSurface, &(ButtonObject->ButtonRight[Selected]), ButtonObject->DDevice->Screen, &RightDstRect); /* Right part */
                SDL_BlitSurface(ButtonObject->ButtonsSurface, &(ButtonObject->ButtonMiddle), ButtonObject->DDevice->Screen, &CenterDstRect); /* Center part */
                CenterDstRect.x++;
            }
        #else
            SDL_RenderCopy(ButtonObject->DDevice->Renderer, ButtonObject->ButtonsSurface, &(ButtonObject->ButtonLeft[Selected]), &LeftDstRect); /* Left part */
            SDL_RenderCopy(ButtonObject->DDevice->Renderer, ButtonObject->ButtonsSurface, &(ButtonObject->ButtonRight[Selected]), &RightDstRect); /* Right part */
            SDL_RenderCopy(ButtonObject->DDevice->Renderer, ButtonObject->ButtonsSurface, &(ButtonObject->ButtonMiddle), &CenterDstRect); /* Center part */
        #endif

        TextPosition = ButtonDstRect;
        TextPosition.x += ((ButtonObject->buttonLength - (gstrlen(ButtonObject->Font, ButtonObject->Label[i], 1).x)) >> 1);
        TextPosition.y += ((ButtonObject->buttonHeight - (ButtonObject->Font->Rects[0].h)) >> 1);

        gprintf(ButtonObject->DDevice, ButtonObject->Font, ButtonObject->Label[i], 1, &(TextPosition));

        ButtonDstRect.y += ButtonObject->buttonHeight + 5; /* Go to next button */
    }
}

void SetButtonClkSndEffect(ButtonsContext* ButtonObject, unsigned char ButtonID, EffectsPlaylistID NewSndEffect){
    if (ButtonObject->ClickedSndEffect[ButtonID] != ButtonObject->ButtonClicked){
        Mix_FreeChunk(ButtonObject->ClickedSndEffect[ButtonID]);
    }
    ButtonObject->ClickedSndEffect[ButtonID] = LoadSoundEffect(EffectPath[NewSndEffect]);
}