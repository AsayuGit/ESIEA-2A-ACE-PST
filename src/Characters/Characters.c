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
    }
};

void InitCharacter(DisplayDevice* DDevice, int CharacterID){
    Cast[CharacterID].Surface = LoadSurface(Cast[CharacterID].SurfacePath, DDevice, &Cast[CharacterID].ColorKey, false);
}

void TieCharacterToBackground(int CharacterID, int* BackgroundOffset){
    Cast[CharacterID].BackgroundOffset = BackgroundOffset;
}

void CharacterPlayAnimation(int CharacterID, int AnimationID){
    Cast[CharacterID].PlayingAnimation = AnimationID;
    Cast[CharacterID].CurrentFrame = 0;
}

void DisplayCharacter(DisplayDevice* DDevice, int CharacterID){
    SDL_Rect SpriteWindow, SpriteLayer;
    // On veille a ne pas dépacer le nombre de frames de l'animation
    if (Cast[CharacterID].CurrentFrame >= Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].NbOfFrames){
        Cast[CharacterID].CurrentFrame = 0;
    }
    
    //printf("Frame N°%d\n", *TEMP);
    // On déplace la fenêtre dans la spritesheet en fonction du numéro de la frame
    SpriteWindow.x = Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].SrcRect.x + Cast[CharacterID].CurrentFrame * Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].SrcRect.w;
    SpriteWindow.y = Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].SrcRect.y;
    SpriteWindow.w = Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].SrcRect.w;
    SpriteWindow.h = Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].SrcRect.h;

    SpriteLayer = Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].DstRect;
    if (Cast[CharacterID].BackgroundOffset != NULL){
        SpriteLayer.x -= *(Cast[CharacterID].BackgroundOffset);
    }

    // On affiche la frame d'animation a l'écran
    #ifdef _SDL
        SDL_BlitSurface(Cast[CharacterID].Surface, &SpriteWindow, DDevice->Screen, &SpriteLayer); // Curent Character on screen
    #else
        SDL_RenderCopy(DDevice->Renderer, Cast[CharacterID].Surface, &SpriteWindow, &SpriteLayer);
    #endif
    if (SDL_GetTicks() > Cast[CharacterID].LastFrame + Cast[CharacterID].Anim[Cast[CharacterID].PlayingAnimation].Framerate){
        Cast[CharacterID].LastFrame = SDL_GetTicks();
        Cast[CharacterID].CurrentFrame++;
    }
};