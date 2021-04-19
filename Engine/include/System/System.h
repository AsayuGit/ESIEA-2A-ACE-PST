#ifndef _SYSTEM_H
#define _SYSTEM_H

    #include "include.h"

    void InitSDL();
    InputDevice* InitInputs(bool JoyEnabled);
    DisplayDevice* CreateDisplayDevice(int ScreenWidth, int ScreenHeight, char* Title);
    SoundDevice* CreateSoundDevice();
    int SetRenderTarget(DisplayDevice* DDevice, SDL_Texture* surface);

#endif