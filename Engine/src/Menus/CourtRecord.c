#include "CourtRecord.h"
#include "UI.h"
#include <math.h>

#ifdef _XBOX
	#include <libxml/parser.h>
#else
	#include <libxml2/libxml/parser.h>
#endif

#define NBOFITEMS 8
#define NBOFBARS 2

/* Linked List */
typedef struct ItemList_t{
    int ItemID;
    struct ItemList_t* next;
}ItemList;

enum {
    MainCRMenu,
    DetailsMenu
};

/* Surfaces */
static SDL_Texture* CourtRecordSpriteSheet;
static SDL_Texture* CourtDetailSpriteSheet;

/* Rects */
static SDL_Rect CourtRecordBackground[2]; /* [0] Src; [1] Dst */
static SDL_Rect CourtDetailBackground[2]; /* [0] Src; [1] Dst */
static SDL_Rect SelectedSlotSrc;
static SDL_Rect SelectedSlotPos[NBOFITEMS];
static SDL_Rect Bars[NBOFBARS][2]; /* [0] Src; [1] Dst */
static SDL_Rect Arrows[NBOFBARS][3]; /* [0] Src; [1] Dst; [2] Dst2 */
static SDL_Rect ItemName;
static SDL_Rect DetailItemName;

static const SDL_Rect DetailItemPos = {19, 38, 68, 68};
static const SDL_Rect ItemDescription = {28, 117, 238, 46};
static const SDL_Rect ItemOrigin = {102, 61, 123, 39};

/* Sound Effects */
static Mix_Chunk* MoveCursor;

/* Fonts */
static BitmapFont* ItemNameFont;
static BitmapFont* DetailsFont;

static Items* ItemBank;
static ItemList* StoredItemList;
static ItemList** StoredItemListPointer;

static unsigned char SelectedSlot;
static unsigned char MenuSelect;
static char SelectedItem;

static char ItemDetails[50];

static char* ItemTypes[3] = {
    "Other",
    "Reports",
    "Weapons"
};

void InitCourtDetails(DisplayDevice* DDevice){
    /* Load textures */
    CourtDetailSpriteSheet = LoadSurface(ROOT""TEXTURES"Menus"SL"CourtDetails"TEX_EXT, DDevice, 0x00ffff, SURFACE_KEYED);

    /* LoadFonts */
    DetailsFont = LoadBitmapFont(ROOT""FONTS"DetailsFont"TEX_EXT, DDevice, 0xff00ff);

    /* SetRects */
    CourtDetailBackground[0].x = 0; CourtDetailBackground[1].x = 0;
    CourtDetailBackground[0].y = 0; CourtDetailBackground[1].y = 32;
    CourtDetailBackground[0].w = CourtDetailBackground[1].w = 256;
    CourtDetailBackground[0].h = CourtDetailBackground[1].h = 128;

    DetailItemName.y = 43;
    DetailItemName.w = BASE_RESOLUTION_X;
    DetailItemName.h = BASE_RESOLUTION_Y;

    Arrows[0][2] = Arrows[0][1];
    Arrows[1][2] = Arrows[1][1];

    Arrows[0][2].y = Arrows[1][2].y = 66;
}

