#include "Jukebox.h"

char* MusicPath[NumberOfMusicTracks] = {
    ROOT""MUSIC"Courtroom"MUS_EXT
};

char* EffectPath[NumberOfEffectTracks] = {
    ROOT""EFFECTS"ButtonUpDown"MUS_EXT,
    ROOT""EFFECTS"ButtonClicked"MUS_EXT,
    ROOT""EFFECTS"Gravel"MUS_EXT,
    ROOT""EFFECTS"ButtonBack"MUS_EXT
};

Mix_Music* LoadedTrack;

void InitJukebox(){
    LoadedTrack = NULL;
}

void PlayTrackID(MusicPlaylistID TrackID){
    if (LoadedTrack){
        Mix_HaltMusic();
        Mix_FreeMusic(LoadedTrack);
    }
    LoadedTrack = LoadMusic(MusicPath[TrackID]);
    Mix_PlayMusic(LoadedTrack, -1);
}