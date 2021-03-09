#ifndef _SCENECOMMON
#define _SCENECOMMON

    #include "include.h"
    #include "Dialogue.h"

    typedef enum{
        S_Empty,
        S_Courtroom,
        S_TitleScreen,
        BackgroundsCount
    } BackgroundIDS;

    typedef struct{
        int NbOfAnimStates;
        Vector2d* AnimStates;
        int* AnimRuntime;
        Vector2d* AnimRange;
        char* AnimEffects;
        SDL_Rect AnimRegion;
    } BGAnimation;

    typedef struct{
        const char* SurfacePath;// Background Surface path
        BGAnimation* Animation;
        unsigned int nbOfScenes;
        Vector2i* ScenesCoordinates;
    } Background;

    typedef struct{
        // Background Texture properties
        Surface* Surface; // Background Surface
        Vector2i SurfaceBounds;
        int TileID;
        SDL_Rect SrcRect;
        
        // Background Animation properties
        int PlayingAnimation;
        Uint32 StartFrame;
        int CurrentState;
        int AnimOffset;
        BGAnimation* Animation;

        // Scenes index
        unsigned int nbOfScenes;
        Vector2i* ScenesCoordinates;

        // Metadata
        char* AnimState;
        char Flipped;
        int ObjectLayerOffset;
    } SceneContext;

    SceneContext* InitScene(DisplayDevice* DDevice, int BackgroundID);
    void MoveTile(SceneContext* Context, int TileID, char Effect);
    void BackgroundPlayAnimation(SceneContext* Context, int AnimationID, char* AnimState);
    void DisplayBackground(DisplayDevice* DDevice, SceneContext* Context);

#endif