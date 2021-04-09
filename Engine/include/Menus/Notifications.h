#ifndef _NOTIFICATIONS_H
#define _NOTIFICATIONS_H

    #include "include.h"
    #include "CourtRecord.h"

    enum {
        SHOW_ITEM_TO_COURT
    };

    void InitNotifications(DisplayDevice* DDevice, Items* ItemBankPointer);

    /* Add a notification */
    void SummonNotification(unsigned int notifType, unsigned int ItemID);

    /* Remove any notification */
    void ClearNotifications();

    void DrawNotifications(DisplayDevice* DDevice);

#endif