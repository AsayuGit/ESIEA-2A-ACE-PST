#ifndef _COURTRECCORD_H
#define _COURTRECCORD_H

    #include "include.h"

    typedef struct{
        Surface* ItemSpritesheet;
        SDL_Rect* ItemSrcRectArray;
        char** NameArray;
        char** OrignArray;
        char** DescriptionArray;
        int* TypeArray;
        int NbOfItems;
        int ItemInRow;
    }Items;


    void InitCourtReccord(DisplayDevice* DDevice, Items* ItemBankPointer);
    Items* LoadItemsFromFile(DisplayDevice* DDevice, char* filePath);
    void HandleCourtReccordEvents(SDL_Event* event);
    void DrawCourtReccord(DisplayDevice* DDevice, BitmapFont* Font);

#endif