#include "CourtReccord.h"
#include <libxml2/libxml/parser.h>

#define NBOFITEMS 8
#define NBOFBARS 2

// Linked List
typedef struct ItemList_t{
    int ItemID;
    struct ItemList_t* next;
}ItemList;

enum {
    MainCRMenu,
    DetailsMenu
}MenuID;

// Surfaces
Surface* CourtReccordSpriteSheet;
Surface* CourtDetailSpriteSheet;

// Rects
SDL_Rect CourtReccordBackground[2]; // [0] Src; [1] Dst
SDL_Rect CourtDetailBackground[2]; // [0] Src; [1] Dst
SDL_Rect SelectedSlotSrc;
SDL_Rect SelectedSlotPos[NBOFITEMS];
SDL_Rect DetailItemPos;
SDL_Rect Bars[NBOFBARS][2]; // [0] Src; [1] Dst
SDL_Rect Arrows[NBOFBARS][3]; // [0] Src; [1] Dst; [2] Dst2
SDL_Rect ItemName;
SDL_Rect DetailItemName;
SDL_Rect ItemDescription;
SDL_Rect ItemOrigin;

// Sound Effects
Mix_Chunk* MoveCursor;

// Fonts
BitmapFont* ItemNameFont;
BitmapFont* DetailsFont;

Items* ItemBank;
ItemList* StoredItemList;
ItemList** StoredItemListPointer;

char SelectedSlot;
char SelectedItem;
char MenuSelect;
char ItemDetails[50];

char* ItemTypes[2] = {
    "Other",
    "Reports"
};

void InitCourtDetails(DisplayDevice* DDevice){
    Uint32 ColorKey, FontColorKey;
    
    // Load textures
    ColorKey = 0x00ffff;
    FontColorKey = 0xff00ff;
    CourtDetailSpriteSheet = LoadSurface(ROOT""TEXTURES"Menus"SL"CourtDetails"TEX_EXT, DDevice, &ColorKey, false);

    // LoadFonts
    DetailsFont = LoadBitmapFont(ROOT""FONTS"DetailsFont"TEX_EXT, DDevice, FontColorKey);

    // SetRects
    CourtDetailBackground[0].x = 0; CourtDetailBackground[1].x = 0;
    CourtDetailBackground[0].y = 0; CourtDetailBackground[1].y = 32;
    CourtDetailBackground[0].w = CourtDetailBackground[1].w = 256;
    CourtDetailBackground[0].h = CourtDetailBackground[1].h = 128;

    DetailItemPos.x = 19;
    DetailItemPos.y = 38;
    DetailItemPos.w = 68;
    DetailItemPos.h = 68;

    DetailItemName.y = 43;
    DetailItemName.w = DDevice->ScreenResolution.x;
    DetailItemName.h = DDevice->ScreenResolution.y;

    ItemDescription.x = 28;
    ItemDescription.y = 117;
    ItemDescription.w = 238;
    ItemDescription.h = 46;

    ItemOrigin.x = 102;
    ItemOrigin.y = 61;
    ItemOrigin.w = 123;
    ItemOrigin.h = 39;

    Arrows[0][2] = Arrows[0][1];
    Arrows[1][2] = Arrows[1][1];

    Arrows[0][2].y = Arrows[1][2].y = 66;

    //ItemDetails = NULL;
}

