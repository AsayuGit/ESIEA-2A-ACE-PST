#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"

void CourtroomScenarioB(SceneContext* SContext, DialogueContext* DiagContext, int* Plot, int* CurrentCharacter, int* ReturnToDefault){
    switch (*Plot)
    {
    case -1:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        CharacterPlayAnimation(Judge, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "The court is now in session\nfor the trial of Ms. DEMO.", 2);
        (*Plot)++;
        break;
    case 0:
        *CurrentCharacter = Miles_Edgeworth;
        MoveTile(SContext, 1, 0);
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution is\nready, Your Honor.", 2);
        (*Plot)++;
        break;
    case 1:
        *CurrentCharacter = Phoenix_Wright;
        BackgroundPlayAnimation(SContext, 2, NULL);
        CharacterPlayAnimation(Phoenix_Wright, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Phoenix_Wright), "The defence\nis ready, Your Honor.", 2);
        (*Plot)++;
        break;
    case 2:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        CharacterPlayAnimation(Judge, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "Mr. Edgeworth.\nPlease give the court\nyour opening statement.", 2);
        (*Plot)++;
        break;
    case 3:
        *CurrentCharacter = Miles_Edgeworth;
        MoveTile(SContext, 1, 0);
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "Thank you, Your Honor.\nThe defandant, Ms. DEMO,\nwas at the scene of the crime.", 2);
        (*Plot)++;
        break;
    case 4:
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution has evidence\nshe committed this murder...", 1);
        (*Plot)++;
        break;
    case 5:
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "and we have a witness\nwho saw her do it.", 1);
        (*Plot)++;
        break;
    case 6:
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution sees no\nreason to doubt the facts\nof this case, Your Honor.", 1);
        (*Plot)++;
        break;
    case 7:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        CharacterPlayAnimation(Judge, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "I see.", 2);
        (*Plot)++;
        break;
    case 8:
        CharacterPlayAnimation(Judge, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "Thank you, Mr. Edgeworth.\nLet's begin then.", 1);
        (*Plot)++;
        break;
    case 9:
        CharacterPlayAnimation(Judge, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Judge), "You may call your\nfirst witness.", 1);
        (*Plot)++;
        break;
    case 10:
        *CurrentCharacter = Miles_Edgeworth;
        MoveTile(SContext, 1, 0);
        CharacterPlayAnimation(Miles_Edgeworth, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Miles_Edgeworth), "The prosecution calls the\nchief officer at the scene,\nDetective Gumshoe!", 2);
        (*Plot)++;
        break;
    // end of plot
    case 11:
        *CurrentCharacter = Mia_Fey;
        MoveTile(SContext, 5, 0);
        CharacterPlayAnimation(Mia_Fey, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Mia_Fey), "Get Ready Phoenix !\nThis will be our last\nchance !", 2);
        (*Plot)++;
        break;
    case 12:
        CharacterPlayAnimation(Mia_Fey, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(Mia_Fey), "On this note, this conclude this\nbrief demo.", 1);
        (*Plot)++;
        break;
    
    default:
        break;
    }
}

void CourtroomScenarioA(SceneContext* SContext, DialogueContext* DiagContext, int* Plot, int* CurrentCharacter, int* ReturnToDefault){
    switch (*Plot)
    {
    case 0:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The court is now in session for\nthe trial of Mr. Larry Butz.", 1);
        break;
    
    case 1:
        *CurrentCharacter = Miles_Edgeworth;
        MoveTile(SContext, 1, 0);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The prosecution is ready,\nYour Honor.", 1);
        break;

    case 2:
        *CurrentCharacter = Phoenix_Wright;
        BackgroundPlayAnimation(SContext, 2, NULL);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The, um, defense is ready,\nYour Honor.", 1);
        break;

    case 3:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Ahem.", 1);
        break;
    case 4:
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Mr. Wright?", 1);
        break;
    case 5:
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "This is your first trial,\nis it not?", 1);
        break;

    case 6:
        *CurrentCharacter = Phoenix_Wright;
        MoveTile(SContext, 0, 0);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Y-Yes, Your Honor.\nI'm, um, a little nervous.", 1);
        break;
    
    case 7:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Your conduct during this\ntrial will decide the fate\nof your client.", 1);
        break;
    case 8:
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Murder is a serious charge.\nFor your client's sake, I hope\nyou can control your nerves.", 1);
        break;

    case 9:
        *CurrentCharacter = Phoenix_Wright;
        MoveTile(SContext, 0, 0);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Thank... thank you, Your Honor.", 1);
        break;

    case 10:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "...", 1);
        break;
    case 11:
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Mr. Wright, given the circumstances...", 1);
        break;
    case 12:
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "I think we should have a test to\nascertain your readiness.", 1);
        break; 

    case 13:
        *CurrentCharacter = Phoenix_Wright;
        MoveTile(SContext, 0, 0);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Yes, Your Honor.", 1);
        break;
    case 14:
        CharacterPlayAnimation(*CurrentCharacter, 2); // Glup Animation
        SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "(Glup... Hands shaking...\nEyesight... fading...)", 1);
        break;
    
    case 15:
        *CurrentCharacter = Judge;
        MoveTile(SContext, 4, 0);
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "The test will consist of a\nfew simple questions. Answer\nthem clearly and concisely", 1);
        break;
    case 16:
        CharacterPlayAnimation(*CurrentCharacter, 1);
        *ReturnToDefault = SetDialogueText(DiagContext, GetCharacterName(*CurrentCharacter), "Please state the name of\nthe defendant in this case.", 1);
        break;
    /// QUESTION

    default:
        break;
    }
    (*Plot)++;
}

int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context){

    // Declaration
    SDL_Event event;

    DialogueContext* DiagContext;
    SceneContext* SContext;
    CharacterLayer* CharaLayer;

    int CurrentCharacter;
    bool SceneFlip = false;
    int Plot = 0;
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
    AddCharacterToLayer(CharaLayer, Phoenix_Wright, SContext, 0, 0, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, Court_Desk, SContext, 0, 0, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, Mia_Fey, SContext, 5, 0, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, Miles_Edgeworth, SContext, 1, 0, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, Court_Desk, SContext, 1, 1, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, Judge, SContext, 4, 0, DDevice, SContext->SurfaceBounds);
    PlayTrackID(TRK_Courtroom);


    // Scene setup
    CourtroomScenarioA(SContext, DiagContext, &Plot, &CurrentCharacter, &ReturnToDefault);

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
                    CourtroomScenarioA(SContext, DiagContext, &Plot, &CurrentCharacter, &ReturnToDefault);
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