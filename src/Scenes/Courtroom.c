#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"

int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context){

    // Declaration
    SDL_Event event;

    DialogueContext* DiagContext;
    SceneContext* SContext;
    CharacterLayer* CharaLayer;

    int CurrentCharacter;
    bool SceneFlip = false;
    Uint32 Plot = 0;
    int ReturnToDefault;

    // Initialisation
    if (Context == NULL){
        fprintf(stderr, "Courtroom Context non initialised");
        return -1;
    }
    DiagContext = InitDialog(DDevice, Context->Font);
    SetDialogueText(DiagContext, "Mmmmm", 0);

    SContext = InitScene(DDevice, S_Courtroom);

    InitCharacter(DDevice, Phoenix_Wright); // Initialise the character in memory
    InitCharacter(DDevice, Mia_Fey);
    InitCharacter(DDevice, Miles_Edgeworth);

    InitCharacter(DDevice, Court_Desk);

    CharaLayer = NULL;
    InitCharacterLayer(&CharaLayer, SContext);
    AddCharacterToLayer(CharaLayer, Phoenix_Wright, 0, 0, 0, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, Court_Desk, 0, 0, 0, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, Mia_Fey, 2, 1, 0, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, Miles_Edgeworth, 4, 0, 0, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, Court_Desk, 4, 0, 1, DDevice, SContext->SurfaceBounds);

    CurrentCharacter = Phoenix_Wright;
    PlayTrackID(TRK_Courtroom);
    // Main Loop
    while (1){
        // Events Loop
        while(SDL_PollEvent(&event)){
            switch (event.type)
            {
            case SDL_QUIT:
                goto Exit;
                break;

            case SDL_KEYDOWN:
                switch (PADKEY)
                {
                case PAD_A:
                    CharacterPlayAnimation(CurrentCharacter, 0);
                    switch (Plot)
                    {
                    case 0:
                        MoveTile(SContext, 0, 0, 0);
                        CharacterPlayAnimation(Phoenix_Wright, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, "The Defence is ready your honor.", 1);
                        Plot++;
                        break;
                    case 1:
                        MoveTile(SContext, 4, 0, 0);
                        CurrentCharacter = Miles_Edgeworth;
                        CharacterPlayAnimation(Miles_Edgeworth, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, "The Prosecution is ready your honor.", 1);
                        Plot++;
                        break;
                    case 2:
                        MoveTile(SContext, 0, 0, 0);
                        CurrentCharacter = Phoenix_Wright;
                        CharacterPlayAnimation(Phoenix_Wright, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, "Objection !\nThe witness could not have known the statue was a clock !", 2);
                        Plot++;
                        break;
                    
                    case 3:
                        CurrentCharacter = Mia_Fey;
                        MoveTile(SContext, 2, 1, 0);
                        CharacterPlayAnimation(Mia_Fey, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, "Do you have something in mind phoenix ?", 1);
                        Plot++;
                        break;
                    case 4:
                        CurrentCharacter = Phoenix_Wright;
                        MoveTile(SContext, 0, 0, 0);
                        CharacterPlayAnimation(Phoenix_Wright, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, "Your Honor !\nLook at the witness face !", 2);
                        //DeskRect.x = 0;
                        Plot++;
                        break;
                    
                    case 5:
                        BackgroundPlayAnimation(SContext, 1, NULL);
                        Plot++;
                        break;

                    case 6:
                        CurrentCharacter = Mia_Fey;
                        MoveTile(SContext, 2, 1, 0);
                        CharacterPlayAnimation(Mia_Fey, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, "Wow, you just slid across the courtroom !", 2);
                        Plot++;
                        break;
                    
                    default:
                        break;
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

        // Logic
        if ((DiagContext->progress >= ReturnToDefault) && (ReturnToDefault != -1)){
            CharacterPlayAnimation(CurrentCharacter, 0);
            ReturnToDefault = -1;
        }
        // Rendering
        #ifdef _SDL
            DisplayBackground(DDevice, SContext); // Background
            DisplayCharacterLayer(DDevice, CharaLayer);
            //FlipBlitSurface(Desk, NULL, DDevice->Screen, &DeskRect, SceneFlip); // Desk
            Dialogue(IDevice, DiagContext); // Dialog
            SDL_Flip(DDevice->Screen);
        #else
            DisplayBackground(DDevice, SContext); // Background
            DisplayCharacterLayer(DDevice, CharaLayer);
            Dialogue(IDevice, DiagContext); // Dialog
            SDL_RenderPresent(DDevice->Renderer);
        #endif
    }


Exit:
    // Cleaning memory
/*
    #ifdef _SDL
    SDL_FreeSurface(Courtroom);
    #else
    SDL_DestroyTexture(Courtroom);
    #endif
*/
    //free(DiagContext);

    return 0;
}