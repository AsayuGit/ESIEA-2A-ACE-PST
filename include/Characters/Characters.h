#ifndef _CHARACTERS
#define _CHARACTERS

    #include "include.h"

    typedef enum{
        Phoenix_Wright,
        Mia_Fey,
        CharactersCount
    } Cid;

    typedef struct{
        const SDL_Rect SrcRect; // Character source rect
        SDL_Rect DstRect; // Character destination
        int NbOfFrames;   // Number of frames in an animation
        int Framerate;  // Number of milliseconds between two frames
    } Animation;

    typedef struct{
        // Character Texture properties
        Surface* Surface; // Character Surface
        char *SurfacePath;// Character Surface path 
        Uint32 ColorKey;  // Character color key

        // Background Interaction properties
        int* BackgroundOffset;

        // Character Animation properties
        Uint32 LastFrame; // Time at the last frame display
        int CurrentFrame; // Frame currently being displayed
        int PlayingAnimation; // The Animation currently playing
        Animation* Anim; // Contains all the differents animations for a character
    } Characters; // Sprite ?

    //extern Characters Cast[CharactersCount];

    void InitCharacter(DisplayDevice* DisplayDevice, int CharacterID);
    void TieCharacterToBackground(int CharacterID, int* BackgroundOffset);
    void CharacterPlayAnimation(int CharacterID, int AnimationID);
    void DisplayCharacter(DisplayDevice* DDevice, int CharacterID);

#endif