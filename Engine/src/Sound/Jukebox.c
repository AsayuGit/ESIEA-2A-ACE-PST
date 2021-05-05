#include "Jukebox.h"

char* MusicPath[NumberOfMusicTracks] = {
    ROOT""MUSIC"Courtroom"MUS_EXT,
    ROOT""MUSIC"Moderato"MUS_EXT,
    ROOT""MUSIC"Telling_the_Thuth"MUS_EXT,
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
    ROOT""EFFECTS"HoldIt"MUS_EXT
};

static Mix_Music* LoadedTrack;
static unsigned int PlayingTrack;

void InitJukebox(void){
    LoadedTrack = NULL;
}

void PlayTrackID(MusicPlaylistID TrackID){
    if (LoadedTrack){
        Mix_HaltMusic();
        Mix_FreeMusic(LoadedTrack);
    }
    LoadedTrack = LoadMusic(MusicPath[TrackID]);
    Mix_PlayMusic(LoadedTrack, -1);
    PlayingTrack = TrackID;
}

unsigned int GetTrackID(void){
    return PlayingTrack;
}

void StopTrack(void){
    PlayingTrack = -1;
    Mix_HaltMusic();
}