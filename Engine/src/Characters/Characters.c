#include "Characters.h"

Animation* ParseCHAnimation(xmlNode* property){
    Animation* LoadingAnimation;
    xmlNode *array, *entry;
    unsigned int ArrayID;

    LoadingAnimation = (Animation*)malloc(xmlChildElementCount(property)*sizeof(Animation));
    array = property->children;

    ArrayID = 0;
    while (array){
        if (strcmp((char*)array->name, "anim") == 0) {

            LoadingAnimation[ArrayID].NbOfFrames = atoi((char*)xmlGetProp(array, (xmlChar*)"nbOfFrames"));
            LoadingAnimation[ArrayID].Framerate = atoi((char*)xmlGetProp(array, (xmlChar*)"framerate"));

            entry = array->children;
            while (entry){
                if (strcmp((char*)entry->name, "src") == 0){
                    LoadingAnimation[ArrayID].SrcRect = InitRect(
                        atoi((char*)xmlGetProp(entry, (xmlChar*)"X")),
                        atoi((char*)xmlGetProp(entry, (xmlChar*)"Y")),
                        atoi((char*)xmlGetProp(entry, (xmlChar*)"W")),
                        atoi((char*)xmlGetProp(entry, (xmlChar*)"H"))
                    );
                    /*printf("Src %d %d %d %d\n", LoadingAnimation->SrcRect.x, LoadingAnimation->SrcRect.y, LoadingAnimation->SrcRect.w, LoadingAnimation->SrcRect.h);*/
                } else if (strcmp((char*)entry->name, "dst") == 0){
                    LoadingAnimation[ArrayID].DstRect = InitRect(
                        atoi((char*)xmlGetProp(entry, (xmlChar*)"X")),
                        atoi((char*)xmlGetProp(entry, (xmlChar*)"Y")),
                        atoi((char*)xmlGetProp(entry, (xmlChar*)"W")),
                        atoi((char*)xmlGetProp(entry, (xmlChar*)"H"))
                    );
                    /*printf("Dst %d %d %d %d\n", LoadingAnimation->DstRect.x, LoadingAnimation->DstRect.y, LoadingAnimation->DstRect.w, LoadingAnimation->DstRect.h);*/
                }
                entry = entry->next;
            }
            ArrayID++;
        }
        array = array->next;
    }

    return LoadingAnimation;
}

Characters* InitCharacter(DisplayDevice* DDevice, char* CharacterPath){
    /* Declaration */
    Characters* LoadingCharacter;
    xmlDoc* CharacterFile;
    xmlNode *character, *property;
    Uint32 ColorKey;
    char* SurfacePath, *Buffer;

    /* Init */
    LoadingCharacter = (Characters*)calloc(1, sizeof(Characters));

    if (CharacterPath){
        CharacterFile = loadXml(CharacterPath); /* Load the xml file in memory */
        character = xmlDocGetRootElement(CharacterFile); /* root node */

        /* Logic */
        if ((SurfacePath = (char*)xmlGetProp(character, (xmlChar*)"texture"))){
            if ((Buffer = (char*)xmlGetProp(character, (xmlChar*)"colorKey"))){
                sscanf(Buffer, "%x", &ColorKey);
                LoadingCharacter->Surface = LoadSurface(SurfacePath, DDevice, ColorKey, SURFACE_KEYED);
            } else {
                LoadingCharacter->Surface = LoadSurface(SurfacePath, DDevice, 0x0, SURFACE_OPAQUE);
            }
        }

        /* Parsing */
        property = character->children;
        while (property){
            if (strcmp((char*)property->name, "name") == 0){
                astrcpy(&LoadingCharacter->DisplayName ,(char*)xmlNodeGetContent(property));
            } else if (strcmp((char*)property->name, "animArray") == 0){
                LoadingCharacter->Anim = ParseCHAnimation(property);
            }
            property = property->next;
        }

        if (!LoadingCharacter->DisplayName){
            LoadingCharacter->DisplayName = "Default";
        }
    }

    xmlFreeDoc(CharacterFile);

    return LoadingCharacter;
}

void CharacterPlayAnimation(Characters* Character, int AnimationID){
    Character->PlayingAnimation = AnimationID;
    Character->CurrentFrame = 0;
}

void InitCharacterLayer(CharacterLayer** CharaLayer, BackgroundContext* SContext){
    if ((*CharaLayer) == NULL){
        (*CharaLayer) = (CharacterLayer*)malloc(sizeof(CharacterLayer));
        (*CharaLayer)->CharaList = NULL;
        
        (*CharaLayer)->Viewport = &(SContext->SrcRect);
        (*CharaLayer)->Flipped = &(SContext->Flipped);
    }
}

