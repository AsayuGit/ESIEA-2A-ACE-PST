#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"
#include "Buttons.h"
#include "CourtReccord.h"

#include "CHAR_Index.h"

int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context, Characters** CharacterIndex, int NbOfCharacters){

    // Declaration
    SDL_Event event;
    char EventSelect;

    enum {
        MainEvents,
        ButtonEvents,
        CourtRecordEvents
    }EventID;

    DialogueContext* DiagContext;
    SceneContext* SContext;
    CharacterLayer* CharaLayer;

    // Button related variables
    SceneContext* ButtonLayer;
    ButtonsContext* BContext;
    SDL_Rect ButtonsRect;
    char ButtonActivated;
    char ButtonInput;
    char* ButtonJumpLabels[4];
    char BGAnimComplete;

    // CourtReccord related variables
    char CourtReccordActivated;

    int CurrentCharacter;
    int IdleAnimation;
    int ReturnToDefault;

    xmlDoc* sceneFile;
    xmlNode* scenePointer;

    Items* ItemBank;

    // Initialisation
    if (Context == NULL){
        fprintf(stderr, "Courtroom Context non initialised");
        return -1;
    }
    DiagContext = InitDialog(DDevice, Context->MainFont, Context->NameFont);
    SContext = InitScene(DDevice, S_Courtroom);

    EventSelect = MainEvents;

    ButtonsRect.x = 0;
    ButtonsRect.y = 0;
    ButtonsRect.w = DDevice->ScreenResolution.x;
    ButtonsRect.h = DDevice->ScreenResolution.y - (DiagContext->TextBounds.h + DiagContext->NameBounds.h);

    // Button Init
    ButtonLayer = InitScene(DDevice, S_Empty);
    BContext = InitButtons(DDevice, ButtonLayer, Context->ButtonFont, &ButtonsRect);
    MoveButtonsToCoordinates(BContext, DDevice->ScreenResolution.x, 0);
    ButtonActivated = 0;

    // CourtReccord Init
    ItemBank = LoadItemsFromFile(DDevice, ROOT""TEXTURES"Evidences"SL"Evidences.xml");
    InitCourtReccord(DDevice, ItemBank);
    CourtReccordActivated = 0;
    AddItemToCourtReccord(0);
    AddItemToCourtReccord(5);
    //RemoveItemFromCourtReccord(0);
    //RemoveItemFromCourtReccord(5);
    //EmptyCourtReccord();

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

    BGAnimComplete = 1;

    // Scene setup
    sceneFile = loadScene("Assets/Dialogue/court.xml");
    scenePointer = xmlDocGetRootElement(sceneFile)->children;
    parseScene(&scenePointer, DiagContext, SContext, BContext, CharacterIndex, NbOfCharacters, &IdleAnimation, &ReturnToDefault, &CurrentCharacter, &BGAnimComplete, &ButtonActivated, ButtonJumpLabels);
    //CourtroomScenarioA(SContext, ButtonLayer, DiagContext, BContext, &Plot, &CurrentCharacter, &IdleAnimation, &ReturnToDefault, &BGAnimComplete, &ButtonActivated, &ButtonInput);
    // Main Loop
    while (1){
        // Events Loop
        while(SDL_PollEvent(&event)){
            // Global events
            switch (event.type)
            {
            case SDL_QUIT:
                goto Exit;
                break;
            }

            // Animation tied events
            if (BGAnimComplete){
                switch (EventSelect)
                {
                case MainEvents: // Regular events
                    switch (event.type)
                    {
                    case SDL_KEYDOWN:
                        switch (PADKEY)
                        {
                        case PAD_SELECT:
                            CharacterPlayAnimation(CharacterIndex[CurrentCharacter], IdleAnimation); // Mouaif
                            parseScene(&scenePointer, DiagContext, SContext, BContext, CharacterIndex, NbOfCharacters, &IdleAnimation, &ReturnToDefault, &CurrentCharacter, &BGAnimComplete, &ButtonActivated, ButtonJumpLabels);
                            break;
                        case PAD_COURTRECCORD:
                            CourtReccordActivated = 1;
                            EventSelect = CourtRecordEvents;
                            break;
                        }
                        break;
                    }
                    break;
                case ButtonEvents:
                    HandleButtonsEvents(BContext, &event);
                    switch (event.type)
                    {
                    case SDL_KEYDOWN:
                        switch (PADKEY)
                        {
                        case PAD_SELECT:
                            ButtonInput = GetClkdButtonID(BContext); // Get the id of the currently selected button
                            ButtonActivated = 0;
                            EventSelect = MainEvents;
                            scenePointer = searchSceneNode(&scenePointer, ButtonJumpLabels[ButtonInput]);
                            MoveTile(ButtonLayer, 0, 0);
                            CharacterPlayAnimation(CharacterIndex[CurrentCharacter], IdleAnimation); // Mouaif
                            parseScene(&scenePointer, DiagContext, SContext, BContext, CharacterIndex, NbOfCharacters, &IdleAnimation, &ReturnToDefault, &CurrentCharacter, &BGAnimComplete, &ButtonActivated, ButtonJumpLabels);
                            break;
                        case PAD_COURTRECCORD:
                            CourtReccordActivated = 1;
                            EventSelect = CourtRecordEvents;
                            break;
                        }
                        break;
                    }
                    break;

                case CourtRecordEvents:
                    HandleCourtReccordEvents(&event);
                    switch (event.type)
                    {
                    case SDL_KEYDOWN:
                        switch (PADKEY)
                        {
                        case PAD_COURTRECCORD:
                            CourtReccordActivated = 0;
                            if (!ButtonActivated){
                                EventSelect = MainEvents;
                            } else {
                                EventSelect = ButtonEvents;
                            }
                            break;
                        }
                        break;
                    }
                    break;
                }
            }
        }

        // Logic
        if ((DiagContext->progress >= ReturnToDefault) && (ReturnToDefault != -1)){
            CharacterPlayAnimation(CharacterIndex[CurrentCharacter], IdleAnimation);
            ReturnToDefault = -1;
            if (ButtonActivated){
                BackgroundPlayAnimation(ButtonLayer, 0, &BGAnimComplete);
                EventSelect = ButtonEvents;
            }
        }

        // Rendering
        #ifdef _SDL
            DisplayBackground(DDevice, SContext); // Background
            DisplayCharacterLayer(DDevice, CharaLayer);
            //FlipBlitSurface(Desk, NULL, DDevice->Screen, &DeskRect, SceneFlip); // Desk
            Dialogue(IDevice, DiagContext); // Dialog
            SDL_Flip(DDevice->Screen);
        #else
            DisplayBackground(DDevice, SContext);        // Background
            DisplayCharacterLayer(DDevice, CharaLayer);
            Dialogue(IDevice, DiagContext);              // Dialog
            if (ButtonActivated){                        // Check if the buttons are actually on screen to save ressources
                DisplayBackground(DDevice, ButtonLayer); // Animate the button layer
                DrawButtons(BContext);                   // Draw the actual buttons (Maybe merge ?)
            }
            if (CourtReccordActivated){
                DrawCourtReccord(DDevice, Context->MainFont);               // Draw the court reccord
            }
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