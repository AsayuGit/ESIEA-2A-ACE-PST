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
    void ShowCourtRecord(InputDevice* IDevice);
    void ShowCourtRecordXUI(Uint8 newControlsMode);
    void ResetCourtRecordPos();

#endif