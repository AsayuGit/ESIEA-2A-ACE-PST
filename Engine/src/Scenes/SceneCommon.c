#include "SceneCommon.h"
#include "Characters.h"
#include "CourtReccord.h"

// First background animation (90° turn)
Vector2d CourtAnim1States[3] = {
    {0.0, 1.6},
    {4.0, 4.0},
    {0.0, 1.6}
};

int CourtAnim1Runtime[3] = {
    64,
    320,
    64
};

Vector2d CourtAnim1Range[3] = {
    {0.0, 10.0},
    {0.0, 10.56},
    {0.0, 10.0}
};

char CourtAnim1Effects[3] = {
    0, 
    0,
    0
};

// Second background animation (180° turn)
Vector2d CourtAnim2States[4] = {
    {0.0, 1.6},
    {4.0, 4.0},
    {4.0, 4.0},
    {0.0, 1.6}
};

int CourtAnim2Runtime[4] = {
    64,
    320,
    320,
    64
};

Vector2d CourtAnim2Range[4] = {
    {0.0, 10.0},
    {0.0, 10.72}, // To tweak
    {10.72, 0.0},
    {10.0, 0.0}
};

char CourtAnim2Effects[4] = {
    0,
    0,
    0,
    0
};

// Third background animation (Reverse 180° turn)
Vector2d CourtAnim3States[4] = {
    {0.0, -1.6},
    {-4.0, -4.0},
    {-4.0, -4.0},
    {0.0, -1.6}
};

int CourtAnim3Runtime[4] = {
    64,
    320,
    320,
    64
};

Vector2d CourtAnim3Range[4] = {
    {0.0, 10.0},
    {0.0, 10.74}, // To tweak
    {10.74, 0.0},
    {10.0, 0.0}
};

char CourtAnim3Effects[4] = {
    0,
    0,
    0,
    0
};

BGAnimation CourtAnim[3] = {
    {
        3,                  // NbOfAnimStates
        CourtAnim1States,
        CourtAnim1Runtime,
        CourtAnim1Range, // X max form 0 to +infinity
        CourtAnim1Effects,
        {0, 0, 648, 0}
    },
    {
        4,
        CourtAnim2States,
        CourtAnim2Runtime,
        CourtAnim2Range,
        CourtAnim2Effects,
        {0, 0, 1040, 0}
    },
    {
        4,
        CourtAnim3States,
        CourtAnim3Runtime,
        CourtAnim3Range,
        CourtAnim3Effects,
        {1040, 0, 0, 0}
    }
};

// Title Screen
Vector2d TitleScreen1States[1] = {
    {0.0, 25.6}
};

int TitleScreen1Runtime[1] = {
    500
};

Vector2d TitleScreen1Range[1] = {
    {0.0, 10.0}
};

char TitleScreen1Effects[1] = {
    0
};

//
Vector2d TitleScreen2States[1] = {
    {0.0, -25.6}
};

int TitleScreen2Runtime[1] = {
    500
};

Vector2d TitleScreen2Range[1] = {
    {0.0, 10.0}
};

char TitleScreen2Effects[1] = {
    0
};


BGAnimation TitleScreenAnim[2] = {
    {
        1,                  // NbOfAnimStates
        TitleScreen1States,
        TitleScreen1Runtime,
        TitleScreen1Range, // X max form 0 to +infinity
        TitleScreen1Effects,
        {0, 0, 256, 0}
    },
    {
        1,                  // NbOfAnimStates
        TitleScreen2States,
        TitleScreen2Runtime,
        TitleScreen2Range, // X max form 0 to +infinity
        TitleScreen2Effects,
        {256, 0, 0, 0}
    }
};

Vector2i CourtScenes[6] = {
    {0, 0}, 
    {1040, 0},
    {519, 0},
    {0, 192},
    {256, 192},
    {512, 192}
};

Vector2i EmptyScenes[1] = {
    {0, 0}
};

Background Scenes[BackgroundsCount] = {
    {// S_Empty
        NULL,
        TitleScreenAnim,
        1,
        EmptyScenes
    },
    {// S_Courtroom
        ROOT""TEXTURES"Places"SL"Courtroom"TEX_EXT,
        CourtAnim,
        6,
        CourtScenes
    },
    {// S_TitleScreen
        ROOT""TEXTURES"Menus"SL"TitleScreen"TEX_EXT,
        TitleScreenAnim,
        1,
        EmptyScenes
    }
};

