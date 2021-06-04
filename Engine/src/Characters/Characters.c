/*
    Mia's Legacy is a Ace Attorney fangame taking place directly
    after the first game in the serie. All code in this repo have
    been written from scratch in ANSI C using SDL and libxml2.
    This game is designed to run on Linux Windows and the og Xbox

    Copyright (C) 2021 Killian RAIMBAUD (killian.rai@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "Characters.h"

Animation* ParseCHAnimation(xmlNode* array){
    Animation* LoadingAnimation;
    xmlNode *entry;
    unsigned int ArrayID;

    LoadingAnimation = (Animation*)malloc(xmlChildElementCount(array)*sizeof(Animation));
    array = array->children;

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

void AddCharacterToLayer(CharacterLayer* CharaLayer, Characters* Character, BackgroundContext* BGContext, int TileID, bool Flip){  /* Add a new character to a CharacterLayer */
    CharacterList** CharaList;

    if (!CharaLayer)
        return; /* error */
    CharaList = &(CharaLayer->CharaList);
    
    while ((*CharaList) != NULL){
        CharaList = &((*CharaList)->NextCharacter);
    }
    (*CharaList) = (CharacterList*)malloc(sizeof(CharacterList));
    (*CharaList)->Character = Character;
    (*CharaList)->NextCharacter = NULL;
    (*CharaList)->Coordinates = BGContext->ScenesCoordinates[TileID];
    (*CharaList)->Flip = Flip;
    (*CharaList)->Shown = true;
}

void MoveCharacterOnLayer(CharacterLayer* CharaLayer, BackgroundContext* BGContext, const unsigned int charaInLayerID, const int TileID, const bool Flip){
    CharacterList** CharaList;
    unsigned int i = 0;

    if (!CharaLayer)
        return; /* error */
    CharaList = &(CharaLayer->CharaList);
    
    while ((*CharaList) != NULL){
        if (i == charaInLayerID)
            break;
        CharaList = &((*CharaList)->NextCharacter);
        i++;
    }
    if (*CharaList){
        (*CharaList)->Coordinates = BGContext->ScenesCoordinates[TileID];
        (*CharaList)->Flip = Flip;
    }
}

void removeCharacterFromLayer(CharacterLayer* CharaLayer, const unsigned int charaInLayerID){
    CharacterList** CharaList;
    CharacterList* NextCharacter;
    unsigned int i = 0;

    if (!CharaLayer)
        return; /* error */
    CharaList = &(CharaLayer->CharaList);
    
    while ((*CharaList) != NULL){
        if (i == charaInLayerID)
            break;
        CharaList = &((*CharaList)->NextCharacter);
        i++;
    }
    if (*CharaList){
        NextCharacter = (*CharaList)->NextCharacter;
        free(*CharaList);
        (*CharaList) = NextCharacter;
    }
}

void setCharacterVisibility(CharacterLayer* CharaLayer, const unsigned int charaInLayerID, bool Shown){
    CharacterList** CharaList;
    unsigned int i = 0;

    if (!CharaLayer)
        return; /* error */
    CharaList = &(CharaLayer->CharaList);
    
    while ((*CharaList) != NULL){
        if (i == charaInLayerID)
            break;
        CharaList = &((*CharaList)->NextCharacter);
        i++;
    }
    if (*CharaList){
        (*CharaList)->Shown = Shown;
    }
}

void DisplayCharacter(DisplayDevice* DDevice, Characters* Character, SDL_Rect Viewport, Vector2i Coordinates, char Flip){ /* Display "A" Character on screen  */
    SDL_Rect SpriteWindow, SpriteLayer;

    /* On veille a ne pas dépacer le nombre de frames de l'animation */
    if (Character->CurrentFrame >= Character->Anim[Character->PlayingAnimation].NbOfFrames){
        Character->CurrentFrame = 0;
    }
    
    /* On déplace la fenêtre dans la spritesheet en fonction du numéro de la frame */
    SpriteWindow = Character->Anim[Character->PlayingAnimation].SrcRect;
    SpriteWindow.x = Character->Anim[Character->PlayingAnimation].SrcRect.x + Character->CurrentFrame * Character->Anim[Character->PlayingAnimation].SrcRect.w;

    SpriteLayer = Character->Anim[Character->PlayingAnimation].DstRect;
    SpriteLayer.x = Character->Anim[Character->PlayingAnimation].DstRect.x  + Coordinates.x - Viewport.x,
    SpriteLayer.y = Character->Anim[Character->PlayingAnimation].DstRect.y + Coordinates.y - Viewport.y,

    ScaledDrawEx(DDevice, Character->Surface, &SpriteWindow, &SpriteLayer, Flip);
    
    if (SDL_GetTicks() > Character->LastFrame + Character->Anim[Character->PlayingAnimation].Framerate){
        Character->LastFrame = SDL_GetTicks();
        Character->CurrentFrame++;
    }
}

void DisplayCharacterLayer(DisplayDevice* DDevice, CharacterLayer* CharaLayer){
    CharacterList* CharaList;

    CharaList = CharaLayer->CharaList;
    while (CharaList != NULL){
        if (CharaList->Shown)
            DisplayCharacter(DDevice, CharaList->Character, *(CharaLayer->Viewport), CharaList->Coordinates, CharaList->Flip);
        CharaList = CharaList->NextCharacter;
    }
}