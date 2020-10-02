#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"

int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context){
    // Declaration
    SDL_Event event;

    DialogueContext* DiagContext;

    Surface* Courtroom = NULL;
    SDL_Rect ScreenTile = {0, 0, DDevice->ScreenResolution.x, DDevice->ScreenResolution.y};

    Vector2i ScreenCoordinates = {0, 0};
    int CurrentCharacter;

    // Initialisation
    if (Context == NULL){
        fprintf(stderr, "Courtroom Context non initialised");
    }

    DiagContext = (DialogueContext*)malloc(sizeof(DialogueContext));
    DiagContext->DialogBox = NULL;

    Courtroom = LoadSurface(ROOT""TEXTURES"Places"SL"Courtroom"TEX_EXT, DDevice, NULL);
    MoveTile(&ScreenTile, &ScreenCoordinates); // Default Screen postion
    
    CurrentCharacter = Phoenix_Wright;
    InitCharacter(Phoenix_Wright, DDevice); // Initialise the character in memory
    InitCharacter(Mia_Fey, DDevice);
    
    // Main Loop
    while (1){
        // Events Loop
        while(SDL_PollEvent(&event)){
            switch (event.type)
            {
            case SDL_QUIT:
                goto Exit;
                break;
            default:
                break;
            }
        }
        // Keyboard Inputs
        if (IDevice->KeyStates[SDL_SCANCODE_SPACE]){
            ScreenCoordinates.y = 1;
            ScreenCoordinates.x = 2;
            CurrentCharacter = Mia_Fey;
            MoveTile(&ScreenTile, &ScreenCoordinates);
        }


        // Logic



        // Rendering
        SDL_RenderClear(DDevice->Renderer);
        SDL_RenderCopy(DDevice->Renderer, Courtroom, &ScreenTile, NULL); // Background
        SDL_RenderCopy(DDevice->Renderer, Cast[CurrentCharacter].Surface, &Cast[CurrentCharacter].SrcRect, &Cast[CurrentCharacter].DstRect); // Curent Character on screen
        Dialogue(DDevice, IDevice, DiagContext);
        SDL_RenderPresent(DDevice->Renderer);
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