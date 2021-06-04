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

#ifndef _DIALOGUE_H
#define _DIALOGUE_H

    #include "include.h"

    typedef struct{
        /* Meta */
        bool shown;
        int progress;
        char* Text;
        int TextSpeed;
        Uint32 LastLetter;

        /* Dialog Box */
        SDL_Texture* DialogBox;
        SDL_Rect DialogBoxSrcBounds;
        SDL_Rect DialogBoxBounds;   /* Region where the dialog box should be drawn */
        SDL_Rect TextBounds;        /* Region where the text should be drawn */
        SDL_Rect NameBounds;        /* Region where the name should be drawn */
        SDL_Rect ArrowDstRect;

        /* Font */
        BitmapFont* MainFont;
        BitmapFont* NameFont;
        SDL_Rect DstLetter;
        Uint8 textColor;
        SDL_Texture* textLayer;
        SDL_Texture* nameLayer;

        char letterLag;
        Mix_Chunk* Letter;
        Mix_Chunk* NextLine;
        Mix_Chunk* LineComplete;

        /* Display */
        DisplayDevice* DDevice;
    } DialogueContext;

    void ClearDialogueText(DialogueContext* Context);
    size_t SetDialogueText(DialogueContext* Context, char* Name, char* Text, char SndEffType);
    DialogueContext* InitDialog(DisplayDevice* DDevice, BitmapFont* MainFont, BitmapFont* NameFont);
    void Dialogue(DialogueContext* Context, unsigned char mode);

#endif