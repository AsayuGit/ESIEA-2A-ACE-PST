#include "CourtReccord.h"
#include <libxml2/libxml/parser.h>

#define NBOFITEMS 8
#define NBOFBARS 2

// Linked List
typedef struct ItemList_t{
    int ItemID;
    struct ItemList_t* next;
}ItemList;

// Surfaces
Surface* CourtReccordSpriteSheet;

// Rects
SDL_Rect CourtReccordBackground[2]; // [0] Src; [1] Dst
SDL_Rect SelectedSlotSrc;
SDL_Rect SelectedSlotPos[NBOFITEMS];
SDL_Rect Bars[NBOFBARS][2]; // [0] Src; [1] Dst
SDL_Rect Arrows[NBOFBARS][2];
SDL_Rect ItemName;

// Sound Effects
Mix_Chunk* MoveCursor;

Items* ItemBank;
ItemList* StoredItemList;

char SelectedItem;

// Init the court reccord menu for further use
void InitCourtReccord(DisplayDevice* DDevice, Items* ItemBankPointer){
    Uint32 ColorKey;
    Vector2i SlotOrigin;
    int SlotOffset;
    int i;

    // Load textures
    ColorKey = 0x00ffff;
    CourtReccordSpriteSheet = LoadSurface(ROOT""TEXTURES"Menus"SL"CourtReccord"TEX_EXT, DDevice, &ColorKey, false);

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
     
    SelectedItem = 0;
    StoredItemList = NULL;
    ItemBank = ItemBankPointer;
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

void HandleCourtReccordEvents(SDL_Event* event){
    switch (event->type)
    {
    case SDL_KEYDOWN:
        switch (P_PADKEY)
        {
        case PAD_UP:
        case PAD_DOWN:
            SelectedItem = (SelectedItem + 4) % 8;
            Mix_PlayChannel(-1, MoveCursor, 0);
            break;
        case PAD_LEFT:
            SelectedItem = ((SelectedItem + 3) % 4) + ((SelectedItem / 4) * 4);
            Mix_PlayChannel(-1, MoveCursor, 0);
            break;
        case PAD_RIGHT:
            SelectedItem = (SelectedItem + 1) % 4 + ((SelectedItem / 4) * 4);
            Mix_PlayChannel(-1, MoveCursor, 0);
            break;
        case PAD_SELECT:
            //ButtonObject->clkdButton = ButtonObject->selButtonID;
            //Mix_PlayChannel(-1, ButtonObject->ClickedSndEffect[ButtonObject->clkdButton], 0);
            break;
        }
        break;
    default:
        break;
    }
}

// Display the court reccord menu on scren
void DrawCourtReccord(DisplayDevice* DDevice, BitmapFont* Font){
    int i;
    int ArrowAnim;
    SDL_Rect RightArrowAfterAnim, LeftArrowAfterAnim;
    ItemList* StoredItemListPointer;

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
    StoredItemListPointer = StoredItemList;
    while (StoredItemListPointer){
        if (i == SelectedItem){
            ItemName.x = (DDevice->ScreenResolution.x - gstrlen(Font, ItemBank->NameArray[StoredItemListPointer->ItemID])) / 2;
            gprintf(DDevice, Font, ItemBank->NameArray[StoredItemListPointer->ItemID], &ItemName);
        }

        SDL_RenderCopy(DDevice->Renderer, ItemBank->ItemSpritesheet, &(ItemBank->ItemSrcRectArray[StoredItemListPointer->ItemID]), &(SelectedSlotPos[i]));
        StoredItemListPointer = StoredItemListPointer->next;
        i++;
    }
    

    SDL_RenderCopy(DDevice->Renderer, CourtReccordSpriteSheet, &SelectedSlotSrc, &(SelectedSlotPos[SelectedItem])); // Draw the cursor
}