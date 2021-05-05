#ifndef _SCENECOMMONSTRUCTS_H
#define _SCENECOMMONSTRUCTS_H

    #include "include.h"
    #include "Dialogue.h"
    #include "CharactersStructures.h"
    #include "ButtonsStructs.h"
    #include "Courtroom.h"

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
        
        bool Shown;
    } BackgroundContext;

    typedef struct {
        /* System Devices */
        InputDevice* IDevice;           /* Ref */

        /* Loaded Scene */
        xmlNode* entry;
        xmlNode* press;
        bool Jump;
        bool DiagShown;
        
        /* Dialogue */
        DialogueContext* DiagContext;   /* Ref */
        
        /* Buttons */
        ButtonsContext* BContext;       /* Ref */

        /* Backgrounds */
        BackgroundContext* BGContext;
        BackgroundContext* ScenePics;
        
        /* Characters */
        Characters** CharactersIndex;   /* Ref */

        /* Coms with Courtroom.c */
        CourtroomContext* CContext;     /* Ref */
    } SceneContext;

#endif