void AddCharacterToLayer(CharacterLayer* CharaLayer, Characters* Character, BackgroundContext* SContext, int TileID, char Flip, DisplayDevice* DDevice){  /* Add a new character to a CharacterLayer */
    CharacterList** CharaList;

    if (!CharaLayer)
        return; /* error */
    CharaList = &(CharaLayer->CharaList);
    while ((*CharaList) != NULL){
        CharaList = &((*CharaList)->NextCharacter);
    }
    (*CharaList) = (CharacterList*)malloc(sizeof(CharacterList));
    (*CharaList)->Character = Character;

    (*CharaList)->Coordinates.x = SContext->ScenesCoordinates[TileID].x;
    (*CharaList)->Coordinates.y = SContext->ScenesCoordinates[TileID].y;
    (*CharaList)->Flip = Flip;
    (*CharaList)->NextCharacter = NULL;
}

void DeleteCharacterFromLayer(CharacterLayer* CharaLayer, Characters* Character){ /* Delete a character from a CharacterLayer, Due for a rework */
    CharacterList** CurrentCharacter;
    CharacterList* PrevLayer;
    CharacterList* NextLayer;

    if (!CharaLayer)
        return; /* error */
    CurrentCharacter = &(CharaLayer->CharaList);
    
    if ((*CurrentCharacter)->Character == Character){
        NextLayer = (*CurrentCharacter)->NextCharacter;
        free(*CurrentCharacter);
        (*CurrentCharacter) = NextLayer;
    }else{
        do {
            PrevLayer = (*CurrentCharacter);
            CurrentCharacter = &((*CurrentCharacter)->NextCharacter);
            if ((*CurrentCharacter)->Character == Character){
                NextLayer = (*CurrentCharacter)->NextCharacter;
                free(*CurrentCharacter);
                PrevLayer->NextCharacter = NextLayer;
                break;
            }
        } while ((*CurrentCharacter) != NULL);
    }
}

void DisplayCharacter(DisplayDevice* DDevice, Characters* Character, SDL_Rect Viewport, Vector2i Coordinates, char Flip){ /* Display "A" Character on screen  */
    SDL_Rect SpriteWindow, SpriteLayer;

    /* On veille a ne pas dépacer le nombre de frames de l'animation */
    if (Character->CurrentFrame >= Character->Anim[Character->PlayingAnimation].NbOfFrames){
        Character->CurrentFrame = 0;
    }
    
    /* On déplace la fenêtre dans la spritesheet en fonction du numéro de la frame */
    SpriteWindow.x = Character->Anim[Character->PlayingAnimation].SrcRect.x + Character->CurrentFrame * Character->Anim[Character->PlayingAnimation].SrcRect.w;
    SpriteWindow.y = Character->Anim[Character->PlayingAnimation].SrcRect.y;
    SpriteWindow.w = Character->Anim[Character->PlayingAnimation].SrcRect.w;
    SpriteWindow.h = Character->Anim[Character->PlayingAnimation].SrcRect.h;

    SpriteLayer = Character->Anim[Character->PlayingAnimation].DstRect;
    SpriteLayer.x += Coordinates.x - Viewport.x;
    SpriteLayer.y += Coordinates.y - Viewport.y;

    /* On affiche la frame d'animation a l'écran */
    #ifdef _SDL
        FlipBlitSurface(Character->Surface, &SpriteWindow, DDevice->Screen, &SpriteLayer, Flip); /* Curent Character on screen */
    #else
        SDL_RenderCopyEx(DDevice->Renderer, Character->Surface, &SpriteWindow, &SpriteLayer, 0, 0, Flip);
    #endif
    if (SDL_GetTicks() > Character->LastFrame + Character->Anim[Character->PlayingAnimation].Framerate){
        Character->LastFrame = SDL_GetTicks();
        Character->CurrentFrame++;
    }
}

void DisplayCharacterLayer(DisplayDevice* DDevice, CharacterLayer* CharaLayer){
    CharacterList* CharaList;

    CharaList = CharaLayer->CharaList;
    while (CharaList != NULL){
        DisplayCharacter(DDevice, CharaList->Character, *(CharaLayer->Viewport), CharaList->Coordinates, CharaList->Flip);
        CharaList = CharaList->NextCharacter;
    }
}