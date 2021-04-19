#ifndef _CHARACTERS
#define _CHARACTERS

    #include "include.h"
    #include "CharactersStructures.h"
    #include "SceneCommon.h"

    void InitCharacter(DisplayDevice* DisplayDevice, Characters* Character);
    void CharacterPlayAnimation(Characters* Character, int AnimationID);
    /* char* GetCharacterName(Characters* Character);  */

    /* Character Layer Management */
    void InitCharacterLayer(CharacterLayer** CharaLayer, SceneContext* SContext);
    void AddCharacterToLayer(CharacterLayer* CharaLayer, Characters* Character, SceneContext* SContext, int TileID, char Flip, DisplayDevice* DDevice);
    void DeleteCharacterFromLayer(CharacterLayer* CharaLayer, Characters* Character);
    void DisplayCharacterLayer(DisplayDevice* DDevice, CharacterLayer* CharaLayer);

#endif