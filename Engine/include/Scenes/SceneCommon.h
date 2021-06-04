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

#ifndef _SCENECOMMON_H
#define _SCENECOMMON_H

    #include "include.h"
    #include "SceneCommonStructs.h"
    #include "Dialogue.h"
    #include "CharactersStructures.h"
    #include "Buttons.h"
	
	#ifdef _XBOX
		#include <libxml/parser.h>
	#else
		#include <libxml2/libxml/parser.h>
	#endif

    /* Backgrounds functions */
    void FreeBackground(BackgroundContext* SceneToFree);
    BackgroundContext* InitBackground(DisplayDevice* DDevice, char* ScenePath);
    void MoveBackground(BackgroundContext* Context, int TileID, char Effect);
    void BackgroundPlayAnimation(BackgroundContext* Context, int AnimationID, bool* EndSignal, bool EndSignalValue);
    void DisplayBackground(DisplayDevice* DDevice, BackgroundContext* Context);

    /* Xml Funcitons */
    xmlNode* searchNodeLabel(xmlNode* entry, char* label);

    /* Scenes Functions */
    SceneContext* InitScene(DisplayDevice* DDevice, InputDevice* IDevice, DialogueContext* DiagContext, ButtonsContext* BContext, CourtroomContext* CContext, char* ScenePath);
    void parseScene(DisplayDevice* DDevice, SceneContext* SContext);
    void SceneForward(SceneContext* SContext);
    void SceneBackward(SceneContext* SContext);

#endif