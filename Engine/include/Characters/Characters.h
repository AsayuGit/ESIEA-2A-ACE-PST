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
    void DeleteCharacterFromLayer(CharacterLayer* CharaLayer, Characters* Character);
    void DisplayCharacterLayer(DisplayDevice* DDevice, CharacterLayer* CharaLayer);

#endif