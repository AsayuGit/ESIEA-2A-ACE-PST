#ifndef _SCENECOMMONSTRUCTS
#define _SCENECOMMONSTRUCTS

    #include "include.h"

    typedef struct{
        int NbOfAnimStates;
        Vector2d* AnimStates;
        int* AnimRuntime;
        Vector2d* AnimRange;
        SDL_Rect AnimRegion;
    } BGAnimation;

    typedef struct{
        /* Background Texture properties */
        SDL_Texture* Surface; /* Background Surface */
        int TileID;
        SDL_Rect SrcRect;
        
        /* Background Animation properties */
        int PlayingAnimation;
        Uint32 StartFrame;
        int CurrentState;
        int AnimOffset;
        BGAnimation* Animation;

        /* Scenes index */
        Vector2i* ScenesCoordinates;

        /* Metadata */
        bool* AnimState;
        char Flipped;
        int ObjectLayerOffset;
    } SceneContext;

#endif