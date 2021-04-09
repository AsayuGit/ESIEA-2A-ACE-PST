#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"
#include "Buttons.h"
#include "CourtRecord.h"
#include "Notifications.h"

#include "CHAR_Index.h"

int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context, Characters** CharacterIndex, int NbOfCharacters){

    /* Declaration */
    SDL_Event event;
    char EventSelect;

    enum {
        MainEvents,
        ButtonEvents,
        CourtRecordEvents
    };

    DialogueContext* DiagContext;
    SceneContext* SContext;
    CharacterLayer* CharaLayer;

    /* Button related variables */
    SceneContext* ButtonLayer;
    ButtonsContext* BContext;
    SDL_Rect ButtonsRect;
    char ButtonActivated;
    unsigned char ButtonInput;
    char* ButtonJumpLabels[4];
    char BGAnimComplete;

    /* CourtRecord related variables */
    char CourtRecordActivated;

    int CurrentCharacter;
    int IdleAnimation;
    int ReturnToDefault;

    xmlDoc* sceneFile;
    xmlNode* scenePointer;

    Items* ItemBank;

    /* Initialisation */
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

    /* Button Init */
    ButtonLayer = InitScene(DDevice, S_Empty);
    BContext = InitButtons(DDevice, ButtonLayer, Context->ButtonFont, 224, &ButtonsRect);
    MoveButtonsToCoordinates(BContext, DDevice->ScreenResolution.x, 0);
    ButtonActivated = 0;

    /* Load the ItemBank in memory */
    ItemBank = LoadItemsFromFile(DDevice, ROOT""TEXTURES"Evidences"SL"Evidences.xml");
    /* CourtRecord Init */
    InitCourtRecord(DDevice, ItemBank);
    /* Init the notification handler */
    InitNotifications(DDevice, ItemBank);
    CourtRecordActivated = 0;

    InitCharacter(DDevice, &CHAR_PhoenixWright); /* Initialise the character in memory */
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

    BGAnimComplete = 1;

    /* Scene setup */
    #ifdef _DEBUG
        sceneFile = loadScene("Assets/Dialogue/debug.xml");
    #else
        sceneFile = loadScene("Assets/Dialogue/court.xml");
    #endif
    scenePointer = xmlDocGetRootElement(sceneFile)->children;
    parseScene(&scenePointer, DiagContext, SContext, BContext, CharacterIndex, NbOfCharacters, &IdleAnimation, &ReturnToDefault, &CurrentCharacter, &BGAnimComplete, &ButtonActivated, ButtonJumpLabels);
    /*CourtroomScenarioA(SContext, ButtonLayer, DiagContext, BContext, &Plot, &CurrentCharacter, &IdleAnimation, &ReturnToDefault, &BGAnimComplete, &ButtonActivated, &ButtonInput); */
    /* Main Loop */
    while (1){
        /* Events Loop */
        while(SDL_PollEvent(&event)){
            /* Global events */
            switch (event.type)
            {
            case SDL_QUIT:
                goto Exit;
                break;
            }

            /* Animation tied events */
            if (BGAnimComplete){
                switch (EventSelect)
                {
                case MainEvents: /* Regular events */
                    switch (event.type)
                    {
                    case SDL_KEYDOWN:
                        switch (PADKEY)
                        {
                        case PAD_SELECT:
                            CharacterPlayAnimation(CharacterIndex[CurrentCharacter], IdleAnimation); /* Mouaif */
                            parseScene(&scenePointer, DiagContext, SContext, BContext, CharacterIndex, NbOfCharacters, &IdleAnimation, &ReturnToDefault, &CurrentCharacter, &BGAnimComplete, &ButtonActivated, ButtonJumpLabels);
                            break;
                        case PAD_COURTRECORD:
                            CourtRecordActivated = 1;
                            EventSelect = CourtRecordEvents;
                            break;
                        default:
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
                            ButtonInput = GetClkdButtonID(BContext); /* Get the id of the currently selected button */
                            ButtonActivated = 0;
                            EventSelect = MainEvents;
                            scenePointer = searchSceneNode(&scenePointer, ButtonJumpLabels[ButtonInput]);
                            MoveTile(ButtonLayer, 0, 0);
                            CharacterPlayAnimation(CharacterIndex[CurrentCharacter], IdleAnimation); /* Mouaif */
                            parseScene(&scenePointer, DiagContext, SContext, BContext, CharacterIndex, NbOfCharacters, &IdleAnimation, &ReturnToDefault, &CurrentCharacter, &BGAnimComplete, &ButtonActivated, ButtonJumpLabels);
                            break;
                        case PAD_COURTRECORD:
                            CourtRecordActivated = 1;
                            EventSelect = CourtRecordEvents;
                            break;
                        default:
                            break;
                        }
                        break;
                    }
                    break;

                case CourtRecordEvents:
                    HandleCourtRecordEvents(&event);
                    switch (event.type)
                    {
                    case SDL_KEYDOWN:
                        switch (PADKEY)
                        {
                        case PAD_COURTRECORD:
                            CourtRecordActivated = 0;
                            if (!ButtonActivated){
                                EventSelect = MainEvents;
                            } else {
                                EventSelect = ButtonEvents;
                            }
                            break;
                        default:
                            break;
                        }
                        break;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        /* Logic */
        if ((DiagContext->progress >= ReturnToDefault) && (ReturnToDefault != -1)){
            CharacterPlayAnimation(CharacterIndex[CurrentCharacter], IdleAnimation);
            ReturnToDefault = -1;
            if (ButtonActivated){ /* We do that here because we want to wait for the dialogue to end before showing the buttons */
                BackgroundPlayAnimation(ButtonLayer, 0, &BGAnimComplete);
                EventSelect = ButtonEvents;
            }
        }

        /* Rendering (Back to front) */
        #ifdef _SDL
            DisplayBackground(DDevice, SContext); /* Background */
            DisplayCharacterLayer(DDevice, CharaLayer);
            /*FlipBlitSurface(Desk, NULL, DDevice->Screen, &DeskRect, SceneFlip);  */ /* Desk */
            Dialogue(IDevice, DiagContext); /* Dialog */
            SDL_Flip(DDevice->Screen);
        #else
            DisplayBackground(DDevice, SContext);        /* Background */
            DisplayCharacterLayer(DDevice, CharaLayer);
            Dialogue(IDevice, DiagContext);              /* Dialog */
            if (ButtonActivated){                        /* Check if the buttons are actually on screen to save ressources */
                DisplayBackground(DDevice, ButtonLayer); /* Animate the button layer */
                DrawButtons(BContext);                   /* Draw the actual buttons (Maybe merge ?) */
            }
            DrawNotifications(DDevice);
            if (CourtRecordActivated){
                DrawCourtRecord(DDevice, Context->MainFont);               /* Draw the court Record */
            }
            SDL_RenderPresent(DDevice->Renderer);
        #endif
    }


Exit:
    /* Cleaning memory */
/*
    #ifdef _SDL
    SDL_FreeSurface(Courtroom);
    #else
    SDL_DestroyTexture(Courtroom);
    #endif
*/
    /*free(DiagContext); */
    if (sceneFile)
        xmlFreeDoc(sceneFile); /* Free memory */

    return 0;
}