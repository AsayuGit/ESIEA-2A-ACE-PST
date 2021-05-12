#ifndef _UI_H
#define _UI_H

    #include "include.h"
    #include "CourtRecord.h"
    #include "SceneCommon.h"

    enum {
        UI_EMPTY = -1,
        UI_LIVES,
        TESTIMONY_ICON,
        CROSS_EXAMINATION,
        SHOW_ITEM_TO_COURTL,
        SHOW_ITEM_TO_COURTR,
        HOLD_IT,
        OBJECTION,
        TAKE_THAT
    };

    void InitUI(DisplayDevice* DDevice, Items* UIItemBankPointer, SceneContext* SContext);
    void setUI(unsigned int notifType, unsigned int argument);
    void DrawUI(DisplayDevice* DDevice, InputDevice* IDevice);

#endif