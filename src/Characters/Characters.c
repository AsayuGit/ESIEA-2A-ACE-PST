#include "Characters.h"

Animation PhoenixAnimations[3] = {
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
    },
    {
        {14, 185, 107, 136}, // SrcRect // Glup
        {12, 24, 107, 136}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    }
};

Animation MiaAnimations[8] = {
    {
        {54, 1045, 73, 169}, // SrcRect // Idle [0]
        {87, 23, 73, 169}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {411, 1045, 73, 169}, // SrcRect // Talk [1]
        {87, 23, 73, 169}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    },
    {
        {56, 828, 92, 166}, // SrcRect // Forward Idle [2]
        {76, 26, 92, 166}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {514, 827, 92, 166}, // SrcRect // Forward Talk [3]
        {76, 26, 92, 166}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    },
    {
        {53, 1465, 73, 169}, // SrcRect // Sad Idle [4]
        {83, 23, 73, 169}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {450, 1465, 73, 169}, // SrcRect // Sad Talk [5]
        {83, 23, 73, 169}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    },
    {
        {61, 1257, 73, 170}, // SrcRect // Shocked Idle [6]
        {81, 22, 73, 170}, // DstRect
        4,   // NbOfFrames
        500 // Framerate
    },
    {
        {419, 1257, 73, 170}, // SrcRect // Shocked Talk [7]
        {81, 22, 73, 170}, // DstRect
        3,   // NbOfFrames
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

Animation JudgeAnimations[4] = {
    {
        {8, 11, 118, 96}, // SrcRect // Idle [0]
        {69, 35, 118, 96}, // DstRect
        5,   // NbOfFrames
        500 // Framerate
    },
    {
        {131, 136, 118, 96}, // SrcRect // Talk [1]
        {69, 35, 118, 96}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    },
    {
        {12, 706, 118, 96}, // SrcRect // Serious Idle [2]
        {69, 36, 118, 96}, // DstRect
        5,   // NbOfFrames
        500 // Framerate
    },
    {
        {134, 834, 118, 96}, // SrcRect // Serious Talk [3]
        {69, 36, 118, 96}, // DstRect
        4,   // NbOfFrames
        125 // Framerate
    }
};

Animation PayneAnimations[2] = {
    {
        {17, 26, 85, 134}, // SrcRect // Idle
        {158, 26, 85, 134}, // DstRect
        1,   // NbOfFrames
        500 // Framerate
    },
    {
        {111, 26, 85, 134}, // SrcRect // Talk
        {158, 26, 85, 134}, // DstRect
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
        "Phoenix",
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
        "Mia",
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
        "Edgeworth",
        NULL,
        ROOT""TEXTURES"Characters"SL"Miles_Edgeworth"TEX_EXT,
        0x00b5a5d5,
        NULL,
        0,
        0,
        0,
        (Animation*)&MilesAnimations
    },
    { // Judge
        "Judge",
        NULL,
        ROOT""TEXTURES"Characters"SL"Judge"TEX_EXT,
        0x00b5a5d5,
        NULL,
        0,
        0,
        0,
        (Animation*)&JudgeAnimations
    },
    { // Winston Payne
        "Payne",
        NULL,
        ROOT""TEXTURES"Characters"SL"Winston_Payne"TEX_EXT,
        0x00b5a5d5,
        NULL,
        0,
        0,
        0,
        (Animation*)&PayneAnimations
    },
    { // Courtroom Desk
        NULL,
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

char* GetCharacterName(Cid CharacterID){
    return Cast[CharacterID].DisplayName;
}

void InitCharacterLayer(CharacterLayer** CharaLayer, SceneContext* SContext){
    if ((*CharaLayer) == NULL){
        (*CharaLayer) = (CharacterLayer*)malloc(sizeof(CharacterLayer));
        (*CharaLayer)->CharaList = NULL;
        
        (*CharaLayer)->Viewport = &(SContext->SrcRect);
        (*CharaLayer)->Flipped = &(SContext->Flipped);
    }
}

void AddCharacterToLayer(CharacterLayer* CharaLayer, int CharacterID, SceneContext* SContext, int TileID, char Flip, DisplayDevice* DDevice, Vector2i BackgroundBounds){ // Add a new character to a CharacterLayer
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
    (*CharaList)->Coordinates.x = SContext->ScenesCoordinates[TileID].x;
    (*CharaList)->Coordinates.y = SContext->ScenesCoordinates[TileID].y;
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