/* Init the court Record menu for further use */
void InitCourtRecord(DisplayDevice* DDevice, Items* ItemBankPointer){
    Vector2i SlotOrigin;
    int SlotOffset;
    int i;

    /* Load textures */
    CourtRecordSpriteSheet = LoadSurface(ROOT""TEXTURES"Menus"SL"CourtRecord"TEX_EXT, DDevice, 0x00ffff, SURFACE_KEYED);

    /* Load Fonts */
    ItemNameFont = LoadBitmapFont(ROOT""FONTS"ItemNameFont"TEX_EXT, DDevice, 0xff00ff);

    /* Load Sound Effects */
    MoveCursor = LoadSoundEffect(EffectPath[CHK_ButtonUpDown]);

    /* Set rects */
    CourtRecordBackground[0].x = 0; CourtRecordBackground[1].x = 24;
    CourtRecordBackground[0].y = 0; CourtRecordBackground[1].y = 36;
    CourtRecordBackground[0].w = CourtRecordBackground[1].w = 208;
    CourtRecordBackground[0].h = CourtRecordBackground[1].h = 124;

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

    SlotOrigin.x = CourtRecordBackground[1].x + 10;
    SlotOrigin.y = CourtRecordBackground[1].y + 26;
    SlotOffset = 4;
    for (i = 0; i < NBOFITEMS; i++){
        SelectedSlotPos[i].x = SlotOrigin.x + ((i % 4) * (SelectedSlotSrc.w + SlotOffset));
        SelectedSlotPos[i].y = SlotOrigin.y + ((i / 4) * (SelectedSlotSrc.h + SlotOffset));
        SelectedSlotPos[i].w = SelectedSlotSrc.w;
        SelectedSlotPos[i].h = SelectedSlotSrc.h;
    }

    ItemName = InitRect(0, 40, BASE_RESOLUTION_X, BASE_RESOLUTION_Y);
    
    MenuSelect = MainCRMenu;
    SelectedSlot = 0;
    SelectedItem = 0;
    StoredItemList = NULL;
    StoredItemListPointer = &StoredItemList;
    ItemBank = ItemBankPointer;

    InitCourtDetails(DDevice); /* Once done with the court Record init, we init the court details */
}

void FreeCourtRecord(){
    /* FIXME: We should really Implement this now !*/
}

void AddItemToCourtRecord(int ItemID){
    ItemList** List;

    List = &StoredItemList;
    while (*List){ /* while it isn't empty */
        if ((*List)->ItemID == ItemID) /* Check if the Item is already in the court record */
            return;
        List = &((*List)->next);
    }
    /* Once we find a free spot we append the new Item to the list */
    (*List) = (ItemList*)malloc(sizeof(ItemList));
    (*List)->ItemID = ItemID;
    (*List)->next = NULL;
}

