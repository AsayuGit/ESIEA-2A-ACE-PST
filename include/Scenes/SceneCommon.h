#ifndef _SCENECOMMON
#define _SCENECOMMON

    #include "include.h"
    #include "Dialogue.h"

    typedef enum{
        S_Courtroom,
        BackgroundsCount
    } BackgroundIDS;

    typedef struct{
        int NbOfAnimStates;
        Vector2d* AnimStates;
        int* AnimRuntime;
        Vector2d* AnimRange;
        SDL_Rect AnimRegion;
    } BGAnimation;

    typedef struct{
        const char* SurfacePath;// Background Surface path
        BGAnimation* Animation;
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

        // Metadata
        int ObjectLayerOffset;
    } SceneContext;

    SceneContext* InitScene(DisplayDevice* DDevice, int BackgroundID);
    void MoveTile(SceneContext* Context, int TileID);
    void BackgroundPlayAnimation(SceneContext* Context, int AnimationID);
    void DisplayBackground(DisplayDevice* DDevice, SceneContext* Context);

#endif