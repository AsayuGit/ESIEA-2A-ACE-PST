#include "SceneCommon.h"
#include "Characters.h"
#include "CourtRecord.h"
#include "UI.h"
#include "Load.h"

/* Idée : Remplacer BackgroundID avec un "Background*" de cette façon les scènes gêrent elles meme leurs bg */

BGAnimation* ParseBGAnimation(xmlNode* property){
    BGAnimation* LoadingAnimation;
    xmlNode *array, *entry;
    unsigned int ArrayID, EntryID;

    LoadingAnimation = (BGAnimation*)malloc(xmlChildElementCount(property)*sizeof(BGAnimation));
    array = property->children;

    ArrayID = 0;
    while (array){
        if (strcmp((char*)array->name, "anim") == 0) {
            LoadingAnimation[ArrayID].NbOfAnimStates = xmlChildElementCount(array);
            LoadingAnimation[ArrayID].AnimRegion = InitRect(
                atoi((char*)xmlGetProp(array, (xmlChar*)"X")),
                atoi((char*)xmlGetProp(array, (xmlChar*)"Y")),
                atoi((char*)xmlGetProp(array, (xmlChar*)"W")), 
                atoi((char*)xmlGetProp(array, (xmlChar*)"H"))
            );
            
            LoadingAnimation[ArrayID].AnimStates = (Vector2d*)malloc(sizeof(Vector2d)*LoadingAnimation[ArrayID].NbOfAnimStates);
            LoadingAnimation[ArrayID].AnimRange = (Vector2d*)malloc(sizeof(Vector2d)*LoadingAnimation[ArrayID].NbOfAnimStates);
            LoadingAnimation[ArrayID].AnimRuntime = (int*)malloc(sizeof(int)*LoadingAnimation[ArrayID].NbOfAnimStates);
            entry = array->children;
            
            EntryID = 0;
            while (entry){
                if (strcmp((char*)entry->name, "entry") == 0){
                    LoadingAnimation[ArrayID].AnimStates[EntryID] = InitVector2d(atof((char*)xmlGetProp(entry, (xmlChar*)"stateA")), atof((char*)xmlGetProp(entry, (xmlChar*)"stateB")));
                    LoadingAnimation[ArrayID].AnimRange[EntryID] = InitVector2d(atof((char*)xmlGetProp(entry, (xmlChar*)"rangeA")), atof((char*)xmlGetProp(entry, (xmlChar*)"rangeB")));
                    LoadingAnimation[ArrayID].AnimRuntime[EntryID] = atoi((char*)xmlGetProp(entry, (xmlChar*)"runtime"));
                    EntryID++;
                }
                entry = entry->next;
            }
            
            ArrayID++;
        }
        array = array->next;
    }

    return LoadingAnimation;
}

Vector2i* ParseScenesCoordinates(xmlNode* property){
    Vector2i* LoadingScenesCoordinates;
    xmlNode* entry;
    unsigned int EntryID;

    LoadingScenesCoordinates = (Vector2i*)malloc(xmlChildElementCount(property)*sizeof(Vector2i));
    entry = property->children;

    EntryID = 0;
    while (entry){
        if (strcmp((char*)entry->name, "scene") == 0){
            LoadingScenesCoordinates[EntryID] = InitVector2i(atoi((char*)xmlGetProp(entry, (xmlChar*)"X")), atoi((char*)xmlGetProp(entry, (xmlChar*)"Y")));
            EntryID++;
        }
        entry = entry->next;
    }

    return LoadingScenesCoordinates;
}