void RemoveItemFromCourtRecord(int ItemID){
    ItemList** List;
    ItemList* ItemNext;

    List = &StoredItemList;
    while (*List){ /* while it isn't empty */
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

void EmptyCourtRecord(){
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

Items* LoadItemsFromFile(DisplayDevice* DDevice, char* filePath){ /* TIPS : Maybe we should move that to Load.h */
    /* Declaration */
    Items* loadedItem;
    xmlDoc* itemListFile;
    xmlNode *itemList, *property, *itemProperty;

    Uint32 ColorKey;
    int nbOfItems;
    int ItemInRow;
    int currentItem;
    char* buffer;

    /* Logic */
    loadedItem = NULL;
    xmlKeepBlanksDefault(0); /* Ignore white space */

#ifdef _XBOX
	itemListFile = xmlParseFile(filePath); /* Load File into memory */
#else
    itemListFile = xmlReadFile(filePath, NULL, 0); /* Load File into memory */
#endif

    itemList = xmlDocGetRootElement(itemListFile); /* get the first chidlren */

    if (strcmp((char*)itemList->name, "itemList") == 0){
        nbOfItems = xmlChildElementCount(itemList); /* Get the number of items */
        
        sscanf((char*)xmlGetProp(itemList, (xmlChar*)"colorKey"), "%x", &ColorKey);
        sscanf((char*)xmlGetProp(itemList, (xmlChar*)"ItemInRow"), "%d", &ItemInRow);
        buffer = (char*)xmlGetProp(itemList, (xmlChar*)"spriteSheet");

        /*printf("Allocate %d Items\n", nbOfItems); */
        loadedItem = allocateItems(nbOfItems, ItemInRow); /* Allocate memory for the item bank */
        loadedItem->ItemSpritesheet = LoadSurface(buffer, DDevice, ColorKey, SURFACE_KEYED);
        
        property = itemList->children;
        currentItem = 0;
        
        while (property){
            if (strcmp((char*)property->name, "item") == 0){
                itemProperty = property->children;
                /*printf("Current ITEM %d\n", currentItem); */
                while (itemProperty){
                    
                    if (strcmp((char*)itemProperty->name, "name") == 0) {

                        buffer = (char*)xmlNodeGetContent(itemProperty);
                        loadedItem->NameArray[currentItem] = (char*)malloc((strlen(buffer) + 1)*sizeof(char));
                        strcpy(loadedItem->NameArray[currentItem], buffer);
                    } else if (strcmp((char*)itemProperty->name, "origin") == 0) {

                        buffer = (char*)xmlNodeGetContent(itemProperty);
                        loadedItem->OrignArray[currentItem] = (char*)malloc((strlen(buffer) + 1)*sizeof(char));
                        strcpy(loadedItem->OrignArray[currentItem], buffer);
                    } else if (strcmp((char*)itemProperty->name, "description") == 0) {

                        buffer = (char*)xmlNodeGetContent(itemProperty);
                        loadedItem->DescriptionArray[currentItem] = (char*)malloc((strlen(buffer) + 1)*sizeof(char));
                        strcpy(loadedItem->DescriptionArray[currentItem], buffer);
                    } else if (strcmp((char*)itemProperty->name, "type") == 0) {
                        loadedItem->TypeArray[currentItem] = atoi((char*)xmlNodeGetContent(itemProperty));
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
    SearchList = NULL;
    if (Slot >= 0){
        SearchList = *StoredItemListPointer;
        for (i = 0; SearchList && (i < Slot); i++){
            SearchList = SearchList->next;
        }
    }
    return (SearchList) ? SearchList->ItemID : -1;
}

void UpdateItemDetails(int ItemID){
    sprintf(ItemDetails, "Type:%s\n%s", ItemTypes[ItemBank->TypeArray[ItemID]], ItemBank->OrignArray[ItemID]);
}

void HandleCourtRecordEvents(SDL_Event* event, SceneContext* SContext){
    int SlotSearch;

    switch (MenuSelect)
    {
    case MainCRMenu:
        switch (event->type)
        {
        case PAD_KEYDOWN:
            switch (event->PADKEY)
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
                SelectedSlot = ((SelectedSlot + 1) % 4) + ((SelectedSlot / 4) * 4);
                Mix_PlayChannel(-1, MoveCursor, 0);
                break;
            case PAD_SELECT:

                SelectedItem = GetSelectedItem(SelectedSlot);
                if (SelectedItem >= 0) {
                    MenuSelect = DetailsMenu;
                    UpdateItemDetails(SelectedItem);
                    Mix_PlayChannel(-1, MoveCursor, 0);
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    case DetailsMenu:
        switch (event->type)
        {
        case PAD_KEYDOWN:
            switch (event->PADKEY)
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
            case PAD_PRESS:
                if (SContext->presentDefault){
                    SContext->CContext->ShowCourtRecord = false;
                    SContext->CContext->EventSelect = 0;
                    SContext->CContext->diagRewind = false;
                    if (SContext->presentItem == SelectedItem){
                        setUI(OBJECTION, 0);
                        if (SContext->presentMatch)
                            SContext->entry = SContext->presentMatch;
                    } else {
                        setUI(OBJECTION, 1);
                        if (SContext->presentDefault)
                            SContext->entry = SContext->presentDefault;
                    }
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    }
}

/* Display the main court Record menu on scren */
void DrawMainCourtRecordMenu(DisplayDevice* DDevice, BitmapFont* Font){
    int i;
    int ArrowAnim;
    SDL_Rect RightArrowAfterAnim, LeftArrowAfterAnim;
    ItemList* StoredItemListIterator;

    /* Arrow wiggle */
    ArrowAnim = sin((double)SDL_GetTicks() / 50) * 2;
    
    RightArrowAfterAnim = Arrows[0][1];
    LeftArrowAfterAnim = Arrows[1][1];

    RightArrowAfterAnim.x += ArrowAnim;
    LeftArrowAfterAnim.x -= ArrowAnim;

    ScaledDraw(DDevice, CourtRecordSpriteSheet, CourtRecordBackground, CourtRecordBackground + 1);  /* Draw the background */

    ScaledDraw(DDevice, CourtRecordSpriteSheet, &(Bars[0][0]), &(Bars[0][1]));                      /* Draw the bars */
    ScaledDraw(DDevice, CourtRecordSpriteSheet, &(Bars[1][0]), &(Bars[1][1]));

    ScaledDraw(DDevice, CourtRecordSpriteSheet, &(Arrows[0][0]), &(RightArrowAfterAnim));           /* Draw the arrows */
    ScaledDraw(DDevice, CourtRecordSpriteSheet, &(Arrows[1][0]), &(LeftArrowAfterAnim));

    i = 0;
    StoredItemListIterator = *StoredItemListPointer;
    while (StoredItemListIterator){
        if (i == SelectedSlot){
            ItemName.x = ((BASE_RESOLUTION_X - gstrlen(ItemNameFont, ItemBank->NameArray[StoredItemListIterator->ItemID], 1).x) >> 1);
            gprintf(DDevice, ItemNameFont, ItemBank->NameArray[StoredItemListIterator->ItemID], 1, &ItemName); /* Draw the item's name */
        }

        ScaledDraw(DDevice, ItemBank->ItemSpritesheet, &(ItemBank->ItemSrcRectArray[StoredItemListIterator->ItemID]), &(SelectedSlotPos[i])); /* Draw the item */
        
        StoredItemListIterator = StoredItemListIterator->next;
        i++;
    }

    ScaledDraw(DDevice, CourtRecordSpriteSheet, &SelectedSlotSrc, &(SelectedSlotPos[SelectedSlot])); /* Draw the cursor */
}

void DrawCourtDetails(DisplayDevice* DDevice, BitmapFont* Font, int ItemID){
    int ArrowAnim;
    SDL_Rect RightArrowAfterAnim, LeftArrowAfterAnim;

    /* Arrow wiggle */
    ArrowAnim = sin((double)SDL_GetTicks() / 50) * 2;
    
    RightArrowAfterAnim = Arrows[0][2];
    LeftArrowAfterAnim = Arrows[1][2];

    RightArrowAfterAnim.x += ArrowAnim;
    LeftArrowAfterAnim.x -= ArrowAnim;

    DetailItemName.x = ((144 - gstrlen(ItemNameFont, ItemBank->NameArray[ItemID], 1).x) >> 1) + 91;

    ScaledDraw(DDevice, CourtDetailSpriteSheet, CourtDetailBackground, CourtDetailBackground + 1);              /* Draw the background */
    ScaledDraw(DDevice, CourtRecordSpriteSheet, &(Arrows[0][0]), &(RightArrowAfterAnim));                       /* Draw the arrows */
    ScaledDraw(DDevice, CourtRecordSpriteSheet, &(Arrows[1][0]), &(LeftArrowAfterAnim));
    ScaledDraw(DDevice, ItemBank->ItemSpritesheet, &(ItemBank->ItemSrcRectArray[ItemID]), &(DetailItemPos));    /* Draw the item */

    gprintf(DDevice, ItemNameFont, ItemBank->NameArray[ItemID], 1, &DetailItemName);                            /* Draw the item's name */
    gprintf(DDevice, DetailsFont, ItemDetails, 1, &ItemOrigin);                                                 /* Draw the item's origin */
    gprintf(DDevice, Font, ItemBank->DescriptionArray[ItemID], 1, &ItemDescription);                            /* Draw the item's description */
}


void DrawCourtRecord(DisplayDevice* DDevice, BitmapFont* Font){
    switch (MenuSelect)
    {
    case MainCRMenu:
        DrawMainCourtRecordMenu(DDevice, Font); /* Draw the main menu */
        break;
    
    case DetailsMenu:
        DrawCourtDetails(DDevice, Font, SelectedItem); /* Draw the CourtDetails menu */
        break;
    }
}