// Idée : Remplacer BackgroundID avec un "Background*" de cette façon les scènes gêrent elles meme leurs bg
SceneContext* InitScene(DisplayDevice* DDevice, int BackgroundID){
    SceneContext* LoadingContext;
    LoadingContext = (SceneContext*)malloc(sizeof(SceneContext));
    LoadingContext->PlayingAnimation = -1;
    LoadingContext->TileID = LoadingContext->SrcRect.x = LoadingContext->SrcRect.y = LoadingContext->ObjectLayerOffset = LoadingContext->Flipped = 0;
    LoadingContext->Surface = LoadSurface((char*)Scenes[BackgroundID].SurfacePath, DDevice, NULL, false);
    #ifdef _SDL
        LoadingContext->SurfaceBounds.x = LoadingContext->Surface->w;
        LoadingContext->SurfaceBounds.y = LoadingContext->Surface->h;
    #else
        SDL_QueryTexture(LoadingContext->Surface, NULL, NULL, &LoadingContext->SurfaceBounds.x, &LoadingContext->SurfaceBounds.y);
    #endif
    LoadingContext->SrcRect.w = DDevice->ScreenResolution.x;
    LoadingContext->SrcRect.h = DDevice->ScreenResolution.y;
    LoadingContext->Animation = Scenes[BackgroundID].Animation;
    LoadingContext->nbOfScenes = Scenes[BackgroundID].nbOfScenes;
    LoadingContext->ScenesCoordinates = Scenes[BackgroundID].ScenesCoordinates;
    return LoadingContext;
}

void MoveTile(SceneContext* Context, int TileID, char Effect){ // Change the background tile

    /*    
    Vector2i Coordinates;

    Coordinates = RectTileToCorrdinate(Context->SrcRect, Context->SurfaceBounds, TileX, TileY);
    Context->SrcRect.y = Coordinates.y;
    Context->SrcRect.x = Coordinates.x;*/

    Context->SrcRect.x = Context->ScenesCoordinates[TileID].x;
    Context->SrcRect.y = Context->ScenesCoordinates[TileID].y;
    Context->ObjectLayerOffset = 0;

    // Effects
    switch (Effect){
        case 0:
            Context->Flipped = 0;
            break;
        case 1:
            Context->Flipped = 1;
            break;
    }
}

void BackgroundPlayAnimation(SceneContext* Context, int AnimationID, char* AnimState){ // Start the background animation
    Context->PlayingAnimation = AnimationID;
    Context->StartFrame = Context->CurrentState = Context->AnimOffset = 0;
    Context->ObjectLayerOffset = 0;
    (*AnimState) = 0;
    Context->AnimState = AnimState;
}

void DisplayBackground(DisplayDevice* DDevice, SceneContext* Context){ // Display the background on screen
    SDL_Rect AnimSrcRect, AnimDstRect;
    Uint32 TimeProgress;
    double Progress;
    
    double MaxProgress;
    int MaxOffset;
    int NewOffset;

    if ((Context->PlayingAnimation >= 0) && (Context->CurrentState < Context->Animation[Context->PlayingAnimation].NbOfAnimStates)){ // Background slide animation
        
        if (Context->StartFrame == 0){
            Context->StartFrame = SDL_GetTicks();
            //printf("POC !\n");
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
            MaxOffset = (Context->Animation[Context->PlayingAnimation].AnimStates[Context->CurrentState].x * (MaxProgress * MaxProgress)) + (Context->Animation[Context->PlayingAnimation].AnimStates[Context->CurrentState].y * MaxProgress);// + Context->AnimOffset;
            
            NewOffset = (Context->Animation[Context->PlayingAnimation].AnimStates[Context->CurrentState].x * (Progress * Progress)) + (Context->Animation[Context->PlayingAnimation].AnimStates[Context->CurrentState].y * Progress);
            Context->ObjectLayerOffset = MaxOffset - NewOffset + Context->AnimOffset;
        }
        AnimSrcRect.x =  Context->ObjectLayerOffset + Context->Animation[Context->PlayingAnimation].AnimRegion.x; // + Offset
        //printf("Region %d | Offset %d\n", Context->Animation[Context->PlayingAnimation].AnimRegion.x, Context->ObjectLayerOffset);
        AnimSrcRect.y = Context->Animation[Context->PlayingAnimation].AnimRegion.y;
        AnimSrcRect.w = DDevice->ScreenResolution.x;
        AnimSrcRect.h = DDevice->ScreenResolution.y;

        AnimDstRect.x = AnimDstRect.y = 0;
        AnimDstRect.w = DDevice->ScreenResolution.x;
        AnimDstRect.h = DDevice->ScreenResolution.y;

        // Effects
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

        #ifdef _SDL
            SDL_BlitSurface(Context->Surface, &AnimSrcRect, DDevice->Screen, &AnimDstRect); 
        #else
            SDL_RenderCopyEx(DDevice->Renderer, Context->Surface, &AnimSrcRect, &AnimDstRect, 0, 0, Context->Flipped);
        #endif
        
        if (Progress == Context->Animation[Context->PlayingAnimation].AnimRange[Context->CurrentState].y){
            Context->CurrentState++;
            Context->StartFrame = 0;
            //Context->AnimOffset = AnimSrcRect.x;
            Context->AnimOffset = Context->ObjectLayerOffset;
            if (Context->CurrentState == Context->Animation[Context->PlayingAnimation].NbOfAnimStates){
                //Context->SrcRect = AnimSrcRect;
                Context->SrcRect.x = Context->Animation[Context->PlayingAnimation].AnimRegion.w;
                Context->SrcRect.y = Context->Animation[Context->PlayingAnimation].AnimRegion.h;
                Context->PlayingAnimation = -1;
                if (Context->AnimState){
                    *(Context->AnimState) = 1;
                }
            }
        }

    }else{ // Display the current background tile if not animated
        #ifdef _SDL
            SDL_BlitSurface(Context->Surface, &Context->SrcRect, DDevice->Screen, NULL); 
        #else
            SDL_RenderCopyEx(DDevice->Renderer, Context->Surface, &Context->SrcRect, NULL, 0, 0, Context->Flipped);
        #endif
    }
}

