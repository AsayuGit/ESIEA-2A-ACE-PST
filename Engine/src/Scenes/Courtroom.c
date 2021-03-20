#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"
#include "Buttons.h"

#include "CHAR_Index.h"

int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context, Characters** CharacterIndex, int NbOfCharacters){

    // Declaration
    SDL_Event event;

    DialogueContext* DiagContext;
    SceneContext* SContext;
    CharacterLayer* CharaLayer;

    SceneContext* SEmpty;
    ButtonsContext* BContext;

    SDL_Rect ButtonsRect;

    int CurrentCharacter;
    int IdleAnimation;
    bool SceneFlip = false;
    int Plot = 0;
    int ReturnToDefault;
    
    char ButtonActivated;
    char ButtonInput;
    char* ButtonJumpLabels[4];
    char BGAnimComplete;

    xmlDoc* sceneFile;
    xmlNode* scenePointer;

    // Initialisation
    if (Context == NULL){
        fprintf(stderr, "Courtroom Context non initialised");
        return -1;
    }
    DiagContext = InitDialog(DDevice, Context->MainFont, Context->NameFont);
    SContext = InitScene(DDevice, S_Courtroom);

    ButtonsRect.x = 0;
    ButtonsRect.y = 0;
    ButtonsRect.w = DDevice->ScreenResolution.x;
    ButtonsRect.h = DDevice->ScreenResolution.y - (DiagContext->TextBounds.h + DiagContext->NameBounds.h);

    SEmpty = InitScene(DDevice, S_Empty);
    BContext = InitButtons(DDevice, SEmpty, Context->ButtonFont, &ButtonsRect);
    MoveButtonsToCoordinates(BContext, DDevice->ScreenResolution.x, 0);

    InitCharacter(DDevice, &CHAR_PhoenixWright); // Initialise the character in memory
    InitCharacter(DDevice, &CHAR_MiaFey);
    InitCharacter(DDevice, &CHAR_WinstonPayne);
    InitCharacter(DDevice, &CHAR_Judge);

    InitCharacter(DDevice, &CHAR_Desk);

    CharaLayer = NULL;
    InitCharacterLayer(&CharaLayer, SContext);
    AddCharacterToLayer(CharaLayer, &CHAR_PhoenixWright, SContext, 0, 0, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, &CHAR_Desk, SContext, 0, 0, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, &CHAR_MiaFey, SContext, 5, 0, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, &CHAR_WinstonPayne, SContext, 1, 0, DDevice, SContext->SurfaceBounds);
    AddCharacterToLayer(CharaLayer, &CHAR_Desk, SContext, 1, 1, DDevice, SContext->SurfaceBounds);

    AddCharacterToLayer(CharaLayer, &CHAR_Judge, SContext, 4, 0, DDevice, SContext->SurfaceBounds);
    PlayTrackID(TRK_Courtroom);

    ButtonActivated = 0;
    BGAnimComplete = 1;

    // Scene setup
    sceneFile = loadScene("Assets/Dialogue/court.xml");
    scenePointer = xmlDocGetRootElement(sceneFile)->children;
    parseScene(&scenePointer, DiagContext, SContext, BContext, CharacterIndex, NbOfCharacters, &IdleAnimation, &ReturnToDefault, &CurrentCharacter, &BGAnimComplete, &ButtonActivated, ButtonJumpLabels);
    //CourtroomScenarioA(SContext, SEmpty, DiagContext, BContext, &Plot, &CurrentCharacter, &IdleAnimation, &ReturnToDefault, &BGAnimComplete, &ButtonActivated, &ButtonInput);
    // Main Loop
    while (1){
        // Events Loop
        while(SDL_PollEvent(&event)){
            if (BGAnimComplete && ButtonActivated)
                HandleButtonsEvents(BContext, &event);
            switch (event.type)
            {
            case SDL_QUIT:
                goto Exit;
                break;

            case SDL_KEYDOWN:
                if (BGAnimComplete){
                    switch (PADKEY)
                    {
                    case PAD_A:
                        if (ButtonActivated) {
                            ButtonInput = GetClkdButtonID(BContext);
                            ButtonActivated = 0;
                            scenePointer = searchSceneNode(&scenePointer, ButtonJumpLabels[ButtonInput]);
                            //printf("Reeee : %d | %d\n", SEmpty->ScenesCoordinates, SEmpty->ScenesCoordinates);
                            MoveTile(SEmpty, 0, 0);
                        }
                        CharacterPlayAnimation(CharacterIndex[CurrentCharacter], IdleAnimation); // Mouaif
                        //CourtroomScenarioA(SContext, SEmpty, DiagContext, BContext, &Plot, &CurrentCharacter, &IdleAnimation, &ReturnToDefault, &BGAnimComplete, &ButtonActivated, &ButtonInput);
                        parseScene(&scenePointer, DiagContext, SContext, BContext, CharacterIndex, NbOfCharacters, &IdleAnimation, &ReturnToDefault, &CurrentCharacter, &BGAnimComplete, &ButtonActivated, ButtonJumpLabels);
                        break;
                    
                    default:
                        break;
                    }
                }

                break;
            default:
                break;
            }
        }

        // Logic
        if ((DiagContext->progress >= ReturnToDefault) && (ReturnToDefault != -1)){
            CharacterPlayAnimation(CharacterIndex[CurrentCharacter], IdleAnimation);
            ReturnToDefault = -1;
            if (ButtonActivated)
                BackgroundPlayAnimation(SEmpty, 0, &BGAnimComplete);
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

            DisplayBackground(DDevice, SEmpty);
            DrawButtons(BContext);
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
    if (sceneFile)
        xmlFreeDoc(sceneFile); // Free memory

    return 0;
}