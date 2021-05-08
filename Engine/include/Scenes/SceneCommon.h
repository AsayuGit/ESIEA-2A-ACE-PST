#ifndef _SCENECOMMON_H
#define _SCENECOMMON_H

    #include "include.h"
    #include "SceneCommonStructs.h"
    #include "Dialogue.h"
    #include "CharactersStructures.h"
    #include "Buttons.h"

    #include <libxml2/libxml/parser.h>

    /* Backgrounds functions */
    BackgroundContext* InitBackground(DisplayDevice* DDevice, char* ScenePath);
    void MoveBackground(BackgroundContext* Context, int TileID, char Effect);
    void BackgroundPlayAnimation(BackgroundContext* Context, int AnimationID, bool* AnimState);
    void DisplayBackground(DisplayDevice* DDevice, BackgroundContext* Context);

    /* Xml Funcitons */
    xmlNode* searchNodeLabel(xmlNode* entry, char* label);

    /* Scenes Functions */
    SceneContext* InitScene(DisplayDevice* DDevice, InputDevice* IDevice, DialogueContext* DiagContext, ButtonsContext* BContext, CourtroomContext* CContext, char* ScenePath);
    void parseScene(SceneContext* SContext);
    void SceneForward(SceneContext* SContext);
    void SceneBackward(SceneContext* SContext);
    void FreeBGAnimation(BGAnimation* AnimationToFree);
    void FreeBackground(BackgroundContext* SceneToFree);

#endif