xmlDoc* loadScene(char* filePath){ // To move to load.c (maybe)
    xmlKeepBlanksDefault(0); // Ignore white space
    return xmlReadFile(filePath, NULL, 0); // Load File into memory
}

xmlNode* searchSceneNode(xmlNode** entry, char* label){
    xmlNode* root;
    char* checkLabel;
    root = (*entry)->parent->children;
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

void parseScene(xmlNode** entry, DialogueContext* DiagContext, SceneContext* SContext, ButtonsContext* BContext, Characters** CharactersIndex, int NbOfCharacters, int* IdleAnimation, int* ReturnToDefault, int* CurrentCharacter, char* BGAnimComplete, char* ButtonActivated, char** ButtonJumpLabels){
    // Declaration
    xmlNode *searchNode, *property, *element, *next;
    // diag
    int CurrentCharacterID; // Hold the name of the character who is talking during the dialog
    char* DialogueText; // Hold the text of the dialogue itself
    int lineSize; // Hold the size of the dialog who is about to be played
    // anim
    int animTarget;
    int animationID;
    // jump
    char* jumpLabel;
    // Buttons
    int buttonJumpIndex;
    // Item
    int ItemBuffer;
    // Logic
    next = ((*entry)->next) ? (*entry)->next : (*entry);
    property = (*entry)->children;
    while (property){
        if (strcmp((char*)property->name, "diag") == 0){
            element = property->children;
            while (element){
                if (strcmp((char*)element->name, "char") == 0) {
                    CurrentCharacterID = atoi((char*)xmlNodeGetContent(element));
                } else if (strcmp((char*)element->name, "line") == 0) {
                    //*ReturnToDefault = SetDialogueText(DiagContext, (*CurrentCharacter)->DisplayName, "The court is now in session for\nthe trial of Mr. Larry Butz.", 1);
                    DialogueText = (char*)xmlNodeGetContent(element);
                }
                element = element->next;
            }
            lineSize = SetDialogueText(DiagContext, CharactersIndex[CurrentCharacterID]->DisplayName, DialogueText, 1);
            *CurrentCharacter = CurrentCharacterID;
        } else if (strcmp((char*)property->name, "anim") == 0) {
            element = property->children;
            while (element){
                if (strcmp((char*)element->name, "animTarget") == 0) {
                    animTarget = atoi((char*)xmlNodeGetContent(element));
                } else if (strcmp((char*)element->name, "animation") == 0) {
                    animationID = atoi((char*)xmlNodeGetContent(element));
                } else if (strcmp((char*)element->name, "idleAnim") == 0) {
                    (*IdleAnimation) = atoi((char*)xmlNodeGetContent(element)); // Need to be setup
                    (*ReturnToDefault) = lineSize;
                }
                element = element->next;
            }
            CharacterPlayAnimation(CharactersIndex[animTarget], animationID);
        } else if (strcmp((char*)property->name, "buttons") == 0) {
            ClearButtons(BContext);
            buttonJumpIndex = 0;
            element = property->children;
            while (element){
                if (strcmp((char*)element->name, "option") == 0) {
                    AddButton(BContext, (char*)xmlNodeGetContent(element));
                    ButtonJumpLabels[buttonJumpIndex] = (char*)xmlGetProp(element, (xmlChar*)"jump");
                    buttonJumpIndex++;
                }
                element = element->next;
            }
            (*BGAnimComplete) = 0;
            (*ButtonActivated) = 1;
        } else if (strcmp((char*)property->name, "setBackground") == 0) {
            MoveTile(SContext, atoi((char*)xmlNodeGetContent(property)), 0);
        } else if (strcmp((char*)property->name, "backgroundAnim") == 0) {
            BackgroundPlayAnimation(SContext, atoi((char*)xmlNodeGetContent(property)), BGAnimComplete);
        } else if (strcmp((char*)property->name, "jump") == 0) {
            jumpLabel = (char*)xmlNodeGetContent(property);
            searchNode = searchSceneNode(entry, jumpLabel);
            if (searchNode)
                next = searchNode;
        } else if (strcmp((char*)property->name, "giveItem") == 0){
            AddItemToCourtReccord(atoi((char*)xmlNodeGetContent(property)));
        } else if (strcmp((char*)property->name, "removeItem") == 0){
            ItemBuffer = atoi((char*)xmlNodeGetContent(property));
            if (ItemBuffer > -1){
                RemoveItemFromCourtReccord(ItemBuffer);
            } else {
                EmptyCourtReccord();
            }
        } else if (strcmp((char*)property->name, "playBGM") == 0){
            PlayTrackID(atoi((char*)xmlNodeGetContent(property)));
        }
        property = property->next;
    }
    (*entry) = next;
}