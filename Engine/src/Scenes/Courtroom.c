#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"
#include "Buttons.h"
#include "CourtRecord.h"
#include "UI.h"

#include "CHAR_Index.h"

/* FIXME: We need to sort out the final form of this prototype */
int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context, Characters** CharacterIndex, char* DialogPath){

    /* Declaration */
    char EventSelect;

    enum {
        MainEvents,
        ButtonEvents,
        CourtRecordEvents
    };

    DialogueContext* DiagContext;
    CharacterLayer* CharaLayer;

    /* Button related variables */
    BackgroundContext* ButtonLayer;
    ButtonsContext* BContext;
    SDL_Rect ButtonsRect;
    unsigned char ButtonInput;

    /* CourtRecord related variables */
    bool CourtRecordActivated;

    SceneContext* SContext;

    Items* ItemBank;

    /* Initialisation */
    if (Context == NULL){
        fprintf(stderr, "Courtroom Context non initialised");
        return -1;
    }
    DiagContext = InitDialog(DDevice, Context->MainFont, Context->NameFont);

    EventSelect = MainEvents;
    IDevice->EventEnabled = true;

    ButtonsRect.x = 0;
    ButtonsRect.y = 0;
    ButtonsRect.w = DDevice->ScreenResolution.x;
    ButtonsRect.h = DDevice->ScreenResolution.y - (DiagContext->TextBounds.h + DiagContext->NameBounds.h);

    /* Button Init */
    ButtonLayer = InitBackground(DDevice, "Assets/Anim/ButtonHorizontal.xml");
    BContext = InitButtons(DDevice, ButtonLayer, Context->ButtonFont, 224, &ButtonsRect);
    MoveButtonsToCoordinates(BContext, DDevice->ScreenResolution.x, 0);
    Context->ButtonActivated = 0;

    /* Load the ItemBank in memory */
    ItemBank = LoadItemsFromFile(DDevice, ROOT""TEXTURES"Evidences"SL"Evidences.xml");
    /* CourtRecord Init */
    InitCourtRecord(DDevice, ItemBank);
    /* Init the notification handler */
    InitUI(DDevice, ItemBank);
    CourtRecordActivated = false;

    InitCharacter(DDevice, &CHAR_PhoenixWright); /* Initialise the character in memory */
    InitCharacter(DDevice, &CHAR_MiaFey);
    InitCharacter(DDevice, &CHAR_WinstonPayne);
    InitCharacter(DDevice, &CHAR_Judge);
    InitCharacter(DDevice, &CHAR_FrankShawit);

    InitCharacter(DDevice, &CHAR_Desk);
    InitCharacter(DDevice, &CHAR_DefendantDesk);

    /* InitScene */
    SContext = InitScene(DDevice, IDevice, DiagContext, BContext, CharacterIndex, Context, DialogPath);

    CharaLayer = NULL;
    InitCharacterLayer(&CharaLayer, SContext->BGContext);
    AddCharacterToLayer(CharaLayer, &CHAR_PhoenixWright, SContext->BGContext, 0, 0, DDevice);
    AddCharacterToLayer(CharaLayer, &CHAR_Desk, SContext->BGContext, 0, 0, DDevice);

    AddCharacterToLayer(CharaLayer, &CHAR_MiaFey, SContext->BGContext, 5, 0, DDevice);

    AddCharacterToLayer(CharaLayer, &CHAR_WinstonPayne, SContext->BGContext, 1, 0, DDevice);
    AddCharacterToLayer(CharaLayer, &CHAR_Desk, SContext->BGContext, 1, 1, DDevice);

    AddCharacterToLayer(CharaLayer, &CHAR_Judge, SContext->BGContext, 4, 0, DDevice);

    AddCharacterToLayer(CharaLayer, &CHAR_FrankShawit, SContext->BGContext, 2, 0, DDevice);
    AddCharacterToLayer(CharaLayer, &CHAR_DefendantDesk, SContext->BGContext, 2, 0, DDevice);
    
    /* Main Loop */
    parseScene(SContext);
    while (true){
        /* Events Loop */
        while(SDL_PollEvent(&IDevice->event)){
            /* Global events */
            switch (IDevice->event.type)
            {
            case SDL_QUIT:
                goto Exit;
                break;
            }

            if (IDevice->EventEnabled){

                switch (EventSelect)
                {
                case MainEvents: /* Regular events */
                    switch (IDevice->event.type)
                    {
                    case SDL_KEYDOWN:
                        switch (IDevice->event.PADKEY)
                        {
                        case PAD_SELECT:
                            CharacterPlayAnimation(CharacterIndex[Context->CurrentCharacter], Context->IdleAnimation); /* Mouaif */
                            parseScene(SContext);
                            break;
                        case PAD_COURTRECORD:
                            CourtRecordActivated = true;
                            EventSelect = CourtRecordEvents;
                            break;
                        default:
                            break;
                        }
                        break;
                    }
                    break;
                case ButtonEvents:
                    HandleButtonsEvents(BContext, IDevice);
                    switch (IDevice->event.type)
                    {
                    case SDL_KEYDOWN:
                        switch (IDevice->event.PADKEY)
                        {
                        case PAD_SELECT:
                            ButtonInput = GetClkdButtonID(BContext); /* Get the id of the currently selected button */
                            Context->ButtonActivated = 0;
                            EventSelect = MainEvents;
                            SContext->entry = searchSceneNode(SContext->entry, Context->ButtonJumpLabels[ButtonInput]);
                            MoveBackground(ButtonLayer, 0, 0);
                            CharacterPlayAnimation(CharacterIndex[Context->CurrentCharacter], Context->IdleAnimation); /* Mouaif */
                            parseScene(SContext);
                            break;
                        case PAD_COURTRECORD:
                            CourtRecordActivated = true;
                            EventSelect = CourtRecordEvents;
                            break;
                        default:
                            break;
                        }
                        break;
                    }
                    break;

                case CourtRecordEvents:
                    HandleCourtRecordEvents(&IDevice->event);
                    switch (IDevice->event.type)
                    {
                    case SDL_KEYDOWN:
                        switch (IDevice->event.PADKEY)
                        {
                        case PAD_COURTRECORD:
                            CourtRecordActivated = false;
                            if (!Context->ButtonActivated){
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
        if ((DiagContext->progress >= Context->ReturnToDefault) && (Context->ReturnToDefault != -1)){
            CharacterPlayAnimation(CharacterIndex[Context->CurrentCharacter], Context->IdleAnimation);
            Context->ReturnToDefault = -1;
            if (Context->ButtonActivated){ /* We do that here because we want to wait for the dialogue to end before showing the buttons */
                BackgroundPlayAnimation(ButtonLayer, 0, &IDevice->EventEnabled);
                EventSelect = ButtonEvents;
            }
        }

        /* Rendering (Back to front) */
        DisplayBackground(DDevice, SContext->BGContext);    /* Background */
        DisplayCharacterLayer(DDevice, CharaLayer);         /* Character Layer */
        DisplayBackground(DDevice, SContext->ScenePics);    /* Exposition Pictures */
        Dialogue(DiagContext);                              /* Dialog */
        if (Context->ButtonActivated){                      /* Check if the buttons are actually on screen to save ressources */
            DisplayBackground(DDevice, ButtonLayer);        /* Animate the button layer */
            DrawButtons(BContext);                          /* Draw the actual buttons (Maybe merge ?) */
        }
        DrawUI(DDevice, IDevice);                           /* Draw teh user interface */
        if (CourtRecordActivated){
            DrawCourtRecord(DDevice, Context->MainFont);    /* Draw the court Record */
        }
        #ifdef _SDL
            SDL_Flip(DDevice->Screen);
        #else
            SDL_RenderPresent(DDevice->Renderer);           /* Update the main window */
        #endif
    }


Exit:
    /* Cleaning memory */

    /*free(DiagContext); */
    /*if (sceneFile)
        xmlFreeDoc(sceneFile);*/

    /*if (BGContext)
        FreeBackground(BGContext);*/

    if (ButtonLayer)
        FreeBackground(ButtonLayer);

    return 0;
}