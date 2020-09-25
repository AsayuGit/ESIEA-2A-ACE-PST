#include "Courtroom.h"
#include "SceneCommon.h"

int Scene_Courtroom(DisplayDevice* DisplayDevice, InputDevice* InputDevice, CourtroomContext* Context){
    // Declaration
    SDL_Event event;

    Surface* Courtroom = NULL;
    SDL_Rect ScreenTile = {0, 0, DisplayDevice->ScreenResolution.x, DisplayDevice->ScreenResolution.y};
    Vector2i ScreenCoordinates = {0, 1};

    // Initialisation
    if (Context == NULL){
        Context = (CourtroomContext*)malloc(sizeof(CourtroomContext));
        // Default values
    }

    Courtroom = LoadSurface(ROOT""TEXTURES"Places"SL"Courtroom"TEX_EXT, DisplayDevice);

    MoveTile(&ScreenTile, &ScreenCoordinates);
    
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
            ScreenCoordinates.x = 1;
            MoveTile(&ScreenTile, &ScreenCoordinates);
        }


        // Logic



        // Rendering
        SDL_RenderClear(DisplayDevice->Renderer);
        SDL_RenderCopy(DisplayDevice->Renderer, Courtroom, &ScreenTile, NULL);
        SDL_RenderPresent(DisplayDevice->Renderer);
    }
Exit:
    // Cleaning memory
    #ifdef _SDL
    SDL_FreeSurface(Courtroom);
    #else
    SDL_DestroyTexture(Courtroom);
    #endif

    return 0;
}