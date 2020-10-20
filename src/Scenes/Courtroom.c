#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"

int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context){

    // Declaration
    SDL_Event event;

    DialogueContext* DiagContext;

    Surface* Courtroom = NULL;
    SDL_Rect ScreenTile = {0, 0, DDevice->ScreenResolution.x, DDevice->ScreenResolution.y};
    
    Surface* Desk = NULL; 
    SDL_Rect DeskRect;

    Vector2i ScreenCoordinates = {0, 0};
    int CurrentCharacter;
    Uint32 CommonColorKey = 0xFF00FF;
    bool SceneFlip = false;
    Uint32 Plot = 0;
    int ReturnToDefault;

    // Initialisation
    if (Context == NULL){
        fprintf(stderr, "Courtroom Context non initialised");
    }
    DiagContext = InitDialog(DDevice, Context->Font);
    SetDialogueText(DiagContext, "Mmmmm");
    
    Courtroom = LoadSurface(ROOT""TEXTURES"Places"SL"Courtroom"TEX_EXT, DDevice, NULL, false);
    MoveTile(&ScreenTile, &ScreenCoordinates); // Default Screen postion
    
    Desk = LoadSurface(ROOT""TEXTURES"Places"SL"CourtroomDesk"TEX_EXT, DDevice, &CommonColorKey, false);
    #ifdef _SDL
        DeskRect.w = Desk->w; DeskRect.h = Desk->h;
    #else
        SDL_QueryTexture(Desk, NULL, NULL, &DeskRect.w, &DeskRect.h);
    #endif
    DeskRect.x = 0; DeskRect.y = DDevice->ScreenResolution.y - DeskRect.h;

    CurrentCharacter = Phoenix_Wright;
    InitCharacter(DDevice, Phoenix_Wright); // Initialise the character in memory
    InitCharacter(DDevice, Mia_Fey);

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
                        ScreenCoordinates.y = 1;
                        ScreenCoordinates.x = 2;
                        CurrentCharacter = Mia_Fey;
                        MoveTile(&ScreenTile, &ScreenCoordinates);
                        CharacterPlayAnimation(Mia_Fey, 1);
                        ReturnToDefault = SetDialogueText(DiagContext, "Do you have something in mind phoenix ?");
                        //SceneFlip = true;
                        //DeskRect.x = DDevice->ScreenResolution.x - DeskRect.w; // For Edgeworth
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

        // Keyboard Inputs
        if (IDevice->KeyStates[PAD_A]){
        }

        // Logic
        if ((DiagContext->progress == ReturnToDefault) && (ReturnToDefault != -1)){
            CharacterPlayAnimation(CurrentCharacter, 0);
            ReturnToDefault = -1;
        }
        // Rendering
        #ifdef _SDL
            SDL_BlitSurface(Courtroom, &ScreenTile, DDevice->Screen, NULL); // Background
            
            DisplayCharacter(DDevice, CurrentCharacter); // Curent Character on screen
            
            FlipBlitSurface(Desk, NULL, DDevice->Screen, &DeskRect, SceneFlip); // Desk
            Dialogue(IDevice, DiagContext); // Dialog
            SDL_Flip(DDevice->Screen);
        #else
            //SDL_RenderClear(DDevice->Renderer); Unnecessary as the background fills the screen
            SDL_RenderCopy(DDevice->Renderer, Courtroom, &ScreenTile, NULL); // Background
            
            DisplayCharacter(DDevice, CurrentCharacter); // Curent Character on screen
            
            SDL_RenderCopyEx(DDevice->Renderer, Desk, NULL, &DeskRect, 0, 0, SceneFlip); // Desk
            Dialogue(IDevice, DiagContext); // Dialog
            SDL_RenderPresent(DDevice->Renderer);
        #endif
    }


Exit:
    // Cleaning memory
    #ifdef _SDL
    SDL_FreeSurface(Courtroom);
    #else
    SDL_DestroyTexture(Courtroom);
    #endif

    //free(DiagContext);

    return 0;
}