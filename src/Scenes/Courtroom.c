#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"

int Scene_Courtroom(DisplayDevice* DisplayDevice, InputDevice* InputDevice, CourtroomContext* Context){
    // Declaration
    SDL_Event event;

    DialogueContext* DiagContext;

    Surface* Courtroom = NULL;
    SDL_Rect ScreenTile = {0, 0, DisplayDevice->ScreenResolution.x, DisplayDevice->ScreenResolution.y};

    Vector2i ScreenCoordinates = {0, 0};
    int CurrentCharacter;

    // Initialisation
    if (Context == NULL){
        fprintf(stderr, "Courtroom Context non initialised");
    }

    DiagContext = (DialogueContext*)malloc(sizeof(DialogueContext));


    Courtroom = LoadSurface(ROOT""TEXTURES"Places"SL"Courtroom"TEX_EXT, DisplayDevice, NULL);
    MoveTile(&ScreenTile, &ScreenCoordinates); // Default Screen postion
    
    CurrentCharacter = Phoenix_Wright;
    InitCharacter(Phoenix_Wright, DisplayDevice); // Initialise the character in memory
    InitCharacter(Mia_Fey, DisplayDevice);
    
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
        if (InputDevice->KeyStates[SDL_SCANCODE_SPACE]){
            ScreenCoordinates.y = 1;
            ScreenCoordinates.x = 2;
            CurrentCharacter = Mia_Fey;
            MoveTile(&ScreenTile, &ScreenCoordinates);
        }


        // Logic



        // Rendering
        SDL_RenderClear(DisplayDevice->Renderer);
        SDL_RenderCopy(DisplayDevice->Renderer, Courtroom, &ScreenTile, NULL); // Background
        SDL_RenderCopy(DisplayDevice->Renderer, Cast[CurrentCharacter].Surface, &Cast[CurrentCharacter].SrcRect, &Cast[CurrentCharacter].DstRect); // Curent Character on screen
        Dialogue(DisplayDevice, InputDevice, DiagContext);
        SDL_RenderPresent(DisplayDevice->Renderer);
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