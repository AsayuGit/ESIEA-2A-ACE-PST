#ifndef _CHARACTERS
#define _CHARACTERS

    #include "include.h"
    #include "SceneCommon.h"

    typedef enum{
        Phoenix_Wright,
        Mia_Fey,
        Miles_Edgeworth,
        Judge,
        Winston_Payne,
        Court_Desk,
        CharactersCount
    } Cid;

    typedef struct{
        const SDL_Rect SrcRect; // Character source rect
        SDL_Rect DstRect; // Character destination
        int NbOfFrames;   // Number of frames in an animation
        int Framerate;  // Number of milliseconds between two frames
    } Animation;

    typedef struct{
        char* DisplayName;
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

    typedef struct CharacterList_s{
        Characters* Character;
        Vector2i Coordinates;
        char Flip;
        struct CharacterList_s* NextCharacter;
    } CharacterList;

    typedef struct {
        CharacterList * CharaList;
        SDL_Rect* Viewport;
        char* Flipped;
    } CharacterLayer; // "Context"

    void InitCharacter(DisplayDevice* DisplayDevice, Characters* Character);
    void CharacterPlayAnimation(Characters* Character, int AnimationID);
    //char* GetCharacterName(Characters* Character);

    // Character Layer Management
    void InitCharacterLayer(CharacterLayer** CharaLayer, SceneContext* SContext);
    void AddCharacterToLayer(CharacterLayer* CharaLayer, Characters* Character, SceneContext* SContext, int TileID, char Flip, DisplayDevice* DDevice, Vector2i BackgroundBounds);
    void DeleteCharacterFromLayer(CharacterLayer* CharaLayer, Characters* Character);
    void DisplayCharacterLayer(DisplayDevice* DDevice, CharacterLayer* CharaLayer);

#endif