BackgroundContext* InitBackground(DisplayDevice* DDevice, char* ScenePath){
    /* Declaration */
    BackgroundContext* LoadingContext;
    xmlDoc* SceneFile;
    xmlNode *background, *property;
    char* SurfacePath, *Buffer;
    Uint32 ColorKey;

    /* Init */
    LoadingContext = (BackgroundContext*)calloc(1, sizeof(BackgroundContext));

    /* Logic */
    if (ScenePath){
        SceneFile = loadXml(ScenePath);                 /* Load the xml file in memory */
        background = xmlDocGetRootElement(SceneFile);   /* root node */

        if ((SurfacePath = (char*)xmlGetProp(background, (xmlChar*)"texture"))){
            if ((Buffer = (char*)xmlGetProp(background, (xmlChar*)"colorKey"))){
                sscanf(Buffer, "%x", &ColorKey);
                LoadingContext->Surface = LoadSurface(SurfacePath, DDevice, ColorKey, SURFACE_KEYED);
            } else {
                LoadingContext->Surface = LoadSurface(SurfacePath, DDevice, 0x0, SURFACE_OPAQUE);
            }
        }

        /* Parsing */
        property = background->children;
        while (property){
            if (strcmp((char*)property->name, "animArray") == 0){
                LoadingContext->Animation = ParseBGAnimation(property);
            } else if (strcmp((char*)property->name, "sceneArray") == 0){
                LoadingContext->ScenesCoordinates = ParseScenesCoordinates(property);
            }
            property = property->next;
        }
    }

    LoadingContext->SrcRect.w = BASE_RESOLUTION_X;
    LoadingContext->SrcRect.h = BASE_RESOLUTION_Y;
    LoadingContext->PlayingAnimation = -1;
    LoadingContext->Shown = true;

    xmlFreeDoc(SceneFile);

    return LoadingContext;
}

void MoveBackground(BackgroundContext* Context, int TileID, char Effect){ /* Change the background tile */
    if (TileID >= 0){
        Context->Shown = true;
        Context->SrcRect.x = Context->ScenesCoordinates[TileID].x;
        Context->SrcRect.y = Context->ScenesCoordinates[TileID].y;
        Context->ObjectLayerOffset = 0;

        /* Effects */
        switch (Effect){
            case 0:
                Context->Flipped = 0;
                break;
            case 1:
                Context->Flipped = 1;
                break;
        }
    } else {
        Context->Shown = false;
    }
}

void BackgroundPlayAnimation(BackgroundContext* Context, int AnimationID, bool* EndSignal, bool EndSignalValue){ /* Start the background animation */
    Context->PlayingAnimation = AnimationID;
    Context->StartFrame = Context->CurrentState = Context->AnimOffset = 0;
    Context->ObjectLayerOffset = 0;
    (*EndSignal) = !EndSignalValue;
    Context->EndSignal = EndSignal;
    Context->EndSignalValue = EndSignalValue;
}

