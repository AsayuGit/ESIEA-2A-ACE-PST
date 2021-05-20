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