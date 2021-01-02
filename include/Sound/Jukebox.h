#ifndef _JUKEBOX_H
#define _JUKEBOX_H

    #include "include.h"

    typedef enum{
        TRK_Courtroom,
        NumberOfMusicTracks
    } MusicPlaylistID;

    typedef enum{
        CHK_ButtonUpDown,
        CHK_ButtonClicked,
        CHK_Gravel,
        CHK_ButtonBack,
        CHK_Letter,
        CHK_HPLetter,
        CHK_NextLine,
        CHK_LineComplete,
        NumberOfEffectTracks
    } EffectsPlaylistID;

    extern char* EffectPath[NumberOfEffectTracks];

    void InitJukebox();
    void PlayTrackID(MusicPlaylistID TrackID);

#endif
