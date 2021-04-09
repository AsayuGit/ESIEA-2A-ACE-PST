#include "Notifications.h"

static Items* NotifyItemBank;
static unsigned int StoredItemID;
static SDL_Rect ItemPos;
static void (*notification)(DisplayDevice*);

void Notif_ShowToCourt(DisplayDevice* DDevice){
    SDL_RenderCopy(DDevice->Renderer, NotifyItemBank->ItemSpritesheet, &(NotifyItemBank->ItemSrcRectArray[StoredItemID]), &(ItemPos)); /* Draw the item */
}

/* Init the notification handler */
void InitNotifications(DisplayDevice* DDevice, Items* NotifyItemBankPointer){
    NotifyItemBank = NotifyItemBankPointer;
    notification = NULL;
    StoredItemID = 0;

    ItemPos.x = 13;
    ItemPos.y = 13;
    ItemPos.w = 70;
    ItemPos.h = 70;
}

/* Add a notification */
void SummonNotification(unsigned int notifType, unsigned int ItemID){
    switch (notifType){
        case SHOW_ITEM_TO_COURT:
            notification = &Notif_ShowToCourt;
            StoredItemID = ItemID;
            break;
        default:
            break;
    }
}

/* Remove any notification */
void ClearNotifications(){
    notification = NULL;
}

/* Draw the current notification on screen */
void DrawNotifications(DisplayDevice* DDevice){
    if (notification)
        notification(DDevice);
}