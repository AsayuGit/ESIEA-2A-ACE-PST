#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"

int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context){

    // Declaration
    SDL_Event event;

    DialogueContext* DiagContext;
    SceneContext* SContext;
    CharacterLayer* CharaLayer;
    
    Surface* Desk = NULL; 
    SDL_Rect DeskRect;

    int CurrentCharacter;
    Uint32 CommonColorKey = 0xFF00FF;
    bool SceneFlip = false;
    Uint32 Plot = 0;
    int ReturnToDefault;

    // Initialisation
    if (Context == NULL){
        fprintf(stderr, "Courtroom Context non initialised");
        return -1;
    }
    DiagContext = InitDialog(DDevice, Context->Font);
    SetDialogueText(DiagContext, "Mmmmm");

    SContext = InitScene(DDevice, S_Courtroom);
    
    Desk = LoadSurface(ROOT""TEXTURES"Places"SL"CourtroomDesk"TEX_EXT, DDevice, &CommonColorKey, false);
    #ifdef _SDL
        DeskRect.w = Desk->w; DeskRect.h = Desk->h;
    #else
        SDL_QueryTexture(Desk, NULL, NULL, &DeskRect.w, &DeskRect.h);
    #endif
    DeskRect.x = 0; DeskRect.y = DDevice->ScreenResolution.y - DeskRect.h;

    InitCharacter(DDevice, Phoenix_Wright); // Initialise the character in memory
    InitCharacter(DDevice, Mia_Fey);
    CurrentCharacter = Phoenix_Wright;

    CharaLayer = NULL;
    InitCharacterLayer(&CharaLayer, SContext);
    AddCharacterToLayer(CharaLayer, Phoenix_Wright, 0, 0, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, Mia_Fey, 2, 1, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, Mia_Fey, 4, 0, DDevice, SContext->SurfaceBounds); // To be Egeworth

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
                    switch (Plot)
                    {
                    case 0:
                        CharacterPlayAnimation(Phoenix_Wright, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, "Objection !\nThe witness could not have known the statue was a clock !");
                        Plot++;
                        break;
                    
                    case 1:
                        CurrentCharacter = Mia_Fey;
                        MoveTile(SContext, 2, 1, 0);
                        CharacterPlayAnimation(Mia_Fey, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, "Do you have something in mind phoenix ?");
                        //SceneFlip = true;
                        //DeskRect.x = DDevice->ScreenResolution.x - DeskRect.w; // For Edgeworth
                        DeskRect.x = DDevice->ScreenResolution.x;
                        Plot++;
                        break;
                    case 2:
                        CurrentCharacter = Phoenix_Wright;
                        MoveTile(SContext, 0, 0, 0);
                        CharacterPlayAnimation(Phoenix_Wright, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, "Your Honor !\nLook at the witness face !");
                        DeskRect.x = 0;
                        Plot++;
                        break;
                    
                    case 3:
                        BackgroundPlayAnimation(SContext, 1);
                        Plot++;
                        break;

                    case 4:
                        CurrentCharacter = Mia_Fey;
                        MoveTile(SContext, 2, 1, 0);
                        CharacterPlayAnimation(Mia_Fey, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, "Wow, you just slid across the courtroom !");
                        DeskRect.x = DDevice->ScreenResolution.x;
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
/* // TEMP
        if (SContext->PlayingAnimation != -1){
            DeskRect.x = -SContext->ObjectLayerOffset;
        }
*/
        // Logic
        if ((DiagContext->progress == ReturnToDefault) && (ReturnToDefault != -1)){
            CharacterPlayAnimation(CurrentCharacter, 0);
            ReturnToDefault = -1;
        }
        // Rendering
        #ifdef _SDL
            DisplayBackground(DDevice, SContext); // Background
            //DisplayCharacter(DDevice, CurrentCharacter); // Curent Character on screen
            DisplayCharacterLayer(DDevice, CharaLayer);
            
            FlipBlitSurface(Desk, NULL, DDevice->Screen, &DeskRect, SceneFlip); // Desk
            Dialogue(IDevice, DiagContext); // Dialog
            SDL_Flip(DDevice->Screen);
        #else
            DisplayBackground(DDevice, SContext); // Background
            DisplayCharacterLayer(DDevice, CharaLayer);
            
            SDL_RenderCopyEx(DDevice->Renderer, Desk, NULL, &DeskRect, 0, 0, SceneFlip); // Desk
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