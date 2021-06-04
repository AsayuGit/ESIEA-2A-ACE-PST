/*
    Mia's Legacy is a Ace Attorney fangame taking place directly
    after the first game in the serie. All code in this repo have
    been written from scratch in ANSI C using SDL and libxml2.
    This game is designed to run on Linux Windows and the og Xbox

    Copyright (C) 2021 Killian RAIMBAUD (killian.rai@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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
    PressUI,
    MainCRMenu,
    DetailsMenu
};

/* Viewports */
static SDL_Rect* CRViewport;                                /* Background Layer Viewport */
static const Vector2i CRPosition = {BASE_RESOLUTION_X, 0};  /* CourtRecord position relative to the BGL Viewport */

/* Layers */
static BackgroundContext* courtRecordLayer;

/* Surfaces */
static SDL_Texture* CourtRecordSpriteSheet;
static SDL_Texture* CourtDetailSpriteSheet;

/* Rects */
static SDL_Rect CourtRecordBackground[2]; /* [0] Src; [1] Dst */
static SDL_Rect CourtDetailBackground[2]; /* [0] Src; [1] Dst */
static SDL_Rect SelectedSlotSrc;
static SDL_Rect SelectedSlotPos[NBOFITEMS];
static SDL_Rect SrcBars;
static SDL_Rect DstBars[2]; /* [0] Left; [1] Right */
static SDL_Rect SrcArrows;
static SDL_Rect DstArrows[NBOFBARS][2]; /* [0] Left; [1] Right || [0] CR; [1] CD*/
static SDL_Rect ItemName;
static SDL_Rect DetailItemName;

static const SDL_Rect Hide[2] = {{44, 140, 80, 16}, {24, 160, 80, 16}};
static const SDL_Rect DetailItemPos = {19, 38, 68, 68};
static const SDL_Rect ItemDescription = {28, 117, 238, 46};
static const SDL_Rect ItemOrigin = {102, 61, 123, 39};

/* Sound Effects */
static Mix_Chunk *MoveCursor = NULL, *Back = NULL, *Open = NULL, *Click = NULL; 

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

