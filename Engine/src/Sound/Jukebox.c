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

#include "Jukebox.h"

char* MusicPath[NumberOfMusicTracks][2] = {
    {NULL, ROOT""MUSIC"Courtroom"MUS_EXT},
    {NULL, ROOT""MUSIC"Moderato"MUS_EXT},
    {NULL, ROOT""MUSIC"Telling_the_Thuth"MUS_EXT},
    {ROOT""MUSIC"Objection!_INTRO"MUS_EXT, ROOT""MUSIC"Objection!_LOOP"MUS_EXT},
    {NULL, ROOT""MUSIC"Courtroom Lobby The First Prelude"MUS_EXT}
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