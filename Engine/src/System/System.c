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

void UpdateResolution(DisplayDevice* DDevice){
    int ScreenWidth, ScreenHeight;

    SDL_GetWindowSize(DDevice->Screen, &ScreenWidth, &ScreenHeight);

    DDevice->IRScalar = MAX(MIN(ScreenWidth / BASE_RESOLUTION_X, ScreenHeight / BASE_RESOLUTION_Y), 1);

    DDevice->ScreenResolution.x = ScreenWidth;
    DDevice->ScreenResolution.y = ScreenHeight;

    DDevice->InternalResolution.w = BASE_RESOLUTION_X * DDevice->IRScalar;
    DDevice->InternalResolution.h = BASE_RESOLUTION_Y * DDevice->IRScalar;

    DDevice->InternalResolution.x = (DDevice->ScreenResolution.x - DDevice->InternalResolution.w) >> 1;
    DDevice->InternalResolution.y = (DDevice->ScreenResolution.y - DDevice->InternalResolution.h) >> 1;

    DDevice->OffScreenRender = false;

    DDevice->Frame[0] = InitRect(0, 0, DDevice->InternalResolution.x, ScreenHeight);                                                                                                            /* Left Frame */
    DDevice->Frame[1] = InitRect(DDevice->InternalResolution.x + DDevice->InternalResolution.w, 0, DDevice->InternalResolution.x, ScreenHeight);                                                /* Right Frame */
    DDevice->Frame[2] = InitRect(DDevice->InternalResolution.x, 0, DDevice->InternalResolution.w, DDevice->InternalResolution.y);                                                               /* Top Frame */
    DDevice->Frame[3] = InitRect(DDevice->InternalResolution.x, DDevice->InternalResolution.y + DDevice->InternalResolution.h, DDevice->InternalResolution.w, DDevice->InternalResolution.y);   /* Bottom Frame */
}

DisplayDevice* CreateDisplayDevice(int ScreenWidth, int ScreenHeight, char* Title){
    DisplayDevice* Device = (DisplayDevice*)malloc(sizeof(DisplayDevice));
    
    #ifdef _SDL
        Device->Screen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, 32, SDL_HWSURFACE); /* | SDL_RESIZABLE */
        SDL_WM_SetCaption(Title, NULL);
	    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1); /* VSync */
        Device->Renderer = Device->Screen;
    #else
        Device->Screen = SDL_CreateWindow(Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

    UpdateResolution(Device);

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

void SystemEvents(DisplayDevice* DDevice, InputDevice* IDevice){

    switch (IDevice->event.type){
        case SDL_WINDOWEVENT:
            switch (IDevice->event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                UpdateResolution(DDevice);
                break;
            
            default:
                break;
            }
            break;

        case SDL_KEYDOWN:
            switch (IDevice->event.PADKEY)
            {
            case SDL_SCANCODE_ESCAPE:
                exit(0); /* FIXME: We need a cleaner way of exiting out of the game */
                break;

            default:
                break;
            }
            break;

        default:
            break;
    }

}