static bool courtRecordAnimationOver = false;
static Uint8 controlsMode;

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

    DstArrows[0][1] = DstArrows[0][0];
    DstArrows[1][1] = DstArrows[1][0];
    DstArrows[1][1].y = DstArrows[0][1].y = CourtDetailBackground[1].y + 34;

    DetailItemName.y = 43;
    DetailItemName.w = BASE_RESOLUTION_X;
    DetailItemName.h = BASE_RESOLUTION_Y;
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
    Back = LoadSoundEffect(EffectPath[CHK_ButtonBack]);
    Open = LoadSoundEffect(EffectPath[CHK_CourtRecordOpen]);
    Click = LoadSoundEffect(EffectPath[CHK_EvidenceClick]);

    /* Set rects */
    CourtRecordBackground[0].x = 0; CourtRecordBackground[1].x = 24;
    CourtRecordBackground[0].y = 0; CourtRecordBackground[1].y = 36;
    CourtRecordBackground[0].w = CourtRecordBackground[1].w = 208;
    CourtRecordBackground[0].h = CourtRecordBackground[1].h = 124;

    SrcBars.x = 208;
    SrcBars.y = 0;
    DstBars[0].x = 0; DstBars[1].x = 240;
    DstBars[0].y = DstBars[1].y = 56;
    SrcBars.w = DstBars[0].w = DstBars[1].w = 16;
    SrcBars.h = DstBars[0].h = DstBars[1].h = 96;

    SrcArrows = InitRect(209, 111, 7, 13);
    DstArrows[0][0] = InitRect(4, DstBars[0].y + (DstBars[0].h >> 1) - (13/2), 7, 13);
    DstArrows[1][0] = InitRect(DstBars[1].x + 5, DstBars[1].y + (DstBars[1].h >> 1) - (13/2), 7, 13);

    SelectedSlotSrc.x = 0;
    SelectedSlotSrc.y = 124;
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
    
    MenuSelect = PressUI;
    SelectedSlot = 0;
    SelectedItem = 0;
    StoredItemList = NULL;
    StoredItemListPointer = &StoredItemList;
    ItemBank = ItemBankPointer;

    courtRecordLayer = InitBackground(DDevice, ROOT"Assets"SL"Anim"SL"CourtRecord.xml");
    CRViewport = &(courtRecordLayer->SrcRect); /* FIXME */
    controlsMode = 0;

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
            if (!event->key.repeat){
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
                        Mix_PlayChannel(-1, Click, 0);
                    }
                    break;
                case PAD_BACK:
                case PAD_COURTRECORD:
                    /* Dissable the court reccord */
                    BackgroundPlayAnimation(courtRecordLayer, 1, &courtRecordAnimationOver, false);
                    SContext->IDevice->EventEnabled = true;
                    SContext->CContext->EventSelect = (SContext->CContext->ButtonActivated) ? 1 : 0;
                    Mix_PlayChannel(-1, Back, 0);
                    break;
                
                case PAD_PRESS:
                    if (SContext->presentDefault){
                        courtRecordAnimationOver = false;
                        controlsMode = 0;
                        SContext->CContext->EventSelect = 0;
                        SContext->diagMode = 1;
                        if (SContext->presentItem == GetSelectedItem(SelectedSlot)){
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
            if (!event->key.repeat){
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
                    Mix_PlayChannel(-1, Back, 0);
                    break;
                case PAD_PRESS:
                    if (SContext->presentDefault){
                        courtRecordAnimationOver = false;
                        controlsMode = 0;
                        SContext->CContext->EventSelect = 0;
                        SContext->diagMode = 1;
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
                case PAD_COURTRECORD:
                    /* Dissable the court reccord */
                    BackgroundPlayAnimation(courtRecordLayer, 1, &courtRecordAnimationOver, false);
                    SContext->IDevice->EventEnabled = true;
                    SContext->CContext->EventSelect = (SContext->CContext->ButtonActivated) ? 1 : 0;
                    Mix_PlayChannel(-1, Back, 0);
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
        break;
    }
}

void DrawCourtRecordPressUI(DisplayDevice* DDevice){
    const SDL_Rect Press[2] = {{44, 124, 80, 16}, {0, 0, 80, 16}};
    const SDL_Rect Present[2] = {{124, 124, 80, 16}, {176, 0, 80, 16}};
    
    if (controlsMode & 1)
        ScaledDraw(DDevice, CourtRecordSpriteSheet, Press, Press + 1);     /* Draw the press button */
    if (controlsMode & 2)
        ScaledDraw(DDevice, CourtRecordSpriteSheet, Present, Present + 1);   /* Draw the present button */
}

/* Display the main court Record menu on scren */
void DrawMainCourtRecordMenu(DisplayDevice* DDevice, BitmapFont* Font){
    int i;
    int ArrowAnim;
    SDL_Rect RightArrowAfterAnim, LeftArrowAfterAnim;
    SDL_Rect CRDstRect;
    ItemList* StoredItemListIterator;
    const SDL_Rect Present[2] = {{124, 140, 80, 16}, {88, 0, 80, 16}};

    /* Arrow wiggle */
    ArrowAnim = sin((double)SDL_GetTicks() / 50) * 2;
    
    LeftArrowAfterAnim = DstArrows[0][0];
    RightArrowAfterAnim = DstArrows[1][0];

    RightArrowAfterAnim.x += ArrowAnim;
    LeftArrowAfterAnim.x -= ArrowAnim;

    CRDstRect = RectToVieport(CourtRecordBackground + 1, CRViewport, &CRPosition);
    ScaledDraw(DDevice, CourtRecordSpriteSheet, CourtRecordBackground, &CRDstRect); /* Draw the background */

    CRDstRect = RectToVieport(&(DstBars[0]), CRViewport, &CRPosition);
    ScaledDraw(DDevice, CourtRecordSpriteSheet, &(SrcBars), &CRDstRect);            /* Draw the Left Bar */
    CRDstRect = RectToVieport(&(DstBars[1]), CRViewport, &CRPosition);
    ScaledDrawEx(DDevice, CourtRecordSpriteSheet, &(SrcBars), &CRDstRect, true);    /* Draw the Right Bar */

    CRDstRect = RectToVieport(&(LeftArrowAfterAnim), CRViewport, &CRPosition);
    ScaledDrawEx(DDevice, CourtRecordSpriteSheet, &(SrcArrows), &CRDstRect, true);  /* Draw the Left arrow */
    CRDstRect = RectToVieport(&(RightArrowAfterAnim), CRViewport, &CRPosition);
    ScaledDraw(DDevice, CourtRecordSpriteSheet, &(SrcArrows), &CRDstRect);          /* Draw the Right arrow */

    i = 0;
    StoredItemListIterator = *StoredItemListPointer;
    while (StoredItemListIterator){
        if (i == SelectedSlot){
            ItemName.x = ((BASE_RESOLUTION_X - gstrlen(ItemNameFont, ItemBank->NameArray[StoredItemListIterator->ItemID], 1).x) >> 1);
            CRDstRect = RectToVieport(&ItemName, CRViewport, &CRPosition);
            gprintf(DDevice, ItemNameFont, ItemBank->NameArray[StoredItemListIterator->ItemID], 1, &CRDstRect); /* Draw the item's name */
        }

        CRDstRect = RectToVieport(&(SelectedSlotPos[i]), CRViewport, &CRPosition);
        ScaledDraw(DDevice, ItemBank->ItemSpritesheet, &(ItemBank->ItemSrcRectArray[StoredItemListIterator->ItemID]), &CRDstRect); /* Draw the item */
        
        StoredItemListIterator = StoredItemListIterator->next;
        i++;
    }

    CRDstRect = RectToVieport(&(SelectedSlotPos[SelectedSlot]), CRViewport, &CRPosition);
    ScaledDraw(DDevice, CourtRecordSpriteSheet, &SelectedSlotSrc, &CRDstRect); /* Draw the cursor */

    CRDstRect = RectToVieport(Hide + 1, CRViewport, &CRPosition);
    ScaledDraw(DDevice, CourtRecordSpriteSheet, Hide, &CRDstRect);   /* Draw the hide button */

    if (controlsMode & 2)
        ScaledDraw(DDevice, CourtRecordSpriteSheet, Present, Present + 1);   /* Draw the present button */
}

void DrawCourtDetails(DisplayDevice* DDevice, BitmapFont* Font, int ItemID){
    int ArrowAnim;
    SDL_Rect RightArrowAfterAnim, LeftArrowAfterAnim;
    SDL_Rect CRDstRect;
    const SDL_Rect Present[2] = {{124, 140, 80, 16}, {88, 0, 80, 16}};

    /* Arrow wiggle */
    ArrowAnim = sin((double)SDL_GetTicks() / 50) * 2;
    
    LeftArrowAfterAnim = DstArrows[0][1];
    RightArrowAfterAnim = DstArrows[1][1];

    RightArrowAfterAnim.x += ArrowAnim;
    LeftArrowAfterAnim.x -= ArrowAnim;

    DetailItemName.x = ((144 - gstrlen(ItemNameFont, ItemBank->NameArray[ItemID], 1).x) >> 1) + 91;

    CRDstRect = RectToVieport(CourtDetailBackground + 1, CRViewport, &CRPosition);
    ScaledDraw(DDevice, CourtDetailSpriteSheet, CourtDetailBackground, &CRDstRect);                     /* Draw the background */

    CRDstRect = RectToVieport(&(LeftArrowAfterAnim), CRViewport, &CRPosition);
    ScaledDrawEx(DDevice, CourtRecordSpriteSheet, &(SrcArrows), &CRDstRect, true);                      /* Draw the Left arrow */
    CRDstRect = RectToVieport(&(RightArrowAfterAnim), CRViewport, &CRPosition);
    ScaledDraw(DDevice, CourtRecordSpriteSheet, &(SrcArrows), &CRDstRect);                              /* Draw the Right arrow */
    
    CRDstRect = RectToVieport(&(DetailItemPos), CRViewport, &CRPosition);
    ScaledDraw(DDevice, ItemBank->ItemSpritesheet, &(ItemBank->ItemSrcRectArray[ItemID]), &CRDstRect);  /* Draw the item */

    CRDstRect = RectToVieport(&DetailItemName, CRViewport, &CRPosition);
    gprintf(DDevice, ItemNameFont, ItemBank->NameArray[ItemID], 1, &CRDstRect);                         /* Draw the item's name */
    CRDstRect = RectToVieport(&ItemOrigin, CRViewport, &CRPosition);
    gprintf(DDevice, DetailsFont, ItemDetails, 1, &CRDstRect);                                          /* Draw the item's origin */
    CRDstRect = RectToVieport(&ItemDescription, CRViewport, &CRPosition);
    gprintf(DDevice, Font, ItemBank->DescriptionArray[ItemID], 1, &CRDstRect);                          /* Draw the item's description */

    CRDstRect = RectToVieport(Hide + 1, CRViewport, &CRPosition);
    ScaledDraw(DDevice, CourtRecordSpriteSheet, Hide, &CRDstRect);   /* Draw the hide button */

    if (controlsMode & 2)
        ScaledDraw(DDevice, CourtRecordSpriteSheet, Present, Present + 1);   /* Draw the present button */
}


void DrawCourtRecord(DisplayDevice* DDevice, BitmapFont* Font){
    DisplayBackground(DDevice, courtRecordLayer);
    if (courtRecordAnimationOver){
        switch (MenuSelect)
        {
        case PressUI:
            DrawCourtRecordPressUI(DDevice);
            break;
        case MainCRMenu:
            DrawMainCourtRecordMenu(DDevice, Font); /* Draw the main menu */
            break;
        
        case DetailsMenu:
            DrawCourtDetails(DDevice, Font, SelectedItem); /* Draw the CourtDetails menu */
            break;

        default:
            break;
        }
    } else {
        courtRecordAnimationOver = true;
        MenuSelect = PressUI;
    }
}

void ShowCourtRecord(InputDevice* IDevice){
    courtRecordAnimationOver = true;
    MenuSelect = MainCRMenu;
    BackgroundPlayAnimation(courtRecordLayer, 0, &IDevice->EventEnabled, true);
    Mix_PlayChannel(-1, Open, 0);
}

void ShowCourtRecordXUI(Uint8 newControlsMode){
    controlsMode = newControlsMode;
    courtRecordAnimationOver = true;
    MenuSelect = PressUI;
}

void ResetCourtRecordPos(){
    MoveBackground(courtRecordLayer, 0, 0);
}