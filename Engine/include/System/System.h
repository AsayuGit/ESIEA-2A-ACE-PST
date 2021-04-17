#ifndef _SYSTEM
#define _SYSTEM

    #include "include.h"

    void InitSDL();
    InputDevice* InitInputs(bool JoyEnabled);
    DisplayDevice* CreateDisplayDevice(int ScreenWidth, int ScreenHeight, char* Title);
    SoundDevice* CreateSoundDevice();
    int SetRenderTarget(DisplayDevice* DDevice, SDL_Texture* surface);

#endif