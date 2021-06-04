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

#ifndef _CHARACTERS_H
#define _CHARACTERS_H

    #include "include.h"
    #include "CharactersStructures.h"
    #include "SceneCommon.h"

    Characters* InitCharacter(DisplayDevice* DDevice, char* CharacterPath);
    void CharacterPlayAnimation(Characters* Character, int AnimationID);
    /* char* GetCharacterName(Characters* Character);  */

    /* Character Layer Management */
    void InitCharacterLayer(CharacterLayer** CharaLayer, BackgroundContext* SContext);
    void AddCharacterToLayer(CharacterLayer* CharaLayer, Characters* Character, BackgroundContext* BGContext, int TileID, bool Flip);
    void MoveCharacterOnLayer(CharacterLayer* CharaLayer, BackgroundContext* BGContext, const unsigned int charaInLayerID, const int TileID, const bool Flip);
    void removeCharacterFromLayer(CharacterLayer* CharaLayer, const unsigned int charaInLayerID);
    void setCharacterVisibility(CharacterLayer* CharaLayer, const unsigned int charaInLayerID, bool Shown);
    void DisplayCharacterLayer(DisplayDevice* DDevice, CharacterLayer* CharaLayer);

#endif