// Init the court reccord menu for further use
void InitCourtReccord(DisplayDevice* DDevice, Items* ItemBankPointer){
    Uint32 ColorKey, FontColorKey;
    Vector2i SlotOrigin;
    int SlotOffset;
    int i;

    // Load textures
    ColorKey = 0x00ffff;
    CourtReccordSpriteSheet = LoadSurface(ROOT""TEXTURES"Menus"SL"CourtReccord"TEX_EXT, DDevice, &ColorKey, false);

    // Load Fonts
    FontColorKey = 0xff00ff;
    ItemNameFont = LoadBitmapFont(ROOT""FONTS"ItemNameFont"TEX_EXT, DDevice, FontColorKey);

    // Load Sound Effects
    MoveCursor = LoadSoundEffect(EffectPath[CHK_ButtonUpDown]);

    // Set rects
    CourtReccordBackground[0].x = 0; CourtReccordBackground[1].x = 24;
    CourtReccordBackground[0].y = 0; CourtReccordBackground[1].y = 36;
    CourtReccordBackground[0].w = CourtReccordBackground[1].w = 208;
    CourtReccordBackground[0].h = CourtReccordBackground[1].h = 124;

    Bars[0][0].x = 208;
    Bars[0][0].y = Bars[1][0].y = 0;
    Bars[0][0].w = Bars[1][0].w = Bars[0][1].w = Bars[1][1].w = 16;
    Bars[0][0].h = Bars[1][0].h = Bars[0][1].h = Bars[1][1].h = 96;
    Bars[1][0].x = Bars[0][0].x + Bars[0][0].w;

    Bars[0][1].x = 0;
    Bars[1][1].x = 240;
    Bars[0][1].y = Bars[1][1].y = 56;

    Arrows[0][0].x = Arrows[1][0].x = 240;
    Arrows[0][0].w = Arrows[1][0].w = Arrows[0][1].w = Arrows[1][1].w = 7;
    Arrows[0][0].h = Arrows[1][0].h = Arrows[0][0].y = Arrows[0][1].h = Arrows[1][1].h = 13;

    Arrows[0][1].x = Bars[1][1].x + 4;
    Arrows[1][1].x = Bars[0][1].x + 4;

    Arrows[0][1].y = Arrows[1][1].y = 40 + Bars[0][1].y;

    SelectedSlotSrc.x = 208;
    SelectedSlotSrc.y = 96;
    SelectedSlotSrc.w = SelectedSlotSrc.h = 44;

    SlotOrigin.x = CourtReccordBackground[1].x + 10;
    SlotOrigin.y = CourtReccordBackground[1].y + 26;
    SlotOffset = 4;
    for (i = 0; i < NBOFITEMS; i++){
        SelectedSlotPos[i].x = SlotOrigin.x + ((i % 4) * (SelectedSlotSrc.w + SlotOffset));
        SelectedSlotPos[i].y = SlotOrigin.y + ((i / 4) * (SelectedSlotSrc.h + SlotOffset));
        SelectedSlotPos[i].w = SelectedSlotSrc.w;
        SelectedSlotPos[i].h = SelectedSlotSrc.h;
    }

    ItemName.y = 40;
    ItemName.w = DDevice->ScreenResolution.x;
    ItemName.h = DDevice->ScreenResolution.y;
    
    MenuSelect = MainCRMenu;
    SelectedSlot = 0;
    SelectedItem = 0;
    StoredItemList = NULL;
    StoredItemListPointer = &StoredItemList;
    ItemBank = ItemBankPointer;

    InitCourtDetails(DDevice); // Once done with the court reccord init, we init the court details
}

void FreeCourtReccord(){
}

void AddItemToCourtReccord(int ItemID){
    ItemList** List;

    List = &StoredItemList;
    while (*List){ // while it isn't empty
        List = &((*List)->next);
    }
    // Once we find a free spot we append the new Item to the list
    (*List) = (ItemList*)malloc(sizeof(ItemList));
    (*List)->ItemID = ItemID;
    (*List)->next = NULL;
}

void RemoveItemFromCourtReccord(int ItemID){
    ItemList** List;
    ItemList* ItemNext;

    List = &StoredItemList;
    while (*List){ // while it isn't empty
        if ((*List)->ItemID == ItemID){
            ItemNext = (*List)->next;
            free(*List);
            (*List) = ItemNext;
            break;
        }
        List = &((*List)->next);
    }
}

void FreeItemList(ItemList** List){
    if (List){
        if(*List){
            FreeItemList(&((*List)->next));
            free(*List);
            (*List) = NULL;
        }
    }
}

void EmptyCourtReccord(){
    if (StoredItemList)
        FreeItemList(&StoredItemList);
}

