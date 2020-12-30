#include "TitleScreen.h"
#include "SceneCommon.h"
#include "Buttons.h"

int Scene_TitleScreen(DisplayDevice* DDevice, InputDevice* IDevice, BitmapFont* Font){
    int ReturnValue;
    SDL_Event event;
    SceneContext* SContext;
    ButtonsContext* BContext;

    int Slide;
    char AnimComplete;
    char Menu;

    SContext = InitScene(DDevice, S_TitleScreen);
    BContext = InitButtons(DDevice, SContext, Font);

    AddButton(BContext, "New Game");
    //AddButton(BContext, "Continue");
    AddButton(BContext, "Back");

    MoveButtonsToCoordinates(BContext, DDevice->ScreenResolution.x, 0);

    Slide = 0; // Which side of the slide we're on
    AnimComplete = 0; // Wether the animation is done or not
    Menu = 0; // Wether the menu should be activated or not

    while (1){
        // Events Loop
        while(SDL_PollEvent(&event)){
            if (Menu)
                HandleButtonsEvents(BContext, &event);
            switch (event.type)
            {
            case SDL_QUIT:
                ReturnValue = -1;
                goto Exit;
                break;

            case SDL_KEYDOWN:
                switch (PADKEY)
                {
                case PAD_A:
                    if (Slide == 0){
                        SetSlkdButtonID(BContext, 0);
                        BackgroundPlayAnimation(SContext, 0, &AnimComplete);
                        Slide = 1;
                    } else {
                        switch (GetClkdButtonID(BContext))
                        {
                        case 0:
                            ReturnValue = 0;
                            goto Exit;
                            break;
                        case 1:
                            BackgroundPlayAnimation(SContext, 1, &AnimComplete);
                            Slide = 0;
                            Menu = 0;
                            break;
                        
                        default:
                            break;
                        }
                    }
                    break;
                
                default:
                    break;
                }

                break;
            default:
                break;
            }
            
        }

        if (AnimComplete){
            AnimComplete = 0;
            Menu = Slide;
        }

        // Rendering
        #ifdef _SDL
            DisplayBackground(DDevice, SContext); // Background
            DrawButtons(BContext);

            SDL_Flip(DDevice->Screen);
        #else
            DisplayBackground(DDevice, SContext); // Background
            DrawButtons(BContext);
            
            SDL_RenderPresent(DDevice->Renderer);
        #endif
    }

Exit:

    return ReturnValue;
}