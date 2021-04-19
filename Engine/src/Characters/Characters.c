#include "Characters.h"
#include "CHAR_Index.h"

void InitCharacter(DisplayDevice* DDevice, Characters* Character){
    Character->Surface = LoadSurface(Character->SurfacePath, DDevice, Character->ColorKey, SURFACE_KEYED);
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
        /*SDL_BlitSurface(Character->Surface, &SpriteWindow, DDevice->Screen, &SpriteLayer);*/
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