Items* allocateItems(int nbOfItems, int ItemInRow){
    Items* loadedItem;
    int i;

    loadedItem = (Items*)calloc(1, sizeof(Items));    
    (loadedItem->NameArray) = (char**)malloc(nbOfItems*sizeof(char*));
    loadedItem->OrignArray = (char**)malloc(nbOfItems*sizeof(char*));
    loadedItem->DescriptionArray = (char**)malloc(nbOfItems*sizeof(char*));
    loadedItem->TypeArray = (int*)malloc(nbOfItems*sizeof(int));
    loadedItem->NbOfItems = nbOfItems;
    loadedItem->ItemInRow = ItemInRow;
    loadedItem->ItemSpritesheet = NULL;

    loadedItem->ItemSrcRectArray = (SDL_Rect*)malloc(nbOfItems*sizeof(SDL_Rect));
    for (i = 0; i < nbOfItems; i++){
        loadedItem->ItemSrcRectArray[i].w = loadedItem->ItemSrcRectArray[i].h = 70;
        loadedItem->ItemSrcRectArray[i].x = (i % ItemInRow) * loadedItem->ItemSrcRectArray[i].w;
        loadedItem->ItemSrcRectArray[i].y = (i / ItemInRow) * loadedItem->ItemSrcRectArray[i].h;
    }
    
    return loadedItem;
}

Items* LoadItemsFromFile(DisplayDevice* DDevice, char* filePath){
    // Declaration
    Items* loadedItem;
    xmlDoc* itemListFile;
    xmlNode *itemList, *property, *itemProperty;

    Uint32 ColorKey;
    int nbOfItems;
    int ItemInRow;
    int currentItem;
    char* SpriteSheetPath;
    char* buffer;

    // Logic
    loadedItem = NULL;
    xmlKeepBlanksDefault(0); // Ignore white space
    itemListFile = xmlReadFile(filePath, NULL, 0); // Load File into memory
    itemList = xmlDocGetRootElement(itemListFile); // get the first chidlren

    if (strcmp(itemList->name, "itemList") == 0){
        nbOfItems = xmlChildElementCount(itemList); // Get the number of items
        
        sscanf(xmlGetProp(itemList, "colorKey"), "%x", &ColorKey);
        sscanf(xmlGetProp(itemList, "ItemInRow"), "%d", &ItemInRow);
        buffer = xmlGetProp(itemList, "spriteSheet");

        //printf("Allocate %d Items\n", nbOfItems);
        loadedItem = allocateItems(nbOfItems, ItemInRow); // Allocate memory for the item bank
        loadedItem->ItemSpritesheet = LoadSurface(buffer, DDevice, &ColorKey, false);
        
        property = itemList->children;
        currentItem = 0;
        
        while (property){
            if (strcmp(property->name, "item") == 0){
                itemProperty = property->children;
                //printf("Current ITEM %d\n", currentItem);
                while (itemProperty){
                    
                    if (strcmp(itemProperty->name, "name") == 0) {

                        buffer = xmlNodeGetContent(itemProperty);
                        loadedItem->NameArray[currentItem] = (char*)malloc((strlen(buffer) + 1)*sizeof(char));
                        strcpy(loadedItem->NameArray[currentItem], buffer);
                    } else if (strcmp(itemProperty->name, "origin") == 0) {

                        buffer = xmlNodeGetContent(itemProperty);
                        loadedItem->OrignArray[currentItem] = (char*)malloc((strlen(buffer) + 1)*sizeof(char));
                        strcpy(loadedItem->OrignArray[currentItem], buffer);
                    } else if (strcmp(itemProperty->name, "description") == 0) {

                        buffer = xmlNodeGetContent(itemProperty);
                        loadedItem->DescriptionArray[currentItem] = (char*)malloc((strlen(buffer) + 1)*sizeof(char));
                        strcpy(loadedItem->DescriptionArray[currentItem], buffer);
                    } else if (strcmp(itemProperty->name, "type") == 0) {
                        loadedItem->TypeArray[currentItem] = atoi(xmlNodeGetContent(itemProperty));
                    }
                    
                    itemProperty = itemProperty->next;
                }
                currentItem++;
            }
            property = property->next;
        }
    }

    return loadedItem;
}

