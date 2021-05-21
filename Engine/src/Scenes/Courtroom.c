#include "Courtroom.h"
#include "SceneCommon.h"
#include "Characters.h"
#include "Buttons.h"
#include "CourtRecord.h"
#include "UI.h"

void FreeCourtroom(CourtroomContext* CourtContext){
    free(CourtContext->MainFont);
    free(CourtContext->NameFont);
    free(CourtContext->ButtonFont);
    free(CourtContext);
}

CourtroomContext* InitCourtroom(DisplayDevice* DDevice){
    CourtroomContext* CourtContext;

    CourtContext = (CourtroomContext*)calloc(1, sizeof(CourtroomContext));

    CourtContext->MainFont = LoadBitmapFont(ROOT""FONTS"AceAttorneyFont"TEX_EXT, DDevice, 0xff00ff);
    CourtContext->NameFont = LoadBitmapFont(ROOT""FONTS"NameFont"TEX_EXT, DDevice, 0xff00ff);
    CourtContext->ButtonFont = LoadBitmapFont(ROOT""FONTS"MainMenuFont"TEX_EXT, DDevice, 0xff00ff);
    if (!CourtContext->MainFont || !CourtContext->NameFont){
        printf("ERROR: Couldn't load in fonts or not enough memory!\n");
        goto ERROR;
    }

    return CourtContext;

ERROR:

    if (CourtContext->MainFont)
        free(CourtContext->MainFont);
    if (CourtContext->NameFont)
        free(CourtContext->NameFont);
    if (CourtContext->ButtonFont)
        free(CourtContext->ButtonFont);
    free(CourtContext);

    return NULL;
}

