#include "System.h"

void InitSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "SDL Initialisation failed\n - %s\n", SDL_GetError());
        exit(EXIT_INIT);
    }
}

InputDevice* InitInputs(bool JoyEnabled){
    InputDevice* Inputs;
    
    Inputs = (InputDevice*)malloc(sizeof(InputDevice));
    Inputs->Joy1 = NULL;
    if (SDL_NumJoysticks() && JoyEnabled){
        Inputs->Joy1 = SDL_JoystickOpen(0); /* Open Joystick */
    }
    Inputs->KeyStates = SDL_GetKeyboardState(NULL); /* Open Keyboard */
    if (Inputs->Joy1 == NULL){
        Inputs->JoyEnabled = false;
    }else{
        Inputs->JoyEnabled = true;
    }
    Inputs->EventEnabled = true;
    return Inputs;
}

DisplayDevice* CreateDisplayDevice(int ScreenWidth, int ScreenHeight, char* Title){
    DisplayDevice* Device = (DisplayDevice*)malloc(sizeof(DisplayDevice));
    
    #ifdef _SDL
        Device->Screen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, 32, SDL_HWSURFACE); /* | SDL_RESIZABLE */
        SDL_WM_SetCaption(Title, NULL);
	    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1); /* VSync */
        Device->Renderer = Device->Screen;
    #else
        Device->Screen = SDL_CreateWindow(Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
    #endif
    if (Device->Screen == NULL){
        fprintf(stderr, "Can't create main window\n - %s\n", SDL_GetError());
        exit(EXIT_INIT);
    }
    #ifndef _SDL
        Device->Renderer = SDL_CreateRenderer(Device->Screen , -1, 0);
        if (Device->Renderer == NULL){
            fprintf(stderr, "Can't create main renderer\n - %s\n", SDL_GetError());
            exit(EXIT_INIT);
        }
        SDL_GL_SetSwapInterval(1); /* VSync */
    #endif

    Device->IRScalar = MAX(MIN(ScreenWidth / BASE_RESOLUTION_X, ScreenHeight / BASE_RESOLUTION_Y), 1);

    Device->ScreenResolution.x = ScreenWidth;
    Device->ScreenResolution.y = ScreenHeight;

    Device->InternalResolution.w = BASE_RESOLUTION_X * Device->IRScalar;
    Device->InternalResolution.h = BASE_RESOLUTION_Y * Device->IRScalar;

    Device->InternalResolution.x = (Device->ScreenResolution.x - Device->InternalResolution.w) >> 1;
    Device->InternalResolution.y = (Device->ScreenResolution.y - Device->InternalResolution.h) >> 1;

    Device->OffScreenRender = false;

    Device->Frame[0] = InitRect(0, 0, Device->InternalResolution.x, ScreenHeight);                                                                                                      /* Left Frame */
    Device->Frame[1] = InitRect(Device->InternalResolution.x + Device->InternalResolution.w, 0, Device->InternalResolution.x, ScreenHeight);                                            /* Right Frame */
    Device->Frame[2] = InitRect(Device->InternalResolution.x, 0, Device->InternalResolution.w, Device->InternalResolution.y);                                                           /* Top Frame */
    Device->Frame[3] = InitRect(Device->InternalResolution.x, Device->InternalResolution.y + Device->InternalResolution.h, Device->InternalResolution.w, Device->InternalResolution.y); /* Bottom Frame */

    return Device;
}

SoundDevice* CreateSoundDevice(){
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0){
        fprintf(stderr, "Can't create sound device\n - %s\n", SDL_GetError());
        exit(EXIT_INIT);
    }
    return NULL;
}

int SetRenderTarget(DisplayDevice* DDevice, SDL_Texture* surface){
    DDevice->OffScreenRender = (surface != NULL);
#ifdef _SDL
    DDevice->Renderer = (surface) ? surface : DDevice->Screen;
    return 0;
#else
    return SDL_SetRenderTarget(DDevice->Renderer, surface);
#endif
}

int ScaledDrawEx(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, bool flip){
    SDL_Rect ScaledDstRect = {0, 0, BASE_RESOLUTION_X, BASE_RESOLUTION_Y};

    if (RectOnScreen(DDevice, dstrect)){
        if (dstrect){
            ScaledDstRect = InitRect(
                (dstrect->x * DDevice->IRScalar) + DDevice->InternalResolution.x,
                (dstrect->y * DDevice->IRScalar) + DDevice->InternalResolution.y,
                dstrect->w * DDevice->IRScalar,
                dstrect->h * DDevice->IRScalar
            );
        }

        #ifdef _SDL
            return FlipBlitSurface(texture, srcrect, DDevice->Screen, &ScaledDstRect, flip);
        #else
            return SDL_RenderCopyEx(DDevice->Renderer, texture, srcrect, &ScaledDstRect, 0, 0, flip);
        #endif
    }
    return 0;
}

int ScaledDraw(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect){
    return ScaledDrawEx(DDevice, texture, srcrect, dstrect, false);
}

void FinishFrame(DisplayDevice* DDevice){
    DrawFrame(DDevice);
    SDL_RenderPresent(DDevice->Renderer);
}