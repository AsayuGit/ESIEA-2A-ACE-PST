#include "TitleScreen.h"
#include "SceneCommon.h"
#include "Buttons.h"

int Scene_TitleScreen(DisplayDevice* DDevice, InputDevice* IDevice, BitmapFont* Font){
    int ReturnValue;
    BackgroundContext* SContext;
    ButtonsContext* BContext;

    int Slide;
    char Menu;

    Mix_Chunk* ToMenu;

    SContext = InitBackground(DDevice, "Assets/Anim/TitleScreen.xml");
    BContext = InitButtons(DDevice, SContext, Font, 148, NULL);

    AddButton(BContext, "New Game");
    /* AddButton(BContext, "Continue"); When the save system is implemented */
    AddButton(BContext, "Back");

    MoveButtonsToCoordinates(BContext, DDevice->ScreenResolution.x, 0);
    SetButtonClkSndEffect(BContext, 0, CHK_Gravel);
    SetButtonClkSndEffect(BContext, 1, CHK_ButtonBack);

    Slide = 0; /* Which side of the slide we're on */
    Menu = 0; /* Wether the menu should be activated or not */

    ToMenu = LoadSoundEffect(EffectPath[CHK_ButtonClicked]);

    while (1){
        /* Events Loop */
        while(SDL_PollEvent(&IDevice->event)){
            if (Menu)
                HandleButtonsEvents(BContext, IDevice);
            switch (IDevice->event.type)
            {
            case SDL_QUIT:
                ReturnValue = -1;
                goto Exit;
                break;

            case SDL_KEYDOWN:
                switch (IDevice->event.PADKEY)
                {
                case PAD_SELECT:
                    if (Slide == 0){
                        Mix_PlayChannel(-1, ToMenu, 0);
                        SetSlkdButtonID(BContext, 0);
                        BackgroundPlayAnimation(SContext, 0, &IDevice->EventEnabled);
                        Slide = 1;
                    } else {
                        switch (GetClkdButtonID(BContext))
                        {
                        case 0:
                            ReturnValue = 0;
                            goto Exit;
                            break;
                        case 1:
                            BackgroundPlayAnimation(SContext, 1, &IDevice->EventEnabled);
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

        if (IDevice->EventEnabled){
            Menu = Slide;
        }

        /* Rendering */
        #ifdef _SDL
            DisplayBackground(DDevice, SContext); /* Background */
            DrawButtons(BContext);

            SDL_Flip(DDevice->Screen);
        #else
            DisplayBackground(DDevice, SContext); /* Background */
            DrawButtons(BContext);
            
            SDL_RenderPresent(DDevice->Renderer);
        #endif
    }

Exit:
    if (SContext)
        FreeBackground(SContext);

    return ReturnValue;
}