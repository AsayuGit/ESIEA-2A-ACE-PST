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