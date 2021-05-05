#ifndef _UI_H
#define _UI_H

    #include "include.h"
    #include "CourtRecord.h"
    #include "SceneCommon.h"

    enum {
        UI_EMPTY,
        TESTIMONY_ICON,
        CROSS_EXAMINATION,
        SHOW_ITEM_TO_COURT,
        HOLD_IT
    };

    void InitUI(DisplayDevice* DDevice, Items* UIItemBankPointer, SceneContext* SContext);
    void setUI(unsigned int notifType, unsigned int argument);
    void DrawUI(DisplayDevice* DDevice, InputDevice* IDevice);

#endif