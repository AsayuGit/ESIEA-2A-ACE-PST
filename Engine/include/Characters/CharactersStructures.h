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

#ifndef _CHARACTERSSTRUCTURES_H
#define _CHARACTERSSTRUCTURES_H

    #include "include.h"

    typedef enum{
        Phoenix_Wright,
        Mia_Fey,
        Miles_Edgeworth,
        Judge,
        Winston_Payne,
        Court_Desk,
        FrankShawit,
        Defendant_Desk,
        CharactersCount
    } Cid;

    typedef struct{
        SDL_Rect SrcRect; /* Character source rect */
        SDL_Rect DstRect;       /* Character destination */
        int NbOfFrames;         /* Number of frames in an animation */
        int Framerate;          /* Number of milliseconds between two frames */
    } Animation;

    typedef struct{
        char* DisplayName;      /* Character Name */
        
        /* Character Texture properties */
        SDL_Texture* Surface;   /* Character Surface */

        /* Background Interaction properties */
        int* BackgroundOffset;

        /* Character Animation properties */
        Uint32 LastFrame;       /* Time at the last frame display */
        int CurrentFrame;       /* Frame currently being displayed */
        int PlayingAnimation;   /* The Animation currently playing */
        
        /* Character Animation */
        Animation* Anim;        /* Contains all the differents animations for a character */
    } Characters; /* Sprite ? */ /* mmmm */

    typedef struct CharacterList_s{
        Characters* Character;
        Vector2i Coordinates;
        bool Flip;
        bool Shown;
        struct CharacterList_s* NextCharacter;
    } CharacterList;

    typedef struct {
        CharacterList * CharaList;
        SDL_Rect* Viewport;
        char* Flipped;
    } CharacterLayer; /* "Context" */

#endif