/* FIXME: Curently only support Horizontal motion. It would be great to be able to have vertical animations as well */
void DisplayBackground(DisplayDevice* DDevice, BackgroundContext* Context){ /* Display the background on screen */
    SDL_Rect AnimSrcRect;
    SDL_Rect BGDstRect = {0, 0, BASE_RESOLUTION_X, BASE_RESOLUTION_Y};
    Uint32 TimeProgress;
    double Progress;
    
    double MaxProgress;
    int MaxOffset;
    int NewOffset;

    /* Only process the following if the background is actually shown*/
    if (!Context->Shown)
        return;

    if ((Context->PlayingAnimation >= 0) && (Context->CurrentState < Context->Animation[Context->PlayingAnimation].NbOfAnimStates)){ /* Background slide animation */
        
        if (Context->StartFrame == 0){
            Context->StartFrame = SDL_GetTicks();
        }

        if (Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].y > Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].x){
            
            TimeProgress = SDL_GetTicks() - Context->StartFrame;
            Progress = ((double)TimeProgress / (double)Context->Animation[Context->PlayingAnimation].AnimRuntime[Context->CurrentState]) * (Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].y - Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].x) + Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].x;

            if (Progress > Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].y){
                Progress = Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].y;
            }
            
            NewOffset = (Context->Animation[Context->PlayingAnimation].AnimStates[Context->CurrentState].x * (Progress * Progress)) + (Context->Animation[Context->PlayingAnimation].AnimStates[Context->CurrentState].y * Progress);
            Context->ObjectLayerOffset = NewOffset + Context->AnimOffset;
        }else {
            
            TimeProgress = Context->Animation[Context->PlayingAnimation].AnimRuntime[Context->CurrentState] - (SDL_GetTicks() - Context->StartFrame);
            
            if (TimeProgress > Context->Animation[Context->PlayingAnimation].AnimRuntime[Context->CurrentState]){
                TimeProgress = 0;
            }
            Progress = ((double)TimeProgress / (double)Context->Animation[Context->PlayingAnimation].AnimRuntime[Context->CurrentState]) * (Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].x - Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].y) + Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].y;

            if (Progress < Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].y){
                Progress = Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].y;
            }

            MaxProgress = Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].x;
            MaxOffset = (Context->Animation[Context->PlayingAnimation].AnimStates[Context->CurrentState].x * (MaxProgress * MaxProgress)) + (Context->Animation[Context->PlayingAnimation].AnimStates[Context->CurrentState].y * MaxProgress);/* + Context->AnimOffset; */
            
            NewOffset = (Context->Animation[Context->PlayingAnimation].AnimStates[Context->CurrentState].x * (Progress * Progress)) + (Context->Animation[Context->PlayingAnimation].AnimStates[Context->CurrentState].y * Progress);
            Context->ObjectLayerOffset = MaxOffset - NewOffset + Context->AnimOffset;
        }
        AnimSrcRect.x =  Context->ObjectLayerOffset + Context->Animation[Context->PlayingAnimation].AnimRegion.x; /* + Offset */

        AnimSrcRect.y = Context->Animation[Context->PlayingAnimation].AnimRegion.y;
        AnimSrcRect.w = BASE_RESOLUTION_X;
        AnimSrcRect.h = BASE_RESOLUTION_Y;

        /* Effects */
        /* Not used for now :3 */
        /*
        switch (Context->Animation[Context->PlayingAnimation].AnimEffects[Context->CurrentState]){
            case 0:
                Context->Flipped = 0;
                Context->SrcRect = AnimSrcRect;
                break;
            case 1:
                Context->Flipped = 1;
                Context->SrcRect = AnimSrcRect;
                break;
        }
        */

        Context->SrcRect = AnimSrcRect; /* Used to sync up to other layers */

        ScaledDraw(DDevice, Context->Surface, &AnimSrcRect, &BGDstRect);
        
        if (Progress == Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].y){
            Context->CurrentState++;
            Context->StartFrame = 0;
            /*Context->AnimOffset = AnimSrcRect.x; */
            Context->AnimOffset = Context->ObjectLayerOffset;
            if (Context->CurrentState == Context->Animation[Context->PlayingAnimation].NbOfAnimStates){
                /*Context->SrcRect = AnimSrcRect; */
                Context->SrcRect.x = Context->Animation[Context->PlayingAnimation].AnimRegion.w;
                Context->SrcRect.y = Context->Animation[Context->PlayingAnimation].AnimRegion.h;
                Context->PlayingAnimation = -1;
                if (Context->EndSignal){
                    *(Context->EndSignal) = Context->EndSignalValue;
                }
            }
        }

    }else{ /* Display the current background tile if not animated */
        ScaledDraw(DDevice, Context->Surface, &Context->SrcRect, &BGDstRect);
    }
}

xmlNode* searchNodeLabel(xmlNode* entry, char* label){
    xmlNode* root;
    char* checkLabel;
    root = entry->parent->children;
    if (!label)
        return NULL;
    while (root){
        checkLabel = (char*)xmlGetProp(root, (xmlChar*)"label");
        if (checkLabel){
            if (strcmp(checkLabel, label) == 0){
                return root;
            }
        }
        root = root->next;
    }
    return NULL;
}

