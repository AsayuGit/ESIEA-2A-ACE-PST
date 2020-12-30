#include "Characters.h"

Animation PhoenixAnimations[2] = {
    {
        {14, 24, 125, 142}, // SrcRect // Idle
        {0, 18, 125, 142}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {600, 24, 125, 142}, // SrcRect // Talk
        {0, 18, 125, 142}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    }
};

Animation MiaAnimations[2] = {
    {
        {54, 1045, 73, 169}, // SrcRect // Idle
        {87, 23, 73, 169}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {411, 1045, 73, 169}, // SrcRect // Talk
        {87, 23, 73, 169}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    }
};

Animation MilesAnimations[2] = {
    {
        {37, 35, 104, 143}, // SrcRect // Idle
        {136, 16, 104, 143}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {493, 35, 104, 143}, // SrcRect // Talk
        {136, 16, 104, 143}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    }
};

Animation DeskAnim = {
    {0, 0, 256, 44}, // SrcRect // Idle
    {0, 148, 256, 44}, // DstRect
    1,   // NbOfFrames
    0 // Framerate
};

// Characters constants
Characters Cast[CharactersCount] = {
    { // Phoenix Wright
        NULL,                                                   // Surface
        ROOT""TEXTURES"Characters"SL"Phoenix_Wright"TEX_EXT,    // SurfacePath
        0x00b5a5d5,                                             // ColorKey
        NULL,
        0,                                                      // LastFrame
        0,                                                      // CurrentFrame
        0,                                                      // PlayingAnimation
        (Animation*)&PhoenixAnimations                          // Anim
    },
    { // Mia Fey
        NULL,
        ROOT""TEXTURES"Characters"SL"Mia_Fey"TEX_EXT,
        0x00b5a5d5,
        NULL,
        0,
        0,
        0,
        (Animation*)&MiaAnimations
    },
    { // Miles Edgeworth
        NULL,
        ROOT""TEXTURES"Characters"SL"Miles_Edgeworth"TEX_EXT,
        0x00b5a5d5,
        NULL,
        0,
        0,
        0,
        (Animation*)&MilesAnimations
    },
    { // Courtroom Desk
        NULL,
        ROOT""TEXTURES"Places"SL"CourtroomDesk"TEX_EXT,
        0x00ff00ff,
        NULL,
        0,
        0,
        0,
        (Animation*)&DeskAnim
    }
};

void InitCharacter(DisplayDevice* DDevice, int CharacterID){
    Cast[CharacterID].Surface = LoadSurface(Cast[CharacterID].SurfacePath, DDevice, &Cast[CharacterID].ColorKey, false);
}

void CharacterPlayAnimation(int CharacterID, int AnimationID){
    Cast[CharacterID].PlayingAnimation = AnimationID;
    Cast[CharacterID].CurrentFrame = 0;
}

void InitCharacterLayer(CharacterLayer** CharaLayer, SceneContext* SContext){
    if ((*CharaLayer) == NULL){
        (*CharaLayer) = (CharacterLayer*)malloc(sizeof(CharacterLayer));
        (*CharaLayer)->CharaList = NULL;
        
        (*CharaLayer)->Viewport = &(SContext->SrcRect);
        (*CharaLayer)->Flipped = &(SContext->Flipped);
    }
}

void AddCharacterToLayer(CharacterLayer* CharaLayer, int CharacterID, int TileX, int TileY, char Flip, DisplayDevice* DDevice, Vector2i BackgroundBounds){ // Add a new character to a CharacterLayer
    CharacterList** CharaList;
    SDL_Rect CharaRect;

    if (!CharaLayer)
        return; // error
    CharaList = &(CharaLayer->CharaList);
    while ((*CharaList) != NULL){
        CharaList = &((*CharaList)->NextCharacter);
    }
    (*CharaList) = (CharacterList*)malloc(sizeof(CharacterList));
    (*CharaList)->CharacterID = CharacterID;

    CharaRect.w = DDevice->ScreenResolution.x;
    CharaRect.h = DDevice->ScreenResolution.y;
    (*CharaList)->Coordinates = RectTileToCorrdinate(CharaRect, BackgroundBounds, TileX, TileY);
    (*CharaList)->Flip = Flip;
    (*CharaList)->NextCharacter = NULL;
}

void DeleteCharacterFromLayer(CharacterLayer* CharaLayer, int CharacterID){ // Delete a character from a CharacterLayer
    CharacterList** CurrentCharacter;
    CharacterList* PrevLayer;
    CharacterList* NextLayer;

    if (!CharaLayer)
        return; // error
    CurrentCharacter = &(CharaLayer->CharaList);
    
    if ((*CurrentCharacter)->CharacterID == CharacterID){
        NextLayer = (*CurrentCharacter)->NextCharacter;
        free(*CurrentCharacter);
        (*CurrentCharacter) = NextLayer;
    }else{
        do {
            PrevLayer = (*CurrentCharacter);
            CurrentCharacter = &((*CurrentCharacter)->NextCharacter);
            if ((*CurrentCharacter)->CharacterID == CharacterID){
                NextLayer = (*CurrentCharacter)->NextCharacter;
                free(*CurrentCharacter);
                PrevLayer->NextCharacter = NextLayer;
                break;
            }
        } while ((*CurrentCharacter) != NULL);
    }
}

void DisplayCharacter(DisplayDevice* DDevice, int CharacterID, SDL_Rect Viewport, Vector2i Coordinates, char Flip){ // Display "A" Character on screen
    SDL_Rect SpriteWindow, SpriteLayer;

    // On veille a ne pas dépacer le nombre de frames de l'animation
    if (Cast[CharacterID].CurrentFrame >= Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].NbOfFrames){
        Cast[CharacterID].CurrentFrame = 0;
    }
    
    // On déplace la fenêtre dans la spritesheet en fonction du numéro de la frame
    SpriteWindow.x = Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].SrcRect.x + Cast[CharacterID].CurrentFrame * Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].SrcRect.w;
    SpriteWindow.y = Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].SrcRect.y;
    SpriteWindow.w = Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].SrcRect.w;
    SpriteWindow.h = Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].SrcRect.h;

    SpriteLayer = Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].DstRect;
    SpriteLayer.x += Coordinates.x - Viewport.x;
    SpriteLayer.y += Coordinates.y - Viewport.y;

    // On affiche la frame d'animation a l'écran
    #ifdef _SDL
        SDL_BlitSurface(Cast[CharacterID].Surface, &SpriteWindow, DDevice->Screen, &SpriteLayer); // Curent Character on screen
    #else
        SDL_RenderCopyEx(DDevice->Renderer, Cast[CharacterID].Surface, &SpriteWindow, &SpriteLayer, 0, 0, Flip);
    #endif
    if (SDL_GetTicks() > Cast[CharacterID].LastFrame + Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].Framerate){
        Cast[CharacterID].LastFrame = SDL_GetTicks();
        Cast[CharacterID].CurrentFrame++;
    }
}

void DisplayCharacterLayer(DisplayDevice* DDevice, CharacterLayer* CharaLayer){
    CharacterList* CharaList;

    CharaList = CharaLayer->CharaList;
    while (CharaList != NULL){
        DisplayCharacter(DDevice, CharaList->CharacterID, *(CharaLayer->Viewport), CharaList->Coordinates, CharaList->Flip);
        CharaList = CharaList->NextCharacter;
    }
}