#ifndef _SYSTEM_H
#define _SYSTEM_H

    #include "include.h"

    void InitSDL();
    InputDevice* InitInputs(bool JoyEnabled);
    DisplayDevice* CreateDisplayDevice(int ScreenWidth, int ScreenHeight, char* Title);
    SoundDevice* CreateSoundDevice();
    int SetRenderTarget(DisplayDevice* DDevice, SDL_Texture* surface);
    int DrawEx(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, bool flip);
    int Draw(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
    int ScaledDrawEx(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, bool flip);
    int ScaledDraw(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
    void FinishFrame(DisplayDevice* DDevice);
    void SystemEvents(DisplayDevice* DDevice, InputDevice* IDevice);
    
#endif