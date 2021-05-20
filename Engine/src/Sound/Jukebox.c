#include "Jukebox.h"

char* MusicPath[NumberOfMusicTracks][2] = {
    {NULL, ROOT""MUSIC"Courtroom"MUS_EXT},
    {NULL, ROOT""MUSIC"Moderato"MUS_EXT},
    {NULL, ROOT""MUSIC"Telling_the_Thuth"MUS_EXT},
    {ROOT""MUSIC"Objection!_INTRO"MUS_EXT, ROOT""MUSIC"Objection!_LOOP"MUS_EXT}
};

char* EffectPath[NumberOfEffectTracks] = {
    ROOT""EFFECTS"ButtonUpDown"MUS_EXT,
    ROOT""EFFECTS"ButtonClicked"MUS_EXT,
    ROOT""EFFECTS"Gravel"MUS_EXT,
    ROOT""EFFECTS"ButtonBack"MUS_EXT,
    ROOT""EFFECTS"Letter"MUS_EXT,
    ROOT""EFFECTS"HPLetter"MUS_EXT,
    ROOT""EFFECTS"NextLine"MUS_EXT,
    ROOT""EFFECTS"LineComplete"MUS_EXT,
    ROOT""EFFECTS"TestimonySwoosh"MUS_EXT,
    ROOT""EFFECTS"HoldIt"MUS_EXT,
    ROOT""EFFECTS"Objection"MUS_EXT,
    ROOT""EFFECTS"TakeThat"MUS_EXT
};

static Mix_Music* Track_INTRO = NULL;
static Mix_Music* Track_LOOP = NULL;
static int PlayingTrack = -1;

/* MUSIC QUEUE SYSTEM */

void PlayTrackID(MusicPlaylistID TrackID){
    StopTrack();

    if (Track_INTRO){
        Mix_FreeMusic(Track_INTRO);
        Track_INTRO = NULL;
    }

    if (Track_LOOP){
        Mix_FreeMusic(Track_LOOP);
        Track_LOOP = NULL;
    }
    
    if (MusicPath[TrackID][0])
        Track_INTRO = LoadMusic(MusicPath[TrackID][0]);
    if (MusicPath[TrackID][1]){
        Track_LOOP = LoadMusic(MusicPath[TrackID][1]);
    }

    if (Track_INTRO)
        Mix_PlayMusic(Track_INTRO, 1);
    
    PlayingTrack = TrackID;
}

void MusicDaemon(void){
    if ((PlayingTrack >= 0) && (!Mix_PlayingMusic()) && Track_LOOP){
        Mix_PlayMusic(Track_LOOP, -1);
    }
}

unsigned int GetTrackID(void){
    return PlayingTrack;
}

void StopTrack(void){
    PlayingTrack = -1;
    Mix_HaltMusic();
}