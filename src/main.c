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

#include "include.h"
#include "Scenes.h"
#ifdef _XBOX
	#include "Xtl.h"
#endif

int main(int argc, char* argv[]){

    DisplayDevice* DDevice = NULL;
    InputDevice* IDevice = NULL;
    /*SoundDevice* SDevice; */

    CourtroomContext* CourtContext = NULL;

    InitSDL();

    DDevice = CreateDisplayDevice(DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y, "Mia's Legacy");
    if (!DDevice){
        printf("ERROR: Couldn't create display device!\n");
        goto ERROR;
    }

    IDevice = InitInputs();
    if (!DDevice){
        printf("ERRPR: Couldn't create input device!\n");
    }

    CreateSoundDevice();

    CourtContext = InitCourtroom(DDevice);
    if (!CourtContext){
        printf("ERROR: Not enough memory!\n");
        goto ERROR;
    }

    /* First we start the title screen */
    if (Scene_TitleScreen(DDevice, IDevice) == 0){
        /* We start the courtroom Scene */
        if (argc > 1){
            Scene_Courtroom(DDevice, IDevice, CourtContext, argv[1]);
        } else {
            Scene_Courtroom(DDevice, IDevice, CourtContext, ROOT"Assets"SL"Dialogue"SL"default.xml");
        }
    }
    
    FreeCourtroom(CourtContext);
    FreeDisplayDevice(DDevice);
    FreeInputDevice(IDevice);
    FreeSoundDevice();

    SDL_Quit();

    return 0;

ERROR:

    if (CourtContext)
        FreeCourtroom(CourtContext);
    if (DDevice)
        FreeDisplayDevice(DDevice);
    if (IDevice)
        FreeInputDevice(IDevice);
    FreeSoundDevice();

    SDL_Quit();

    return 1;
}