char GetSelectedItem(int Slot){
    int i;
    ItemList* SearchList;

    if (Slot >= 0){
        SearchList = *StoredItemListPointer;
        for (i = 0; SearchList && (i < Slot); i++){
            SearchList = SearchList->next;
        }
    }

    return (SearchList) ? SearchList->ItemID : -1;
}

void UpdateItemDetails(int ItemID){
    /*
    if (ItemDetails)
        free(ItemDetails);
    asprintf(&ItemDetails, "Type:%s\n%s", "Reports", ItemBank->OrignArray[ItemID]);*/
    sprintf(ItemDetails, "Type:%s\n%s", ItemTypes[ItemBank->TypeArray[ItemID]], ItemBank->OrignArray[ItemID]);
}

void HandleCourtReccordEvents(SDL_Event* event){
    int SlotSearch;

    switch (MenuSelect)
    {
    case MainCRMenu:
        switch (event->type)
        {
        case SDL_KEYDOWN:
            switch (P_PADKEY)
            {
            case PAD_UP:
            case PAD_DOWN:
                SelectedSlot = (SelectedSlot + 4) % 8;
                Mix_PlayChannel(-1, MoveCursor, 0);
                break;
            case PAD_LEFT:
                SelectedSlot = ((SelectedSlot + 3) % 4) + ((SelectedSlot / 4) * 4);
                Mix_PlayChannel(-1, MoveCursor, 0);
                break;
            case PAD_RIGHT:
                SelectedSlot = (SelectedSlot + 1) % 4 + ((SelectedSlot / 4) * 4);
                Mix_PlayChannel(-1, MoveCursor, 0);
                break;
            case PAD_SELECT:
                SelectedItem = GetSelectedItem(SelectedSlot);
                if (SelectedItem != -1) {
                    MenuSelect = DetailsMenu;
                    UpdateItemDetails(SelectedItem);
                    Mix_PlayChannel(-1, MoveCursor, 0);
                }
                break;
            }
            break;
        }
        break;
    case DetailsMenu:
        switch (event->type)
        {
        case SDL_KEYDOWN:
            switch (P_PADKEY)
            {
            case PAD_LEFT:
                SlotSearch = GetSelectedItem(SelectedSlot - 1);
                if (SlotSearch >= 0) {
                    SelectedItem = SlotSearch;
                    SelectedSlot--;
                    MenuSelect = DetailsMenu;
                    UpdateItemDetails(SelectedItem);
                    Mix_PlayChannel(-1, MoveCursor, 0);
                }
                break;

            case PAD_RIGHT:
                SlotSearch = GetSelectedItem(SelectedSlot + 1);
                if (SlotSearch >= 0) {
                    SelectedItem = SlotSearch;
                    SelectedSlot++;
                    MenuSelect = DetailsMenu;
                    UpdateItemDetails(SelectedItem);
                    Mix_PlayChannel(-1, MoveCursor, 0);
                }
                break;

            case PAD_BACK:
                MenuSelect = MainCRMenu;
                Mix_PlayChannel(-1, MoveCursor, 0);
                break;
            }
            break;
        }
        break;
    }
}

