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