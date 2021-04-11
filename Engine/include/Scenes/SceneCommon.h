#ifndef _SCENECOMMON
#define _SCENECOMMON

    #include "include.h"
    #include "SceneCommonStructs.h"
    #include "Dialogue.h"
    #include "CharactersStructures.h"
    #include "Buttons.h"
    #include <libxml2/libxml/parser.h>

    SceneContext* InitScene(DisplayDevice* DDevice, int BackgroundID);
    void MoveTile(SceneContext* Context, int TileID, char Effect);
    void BackgroundPlayAnimation(SceneContext* Context, int AnimationID, bool* AnimState);
    void DisplayBackground(DisplayDevice* DDevice, SceneContext* Context);
    xmlDoc* loadScene(char* filePath);
    xmlNode* searchSceneNode(xmlNode** entry, char* label);
    void parseScene(xmlNode** entry, InputDevice* IDevice, DialogueContext* DiagContext, SceneContext* SContext, ButtonsContext* BContext, Characters** CharactersIndex, int NbOfCharacters, int* IdleAnimation, int* ReturnToDefault, int* CurrentCharacter, char* ButtonActivated, char** ButtonJumpLabels);

#endif