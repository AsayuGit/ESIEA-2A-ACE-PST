#ifndef _SCENECOMMONSTRUCTS
#define _SCENECOMMONSTRUCTS

    #include "include.h"

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
        const char* SurfacePath; /* Background Surface path */
        BGAnimation* Animation;
        unsigned int nbOfScenes;
        Vector2i* ScenesCoordinates;
    } Background;

    typedef struct{
        /* Background Texture properties */
        SDL_Texture* Surface; /* Background Surface */
        Vector2i SurfaceBounds;
        int TileID;
        SDL_Rect SrcRect;
        
        /* Background Animation properties */
        int PlayingAnimation;
        Uint32 StartFrame;
        int CurrentState;
        int AnimOffset;
        BGAnimation* Animation;

        /* Scenes index */
        unsigned int nbOfScenes;
        Vector2i* ScenesCoordinates;

        /* Metadata */
        bool* AnimState;
        char Flipped;
        int ObjectLayerOffset;
    } SceneContext;

#endif