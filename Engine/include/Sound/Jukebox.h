#ifndef _JUKEBOX_H
#define _JUKEBOX_H

    #include "include.h"

    typedef enum{
        TRK_Courtroom,
        TRK_Moderato,
        TRK_Telling_the_Truth,
        TRK_Objection,
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
        CHK_TestimonySwoosh,
        CHK_HoldIt,
        CHK_Objection,
        CHK_TakeThat,
        NumberOfEffectTracks
    } EffectsPlaylistID;

    extern char* EffectPath[NumberOfEffectTracks];

    void PlayTrackID(MusicPlaylistID TrackID);
    void MusicDaemon(void);
    unsigned int GetTrackID(void);
    void StopTrack(void);

#endif