xmlNode* searchNode(xmlNode* entry, char* name){
    if (entry->parent)
        entry = entry->parent->children;
    while (entry){
        if (strcmp((char*)entry->name, name) == 0){
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

Characters** initCharArray(DisplayDevice* DDevice, xmlNode* array){
    Characters** CharactersIndex;
    unsigned int ArrayID;

    CharactersIndex = (Characters**)malloc(xmlChildElementCount(array)*sizeof(Characters*));
    array = array->children;
    
    ArrayID = 0;
    while (array){
        if (strcmp((char*)array->name, "init") == 0) {
            CharactersIndex[ArrayID] = InitCharacter(DDevice, (char*)xmlGetProp(array, (xmlChar*)"path"));
            ArrayID++;
        }
        array = array->next;
    }

    return CharactersIndex;
}

void parseFlags(DisplayDevice* DDevice, SceneContext* SContext, xmlNode* element){
    char* CharBuffer;
    int IntBuffer;
    xmlNode *searchNode;

    SContext->press = NULL;
    SContext->presentItem = -1;
    SContext->presentDefault = NULL;
    SContext->presentMatch = NULL;
    while (element){
        if (strcmp((char*)element->name, "setBG") == 0) {
            MoveBackground(SContext->BGContext, atoi((char*)xmlGetProp(element, (xmlChar*)"value")), 0);
        } else if (strcmp((char*)element->name, "setBGAnim") == 0) {
            BackgroundPlayAnimation(SContext->BGContext, atoi((char*)xmlGetProp(element, (xmlChar*)"value")), &SContext->IDevice->EventEnabled, true);
        } else if (strcmp((char*)element->name, "setPic") == 0) {
            MoveBackground(SContext->ScenePics, atoi((char*)xmlNodeGetContent(element)), 0);
        } else if (strcmp((char*)element->name, "setBGM") == 0){
            IntBuffer = atoi((char*)xmlGetProp(element, (xmlChar*)"value"));
            if (IntBuffer >= 0){
                if (IntBuffer != GetTrackID())
                    PlayTrackID(IntBuffer);
            } else {
                StopTrack();
            }
        } else if (strcmp((char*)element->name, "setUI") == 0){
            CharBuffer = (char*)xmlGetProp(element, (xmlChar*)"param");
            setUI((unsigned int)atoi((char*)xmlGetProp(element, (xmlChar*)"value")), (CharBuffer) ? atoi(CharBuffer) : 0);
        } else if (strcmp((char*)element->name, "setREW") == 0){
            SContext->diagMode = (unsigned char)(atoi((char*)xmlGetProp(element, (xmlChar*)"value")) + 1);
        } else if (strcmp((char*)element->name, "setDIAG") == 0){
            SContext->DiagShown = (bool)atoi((char*)xmlGetProp(element, (xmlChar*)"value"));
        } else if (strcmp((char*)element->name, "setCR") == 0){
            ResetCourtRecordPos();
            SContext->activateCourtRecord = true;
            SContext->IDevice->EventEnabled = false;
            SContext->diagMode = 0;
        } else if (strcmp((char*)element->name, "jump") == 0) {
            CharBuffer = (char*)xmlNodeGetContent(element);
            searchNode = searchNodeLabel(SContext->entry, CharBuffer);
            if (searchNode){
                SContext->entry = searchNode;
                SContext->Jump = true;
            }
        } else if (strcmp((char*)element->name, "press") == 0) {
            CharBuffer = (char*)xmlNodeGetContent(element);
            searchNode = searchNodeLabel(SContext->entry, CharBuffer);
            if (searchNode)
                SContext->press = searchNode;
        } else if (strcmp((char*)element->name, "present") == 0) {
            CharBuffer = (char*)xmlGetProp(element, (xmlChar*)"item");
            if (CharBuffer){
                SContext->presentItem = atoi(CharBuffer);
            }
            searchNode = searchNodeLabel(SContext->entry, (char*)xmlGetProp(element, (xmlChar*)"match"));
            if (searchNode)
                SContext->presentMatch = searchNode;
            searchNode = searchNodeLabel(SContext->entry, (char*)xmlGetProp(element, (xmlChar*)"default"));
            if (searchNode)
                SContext->presentDefault = searchNode;
            SContext->returnTarget = SContext->entry;
        } else if (strcmp((char*)element->name, "giveItem") == 0){
            AddItemToCourtRecord(atoi((char*)xmlNodeGetContent(element)));
        } else if (strcmp((char*)element->name, "removeItem") == 0){
            IntBuffer = atoi((char*)xmlNodeGetContent(element));
            if (IntBuffer > -1){
                RemoveItemFromCourtRecord(IntBuffer);
            } else {
                EmptyCourtRecord();
            }
        } else if (strcmp((char*)element->name, "return") == 0){
            SContext->next = SContext->returnTarget;
        } else if (strcmp((char*)element->name, "addToLayer") == 0) {
            CharBuffer = (char*)xmlGetProp(element, (xmlChar*)"flip");
            AddCharacterToLayer(
                SContext->CharaLayer,
                SContext->CharactersIndex[(unsigned int)atoi((char*)xmlGetProp(element, (xmlChar*)"char"))],
                SContext->BGContext,
                (int)atoi((char*)xmlGetProp(element, (xmlChar*)"dest")),
                (CharBuffer) ? (bool)atoi(CharBuffer) : 0
            );
        } else if (strcmp((char*)element->name, "moveOnLayer") == 0) {
            CharBuffer = (char*)xmlGetProp(element, (xmlChar*)"flip");
            MoveCharacterOnLayer(
                SContext->CharaLayer,
                SContext->BGContext,
                (unsigned int)atoi((char*)xmlGetProp(element, (xmlChar*)"char")),
                (int)atoi((char*)xmlGetProp(element, (xmlChar*)"dest")),
                (CharBuffer) ? (bool)atoi(CharBuffer) : 0
            );
        } else if (strcmp((char*)element->name, "removeFromLayer") == 0) {
            DeleteCharacterFromLayer(
                SContext->CharaLayer,
                SContext->CharactersIndex[(unsigned int)atoi((char*)xmlGetProp(element, (xmlChar*)"char"))]
            );
        }
        element = element->next;
    }
}

SceneContext* InitScene(DisplayDevice* DDevice, InputDevice* IDevice, DialogueContext* DiagContext, ButtonsContext* BContext, CourtroomContext* CContext, char* ScenePath){
    /* Declaration */
    xmlDoc* sceneFile;
    SceneContext* LoadingScene;
    xmlNode* rootNode, *element;

    /* Init */
    LoadingScene = (SceneContext*)malloc(sizeof(SceneContext));
    sceneFile = loadXml(ScenePath);
    rootNode = xmlDocGetRootElement(sceneFile);
    LoadingScene->BGContext = InitBackground(DDevice, (char*)xmlGetProp(rootNode, (xmlChar*)"background"));
    LoadingScene->ScenePics = InitBackground(DDevice, (char*)xmlGetProp(rootNode, (xmlChar*)"scenePics"));
    LoadingScene->ScenePics->Shown = false;

    LoadingScene->IDevice = IDevice;
    LoadingScene->DiagContext = DiagContext;
    LoadingScene->BContext = BContext;
    LoadingScene->CContext = CContext;
    LoadingScene->press = NULL;
    LoadingScene->Jump = false;
    LoadingScene->presentItem = -1;
    LoadingScene->presentMatch = NULL;
    LoadingScene->presentDefault = NULL;
    LoadingScene->DiagShown = true;
    LoadingScene->activateCourtRecord = false;
    LoadingScene->diagMode = 1;

    LoadingScene->CharactersIndex = NULL;
    LoadingScene->CharaLayer = NULL;
    InitCharacterLayer(&LoadingScene->CharaLayer, LoadingScene->BGContext);

    LoadingScene->entry = searchNode(rootNode->children, "entry");
    LoadingScene->returnTarget = LoadingScene->entry;
    LoadingScene->next = LoadingScene->entry->next;

    element = searchNode(rootNode->children, "setup");
    if (element) {
        element = element->children;
        while (element){
            if (strcmp((char*)element->name, "initChar") == 0) {
                LoadingScene->CharactersIndex = initCharArray(DDevice, element);
            } else if (strcmp((char*)element->name, "flags") == 0) {
                parseFlags(DDevice, LoadingScene, element->children);
            }
            element = element->next;
        }
    }

    return LoadingScene;
}

void FreeScene(SceneContext* SceneContext){

}

size_t parseDiag(SceneContext* SContext, xmlNode* element){
    int CurrentCharacterID; /* Hold the name of the character who is talking during the dialog */
    char* DialogueText; /* Hold the text of the dialogue itself */

    while (element){
        if (strcmp((char*)element->name, "char") == 0) {
            CurrentCharacterID = atoi((char*)xmlNodeGetContent(element));
        } else if (strcmp((char*)element->name, "line") == 0) {
            DialogueText = (char*)xmlNodeGetContent(element);
        }
        element = element->next;
    }
    SContext->CContext->CurrentCharacter = CurrentCharacterID;

    return SetDialogueText(SContext->DiagContext, SContext->CharactersIndex[CurrentCharacterID]->DisplayName, DialogueText, 1);
}

void parseAnim(SceneContext* SContext, xmlNode* element){
    int animTarget;
    int animationID;

    while (element){
        if (strcmp((char*)element->name, "animTarget") == 0) {
            animTarget = atoi((char*)xmlNodeGetContent(element));
        } else if (strcmp((char*)element->name, "animation") == 0) {
            animationID = atoi((char*)xmlNodeGetContent(element));
        } else if (strcmp((char*)element->name, "idleAnim") == 0) {
            SContext->CContext->IdleAnimation = atoi((char*)xmlNodeGetContent(element)); /* Need to be setup */
        }
        element = element->next;
    }
    CharacterPlayAnimation(SContext->CharactersIndex[animTarget], animationID);
}

void parseButtons(SceneContext* SContext, xmlNode* element){
    int buttonJumpIndex;

    ClearButtons(SContext->BContext);
    buttonJumpIndex = 0;
    while (element){
        if (strcmp((char*)element->name, "option") == 0) {
            AddButton(SContext->BContext, (char*)xmlNodeGetContent(element));
            SContext->CContext->ButtonJumpLabels[buttonJumpIndex] = (char*)xmlGetProp(element, (xmlChar*)"jump");
            buttonJumpIndex++;
        }
        element = element->next;
    }
    SContext->IDevice->EventEnabled = false;
    SContext->CContext->ButtonActivated = 1;
}

void parseScene(DisplayDevice* DDevice, SceneContext* SContext){
    /* Declaration */
    xmlNode *property;
    /* diag */
    size_t lineSize = 0; /* Hold the size of the dialog who is about to be played */

    /* Logic */
    SContext->next = SContext->entry->next;
    property = SContext->entry->children;
    while (property){
        if (strcmp((char*)property->name, "diag") == 0){
            lineSize = parseDiag(SContext, property->children);
        } else if (strcmp((char*)property->name, "anim") == 0) {
            parseAnim(SContext, property->children);
        } else if (strcmp((char*)property->name, "buttons") == 0) {
            parseButtons(SContext, property->children);
        } else if (strcmp((char*)property->name, "flags") == 0) {
            parseFlags(DDevice, SContext, property->children);
        }
        property = property->next;
    }

    /* Post parsing */
    if (lineSize)   /* We don't want to modify Return to default unless a new dialogue is set */
        SContext->CContext->ReturnToDefault = lineSize;
}

void SceneForward(SceneContext* SContext){
    if (SContext->Jump) {
        SContext->Jump = false;
    } else if (SContext->entry->next){
        SContext->entry = SContext->next;
    }
}


void SceneBackward(SceneContext* SContext){
    if (SContext->entry->prev)
        SContext->entry = SContext->entry->prev;
}

void FreeBGAnimation(BGAnimation* AnimationToFree){
    if (AnimationToFree->AnimStates)
        free(AnimationToFree->AnimStates);
    
    if (AnimationToFree->AnimRuntime)
        free(AnimationToFree->AnimRuntime);

    if (AnimationToFree->AnimRange)
        free(AnimationToFree->AnimRange);
}

void FreeBackground(BackgroundContext* SceneToFree){
    if (SceneToFree->Surface)
    #ifdef _SDL
        free(SceneToFree->Surface);
    #else 
        SDL_DestroyTexture(SceneToFree->Surface);
    #endif

    if (SceneToFree->Animation)
        FreeBGAnimation(SceneToFree->Animation);

    free(SceneToFree->ScenesCoordinates);
    free(SceneToFree);
}