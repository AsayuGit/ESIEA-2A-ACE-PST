#ifndef _COURTRECORD_H
#define _COURTRECORD_H

    #include "include.h"
    #include "SceneCommonStructs.h"

    typedef struct{
        SDL_Texture* ItemSpritesheet;
        SDL_Rect* ItemSrcRectArray;
        char** NameArray;
        char** OrignArray;
        char** DescriptionArray;
        int* TypeArray;
        int NbOfItems;
        int ItemInRow;
    }Items;


    void InitCourtRecord(DisplayDevice* DDevice, Items* ItemBankPointer);      /* Initialise the court reccord */

    Items* LoadItemsFromFile(DisplayDevice* DDevice, char* filePath);          /* Loads the ItemBank from a file */
    void SetItemBank(Items* ItemBankPointer);                                  /* Sets the ItemBank to use */
    Items* GetItemBank();                                                      /* Return the ItemBank in use */
    void HandleCourtRecordEvents(SDL_Event* event, SceneContext* SContext); /* Handles the court record related events */
    void DrawCourtRecord(DisplayDevice* DDevice, BitmapFont* Font);            /* Draw the court record */
    void AddItemToCourtRecord(int ItemID);                                     /* Adds an Item to the court record */
    void RemoveItemFromCourtRecord(int ItemID);                                /* Remove an Item from the court record */
    void EmptyCourtRecord();                                                   /* Empty out the court record */

#endif