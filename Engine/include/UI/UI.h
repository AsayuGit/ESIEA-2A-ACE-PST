#ifndef _UI_H
#define _UI_H

    #include "include.h"
    #include "CourtRecord.h"

    enum {
        UI_EMPTY,
        TESTIMONY_ICON,
        CROSS_EXAMINATION,
        SHOW_ITEM_TO_COURT
    };

    void InitUI(DisplayDevice* DDevice, Items* ItemBankPointer);
    void setUI(unsigned int notifType, unsigned int argument);
    void DrawUI(DisplayDevice* DDevice, InputDevice* IDevice);

#endif