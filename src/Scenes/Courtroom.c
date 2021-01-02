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
    DiagContext = InitDialog(DDevice, Context->MainFont, Context->NameFont);
    SContext = InitScene(DDevice, S_Courtroom);

    InitCharacter(DDevice, Phoenix_Wright); // Initialise the character in memory
    InitCharacter(DDevice, Mia_Fey);
    InitCharacter(DDevice, Miles_Edgeworth);
    InitCharacter(DDevice, Judge);

    InitCharacter(DDevice, Court_Desk);

    CharaLayer = NULL;
    InitCharacterLayer(&CharaLayer, SContext);
    AddCharacterToLayer(CharaLayer, Phoenix_Wright, 0, 0, 0, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, Court_Desk, 0, 0, 0, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, Mia_Fey, 2, 1, 0, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, Miles_Edgeworth, 4, 0, 0, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, Court_Desk, 4, 0, 1, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, Judge, 1, 1, 0, DDevice, SContext->SurfaceBounds);
    PlayTrackID(TRK_Courtroom);


    // Scene setup
    CurrentCharacter = Judge;
    MoveTile(SContext, 1, 1, 0);
    CharacterPlayAnimation(Judge, 1);
    ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "The court is now in session\nfor the trial of Ms. DEMO.", 2);

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
                        CurrentCharacter = Miles_Edgeworth;
                        MoveTile(SContext, 4, 0, 0);
                        CharacterPlayAnimation(Miles_Edgeworth, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution is\nready, Your Honor.", 2);
                        Plot++;
                        break;
                    case 1:
                        CurrentCharacter = Phoenix_Wright;
                        BackgroundPlayAnimation(SContext, 2, NULL);
                        CharacterPlayAnimation(Phoenix_Wright, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Phoenix_Wright), "The defence\nis ready, Your Honor.", 2);
                        Plot++;
                        break;
                    case 2:
                        CurrentCharacter = Judge;
                        MoveTile(SContext, 1, 1, 0);
                        CharacterPlayAnimation(Judge, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "Mr. Edgeworth.\nPlease give the court\nyour opening statement.", 2);
                        Plot++;
                        break;
                    case 3:
                        CurrentCharacter = Miles_Edgeworth;
                        MoveTile(SContext, 4, 0, 0);
                        CharacterPlayAnimation(Miles_Edgeworth, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "Thank you, Your Honor.\nThe defandant, Ms. DEMO,\nwas at the scene of the crime.", 2);
                        Plot++;
                        break;
                    case 4:
                        //CurrentCharacter = Miles_Edgeworth;
                        //MoveTile(SContext, 4, 0, 0);
                        CharacterPlayAnimation(Miles_Edgeworth, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution has evidence\nshe committed this murder...", 1);
                        Plot++;
                        break;
                    case 5:
                        //CurrentCharacter = Miles_Edgeworth;
                        //MoveTile(SContext, 4, 0, 0);
                        CharacterPlayAnimation(Miles_Edgeworth, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "and we have a witness\nwho saw her do it.", 1);
                        Plot++;
                        break;
                    case 6:
                        //CurrentCharacter = Miles_Edgeworth;
                        //MoveTile(SContext, 4, 0, 0);
                        CharacterPlayAnimation(Miles_Edgeworth, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution sees no\nreason to doubt the facts\nof this case, Your Honor.", 1);
                        Plot++;
                        break;
                    case 7:
                        CurrentCharacter = Judge;
                        MoveTile(SContext, 1, 1, 0);
                        CharacterPlayAnimation(Judge, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "I see.", 2);
                        Plot++;
                        break;
                    case 8:
                        //CurrentCharacter = Judge;
                        //MoveTile(SContext, 1, 1, 0);
                        CharacterPlayAnimation(Judge, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "Thank you, Mr. Edgeworth.\nLet's begin then.", 1);
                        Plot++;
                        break;
                    case 9:
                        //CurrentCharacter = Judge;
                        //MoveTile(SContext, 1, 1, 0);
                        CharacterPlayAnimation(Judge, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "You may call your\nfirst witness.", 1);
                        Plot++;
                        break;
                    case 10:
                        CurrentCharacter = Miles_Edgeworth;
                        MoveTile(SContext, 4, 0, 0);
                        CharacterPlayAnimation(Miles_Edgeworth, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution calls the\nchief officer at the scene,\nDetective Gumshoe!", 2);
                        Plot++;
                        break;
                    // end of plot
                    case 11:
                        CurrentCharacter = Mia_Fey;
                        MoveTile(SContext, 2, 1, 0);
                        CharacterPlayAnimation(Mia_Fey, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Mia_Fey), "Get Ready Phoenix !\nThis will be our last\nchance !", 2);
                        Plot++;
                        break;
                    case 12:
                        CurrentCharacter = Mia_Fey;
                        MoveTile(SContext, 2, 1, 0);
                        CharacterPlayAnimation(Mia_Fey, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Mia_Fey), "On this note, this conclude this\nbrief demo.", 1);
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