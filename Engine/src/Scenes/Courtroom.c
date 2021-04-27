#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"
#include "Buttons.h"
#include "CourtRecord.h"
#include "UI.h"

/* FIXME: Temporary */
Characters* CharactersIndex[CharactersCount];

/* FIXME: We need to sort out the final form of this prototype */
int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context, char* DialogPath){

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

    CharactersIndex[0] = InitCharacter(DDevice, "Assets/Characters/PhoenixWright.xml"); /* Initialise the character in memory */
    CharactersIndex[1] = InitCharacter(DDevice, "Assets/Characters/MiaFey.xml");
    CharactersIndex[2] = NULL; /* Edgeworth */
    CharactersIndex[3] = InitCharacter(DDevice, "Assets/Characters/Judge.xml");
    CharactersIndex[4] = InitCharacter(DDevice, "Assets/Characters/WinstonPayne.xml");
    CharactersIndex[5] = InitCharacter(DDevice, "Assets/Characters/Desk.xml");
    CharactersIndex[6] = InitCharacter(DDevice, "Assets/Characters/FrankShawit.xml");
    CharactersIndex[7] = InitCharacter(DDevice, "Assets/Characters/DefendantDesk.xml");

    /* InitScene */
    SContext = InitScene(DDevice, IDevice, DiagContext, BContext, CharactersIndex, Context, DialogPath);

    CharaLayer = NULL;
    InitCharacterLayer(&CharaLayer, SContext->BGContext);
    AddCharacterToLayer(CharaLayer, CharactersIndex[0], SContext->BGContext, 0, 0, DDevice);
    AddCharacterToLayer(CharaLayer, CharactersIndex[5], SContext->BGContext, 0, 0, DDevice);

    AddCharacterToLayer(CharaLayer, CharactersIndex[1], SContext->BGContext, 5, 0, DDevice);

    AddCharacterToLayer(CharaLayer, CharactersIndex[4], SContext->BGContext, 1, 0, DDevice);
    AddCharacterToLayer(CharaLayer, CharactersIndex[5], SContext->BGContext, 1, 1, DDevice);

    AddCharacterToLayer(CharaLayer, CharactersIndex[3], SContext->BGContext, 4, 0, DDevice);

    AddCharacterToLayer(CharaLayer, CharactersIndex[6], SContext->BGContext, 2, 0, DDevice);
    AddCharacterToLayer(CharaLayer, CharactersIndex[7], SContext->BGContext, 2, 0, DDevice);
    
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
                            CharacterPlayAnimation(CharactersIndex[Context->CurrentCharacter], Context->IdleAnimation); /* Mouaif */
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
                            CharacterPlayAnimation(CharactersIndex[Context->CurrentCharacter], Context->IdleAnimation); /* Mouaif */
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

        /* Logic */ /* FIXME: There may be a way to optimize this further */ 
        if ((DiagContext->progress >= Context->ReturnToDefault) && (Context->ReturnToDefault != -1)){
            CharacterPlayAnimation(CharactersIndex[Context->CurrentCharacter], Context->IdleAnimation);
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