/* FIXME: We need to sort out the final form of this prototype */
int Scene_Courtroom(DisplayDevice* DDevice, InputDevice* IDevice, CourtroomContext* Context, char* DialogPath){

    /* Declaration */

    enum {
        MainEvents,
        ButtonEvents,
        CREvents
    };

    DialogueContext* DiagContext;

    /* Button related variables */
    BackgroundContext* ButtonLayer;
    ButtonsContext* BContext;
    SDL_Rect ButtonsRect;
    unsigned char ButtonInput;

    SceneContext* SContext;

    Items* ItemBank;

    /* Initialisation */
    if (Context == NULL){
        fprintf(stderr, "Courtroom Context non initialised");
        return -1;
    }
    DiagContext = InitDialog(DDevice, Context->MainFont, Context->NameFont);

    Context->EventSelect = MainEvents;
    IDevice->EventEnabled = true;

    ButtonsRect.x = 0;
    ButtonsRect.y = 0;
    ButtonsRect.w = BASE_RESOLUTION_X;
    ButtonsRect.h = BASE_RESOLUTION_Y - (DiagContext->TextBounds.h + DiagContext->NameBounds.h);

    /* Button Init */
	ButtonLayer = InitBackground(DDevice, ROOT"Assets"SL"Anim"SL"ButtonHorizontal.xml");
    BContext = InitButtons(DDevice, ButtonLayer, Context->ButtonFont, 224, &ButtonsRect);
    MoveButtonsToCoordinates(BContext, BASE_RESOLUTION_X, 0);
    Context->ButtonActivated = 0;

    /* Load the ItemBank in memory */
    ItemBank = LoadItemsFromFile(DDevice, ROOT""TEXTURES"Evidences"SL"Evidences.xml");
    /* CourtRecord Init */
    InitCourtRecord(DDevice, ItemBank);

    /* InitScene */
    SContext = InitScene(DDevice, IDevice, DiagContext, BContext, Context, DialogPath);

    /* Init the notification handler */
    InitUI(DDevice, ItemBank, SContext);
    
    /* Main Loop */
    parseScene(DDevice, SContext);
    while (true){
        /* Events Loop */
        while(SDL_PollEvent(&IDevice->event)){
            SystemEvents(DDevice, IDevice);

            /* Global events */
            switch (IDevice->event.type)
            {
            case SDL_QUIT:
                goto Exit;
                break;
            }

            if (IDevice->EventEnabled){

                switch (Context->EventSelect)
                {
                case MainEvents: /* Regular events */
                    switch (IDevice->event.type)
                    {
                    case PAD_KEYDOWN:
                        if (!IDevice->event.key.repeat){
                            switch (IDevice->event.PADKEY)
                            {
                            case PAD_SELECT:
                                if (SContext->diagMode > 0){
                                    if ((Context->IdleAnimation > -1) && (Context->CurrentCharacter >= 0))
                                        CharacterPlayAnimation(SContext->CharactersIndex[Context->CurrentCharacter], Context->IdleAnimation); /* Mouaif */
                                    SceneForward(SContext);
                                    parseScene(DDevice, SContext);
                                }
                                break;

                            case PAD_BACK:
                                /* FIXME: We need a nice way to toogle dialogue's booth way mode */
                                if (SContext->diagMode == 2){
                                    if ((Context->IdleAnimation > -1) && (Context->CurrentCharacter >= 0))
                                        CharacterPlayAnimation(SContext->CharactersIndex[Context->CurrentCharacter], Context->IdleAnimation); /* Mouaif */
                                    SceneBackward(SContext);
                                    parseScene(DDevice, SContext);
                                }
                            
                                break;

                            case PAD_COURTRECORD:
                                ShowCourtRecord(IDevice);
                                Context->EventSelect = CREvents;
                                break;

                            case PAD_PRESS:
                                if (SContext->diagMode > 0){
                                    if (SContext->press){
                                        SContext->entry = SContext->press;
                                        SContext->diagMode = 1;
                                        setUI(HOLD_IT, 0);
                                    }
                                }
                                break;
                            default:
                                break;
                            }
                        }
                        break;
                    }
                    break;
                case ButtonEvents:
                    HandleButtonsEvents(BContext, IDevice);
                    switch (IDevice->event.type)
                    {
                    case PAD_KEYDOWN:
                        if (!IDevice->event.key.repeat){
                            switch (IDevice->event.PADKEY)
                            {
                            case PAD_SELECT:
                                ButtonInput = GetClkdButtonID(BContext); /* Get the id of the currently selected button */
                                Context->ButtonActivated = 0;
                                Context->EventSelect = MainEvents;
                                SContext->entry = searchNodeLabel(SContext->entry, Context->ButtonJumpLabels[ButtonInput]);
                                MoveBackground(ButtonLayer, 0, 0);
                                if ((Context->IdleAnimation > -1) && (Context->CurrentCharacter >= 0))
                                    CharacterPlayAnimation(SContext->CharactersIndex[Context->CurrentCharacter], Context->IdleAnimation); /* Mouaif */
                                parseScene(DDevice, SContext);
                                break;
                            case PAD_COURTRECORD:
                                ShowCourtRecord(IDevice);
                                Context->EventSelect = CREvents;
                                break;
                            default:
                                break;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                    break;
                case CREvents:
                    HandleCourtRecordEvents(&IDevice->event, SContext);
                    break;

                default:
                    break;
                }
            }
        }

        /* Logic */ /* FIXME: There may be a way to optimize this further */ 
        if ((DiagContext->progress >= Context->ReturnToDefault) && (Context->ReturnToDefault != -1)){
            if ((Context->IdleAnimation >= 0) && (Context->CurrentCharacter >= 0))
                CharacterPlayAnimation(SContext->CharactersIndex[Context->CurrentCharacter], Context->IdleAnimation);
            Context->ReturnToDefault = -1;
            if (Context->ButtonActivated){ /* We do that here because we want to wait for the dialogue to end before showing the buttons */
                BackgroundPlayAnimation(ButtonLayer, 0, &IDevice->EventEnabled, true);
                Context->EventSelect = ButtonEvents;
            }
            if (SContext->activateCourtRecord){
                ShowCourtRecord(IDevice);
                Context->EventSelect = CREvents;
                SContext->activateCourtRecord = false;
            }
        }

        /* Sound */
        MusicDaemon();

        /* Rendering (Back to front) */
        DisplayBackground(DDevice, SContext->BGContext);        /* Background */
		DisplayCharacterLayer(DDevice, SContext->CharaLayer);   /* Character Layer */
        DisplayBackground(DDevice, SContext->ScenePics);        /* Exposition Pictures */
        if (SContext->DiagShown)
            Dialogue(DiagContext, SContext->diagMode);         /* Dialog */
        if (Context->ButtonActivated){                          /* Check if the buttons are actually on screen to save ressources */
            DisplayBackground(DDevice, ButtonLayer);            /* Animate the button layer */
            DrawButtons(BContext);                              /* Draw the actual buttons (Maybe merge ?) */
        }
        DrawUI(DDevice, IDevice);                               /* Draw teh user interface */
        DrawCourtRecord(DDevice, Context->MainFont);        /* Draw the court Record */
        FinishFrame(DDevice);                                   /* Update the main window */
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