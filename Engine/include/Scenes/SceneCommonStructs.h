/*
    Mia's Legacy is a Ace Attorney fangame taking place directly
    after the first game in the serie. All code in this repo have
    been written from scratch in ANSI C using SDL and libxml2.
    This game is designed to run on Linux Windows and the og Xbox

    Copyright (C) 2021 Killian RAIMBAUD (killian.rai@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef _SCENECOMMONSTRUCTS_H
#define _SCENECOMMONSTRUCTS_H

    #include "include.h"
    #include "Dialogue.h"
    #include "CharactersStructures.h"
    #include "ButtonsStructs.h"
    #include "Courtroom.h"

    typedef struct{
        unsigned int NbOfAnimStates;
        Vector2d* AnimStates;
        int* AnimRuntime;
        Vector2d* AnimRange;
        SDL_Rect AnimRegion;
    } BGAnimation;

    typedef struct{
        /* Background Texture properties */
        SDL_Texture* Surface;           /* Background Surface */
        int TileID;
        SDL_Rect SrcRect;
        
        /* Background Animation properties */
        int PlayingAnimation;
        Uint32 StartFrame;
        int CurrentState;
        int AnimOffset;
        unsigned int nbOfAnimations;
        BGAnimation* Animation;

        /* Scenes index */
        unsigned int nbOfCoordinates;
        Vector2i* ScenesCoordinates;

        /* Metadata */
        bool* EndSignal;                /* Reference */
        bool EndSignalValue;
        char Flipped;
        int ObjectLayerOffset;
        
        bool Shown;
    } BackgroundContext;

    typedef struct {
        /* System Devices */
        InputDevice* IDevice;           /* Ref */

        /* Loaded Scene */
        xmlNode* entry;
        xmlNode* next;
        xmlNode* press;
        xmlNode* presentMatch;
        xmlNode* presentDefault;
        xmlNode* returnTarget;
        int presentItem;
        bool Jump;
        bool DiagShown;
        bool activateCourtRecord;
        unsigned char diagMode;
        
        /* Dialogue */
        DialogueContext* DiagContext;   /* Ref */
        
        /* Buttons */
        ButtonsContext* BContext;       /* Ref */

        /* Backgrounds */
        BackgroundContext* BGContext;
        BackgroundContext* ScenePics;
        
        /* Characters */
        Characters** CharactersIndex;
        CharacterLayer* CharaLayer;

        /* Coms with Courtroom.c */
        CourtroomContext* CContext;     /* Ref */
    } SceneContext;

#endif