// Display the main court reccord menu on scren
void DrawMainCourtReccordMenu(DisplayDevice* DDevice, BitmapFont* Font){
    int i;
    int ArrowAnim;
    SDL_Rect RightArrowAfterAnim, LeftArrowAfterAnim;
    ItemList* StoredItemListIterator;

    // Arrow wiggle
    ArrowAnim = sin((double)SDL_GetTicks() / 50) * 2;
    
    RightArrowAfterAnim = Arrows[0][1];
    LeftArrowAfterAnim = Arrows[1][1];

    RightArrowAfterAnim.x += ArrowAnim;
    LeftArrowAfterAnim.x -= ArrowAnim;

    SDL_RenderCopy(DDevice->Renderer, CourtReccordSpriteSheet, CourtReccordBackground, CourtReccordBackground + 1); // Draw the background

    SDL_RenderCopy(DDevice->Renderer, CourtReccordSpriteSheet, &(Bars[0][0]), &(Bars[0][1])); // Draw the bars
    SDL_RenderCopy(DDevice->Renderer, CourtReccordSpriteSheet, &(Bars[1][0]), &(Bars[1][1]));

    SDL_RenderCopy(DDevice->Renderer, CourtReccordSpriteSheet, &(Arrows[0][0]), &(RightArrowAfterAnim)); // Draw the arrows
    SDL_RenderCopy(DDevice->Renderer, CourtReccordSpriteSheet, &(Arrows[1][0]), &(LeftArrowAfterAnim));
    
    i = 0;
    StoredItemListIterator = *StoredItemListPointer;
    while (StoredItemListIterator){
        if (i == SelectedSlot){
            ItemName.x = (DDevice->ScreenResolution.x - gstrlen(ItemNameFont, ItemBank->NameArray[StoredItemListIterator->ItemID])) / 2;
            gprintf(DDevice, ItemNameFont, ItemBank->NameArray[StoredItemListIterator->ItemID], &ItemName); // Draw the item's name
        }

        SDL_RenderCopy(DDevice->Renderer, ItemBank->ItemSpritesheet, &(ItemBank->ItemSrcRectArray[StoredItemListIterator->ItemID]), &(SelectedSlotPos[i])); // Draw the item
        StoredItemListIterator = StoredItemListIterator->next;
        i++;
    }

    SDL_RenderCopy(DDevice->Renderer, CourtReccordSpriteSheet, &SelectedSlotSrc, &(SelectedSlotPos[SelectedSlot])); // Draw the cursor
}

void DrawCourtDetails(DisplayDevice* DDevice, BitmapFont* Font, int ItemID){
    int ArrowAnim;
    SDL_Rect RightArrowAfterAnim, LeftArrowAfterAnim;

    // Arrow wiggle
    ArrowAnim = sin((double)SDL_GetTicks() / 50) * 2;
    
    RightArrowAfterAnim = Arrows[0][2];
    LeftArrowAfterAnim = Arrows[1][2];

    RightArrowAfterAnim.x += ArrowAnim;
    LeftArrowAfterAnim.x -= ArrowAnim;

    DetailItemName.x = ((144 - gstrlen(ItemNameFont, ItemBank->NameArray[ItemID])) / 2) + 91;

    SDL_RenderCopy(DDevice->Renderer, CourtDetailSpriteSheet, CourtDetailBackground, CourtDetailBackground + 1); // Draw the background
    SDL_RenderCopy(DDevice->Renderer, CourtReccordSpriteSheet, &(Arrows[0][0]), &(RightArrowAfterAnim)); // Draw the arrows
    SDL_RenderCopy(DDevice->Renderer, CourtReccordSpriteSheet, &(Arrows[1][0]), &(LeftArrowAfterAnim));

    gprintf(DDevice, ItemNameFont, ItemBank->NameArray[ItemID], &DetailItemName); // Draw the item's name
    gprintf(DDevice, DetailsFont, ItemDetails, &ItemOrigin); // Draw the item's origin
    gprintf(DDevice, Font, ItemBank->DescriptionArray[ItemID], &ItemDescription); // Draw the item's description

    SDL_RenderCopy(DDevice->Renderer, ItemBank->ItemSpritesheet, &(ItemBank->ItemSrcRectArray[ItemID]), &(DetailItemPos)); // Draw the item
}


void DrawCourtReccord(DisplayDevice* DDevice, BitmapFont* Font){
    switch (MenuSelect)
    {
    case MainCRMenu:
        DrawMainCourtReccordMenu(DDevice, Font); // Draw the main menu
        break;
    
    case DetailsMenu:
        DrawCourtDetails(DDevice, Font, SelectedItem); // Draw the CourtDetails menu
        break;
    }
}