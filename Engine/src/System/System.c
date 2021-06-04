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

#include "System.h"

void InitSDL(){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
        fprintf(stderr, "SDL Initialisation failed\n - %s\n", SDL_GetError());
        exit(EXIT_INIT);
    }
}

void FreeInputDevice(InputDevice* IDevice){
    if (IDevice->Joy1)
        SDL_JoystickClose(IDevice->Joy1);
    free(IDevice);
}

InputDevice* InitInputs(void){
    InputDevice* Inputs;
    
    Inputs = (InputDevice*)calloc(1, sizeof(InputDevice));
    Inputs->Joy1 = NULL;
    if (SDL_NumJoysticks()){
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

    #ifdef _SDL
        ScreenWidth = DDevice->Screen->w;
        ScreenHeight = DDevice->Screen->h;
    #else
        SDL_GetWindowSize(DDevice->Screen, &ScreenWidth, &ScreenHeight);
    #endif

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

void FreeDisplayDevice(DisplayDevice* DDevice){
    #ifdef _SDL
        SDL_FreeSurface(DDevice->Screen);
    #else
        SDL_DestroyRenderer(DDevice->Renderer);
        SDL_DestroyWindow(DDevice->Screen);
    #endif
    free(DDevice);
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

void FreeSoundDevice(void){
    Mix_CloseAudio();
}

SoundDevice* CreateSoundDevice(){
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0){
        fprintf(stderr, "Can't create sound device\n - %s\n", SDL_GetError());
        exit(EXIT_INIT);
    }
    return NULL;
}

int ACE_SetRenderTarget(DisplayDevice* DDevice, SDL_Texture* surface){
    DDevice->OffScreenRender = (surface != NULL);
#ifdef _SDL
    DDevice->Renderer = (surface) ? surface : DDevice->Screen;
    return 0;
#else
    return SDL_SetRenderTarget(DDevice->Renderer, surface);
#endif
}

int DrawEx(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, bool flip){
	#ifdef _SDL
        FlipBlitSurface(texture, srcrect, DDevice->Renderer, dstrect, flip);
        return 0;
    #else
        return SDL_RenderCopyEx(DDevice->Renderer, texture, srcrect, dstrect, 0, 0, flip);
    #endif
}

int Draw(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect){
    return DrawEx(DDevice, texture, srcrect, dstrect, 0);
}

int ScaledDrawEx(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, bool flip){
    SDL_Rect ScaledDstRect = {0, 0, BASE_RESOLUTION_X, BASE_RESOLUTION_Y};
    #ifdef _SDL
        SDL_Rect ScaledSrcRect;
    #endif


    if (texture && RectOnScreen(DDevice, dstrect)){
        
        #ifdef _SDL
            if (srcrect){
                ScaledSrcRect = InitRect(
                    (srcrect->x * DDevice->IRScalar) + DDevice->InternalResolution.x,
                    (srcrect->y * DDevice->IRScalar) + DDevice->InternalResolution.y,
                    srcrect->w * DDevice->IRScalar,
                    srcrect->h * DDevice->IRScalar
                );
            }
        #endif

        if (dstrect){
            ScaledDstRect = InitRect(
                (dstrect->x * DDevice->IRScalar) + DDevice->InternalResolution.x,
                (dstrect->y * DDevice->IRScalar) + DDevice->InternalResolution.y,
                dstrect->w * DDevice->IRScalar,
                dstrect->h * DDevice->IRScalar
            );
        }
        #ifdef _SDL
            return DrawEx(DDevice, texture, &ScaledSrcRect, &ScaledDstRect, flip);
        #else
            return DrawEx(DDevice, texture, srcrect, &ScaledDstRect, flip);
        #endif
    }
    return 0;
}

int ScaledDraw(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect){
    return ScaledDrawEx(DDevice, texture, srcrect, dstrect, false);
}

void FinishFrame(DisplayDevice* DDevice){
    DrawFrame(DDevice);
    #ifdef _SDL
        SDL_Flip(DDevice->Screen);
    #else
        SDL_RenderPresent(DDevice->Renderer);
    #endif
}

void SystemEvents(DisplayDevice* DDevice, InputDevice* IDevice){
	SDL_Event assertedEvent;

    switch (IDevice->event.type){
        #ifndef _SDL
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

		case SDL_JOYHATMOTION:
			assertedEvent.type = PAD_KEYDOWN;
			switch (IDevice->event.jhat.value){
				case SDL_HAT_LEFT:
					assertedEvent.PADKEY = PAD_LEFT;
					SDL_PushEvent(&assertedEvent);
					break;
				case SDL_HAT_RIGHT:
					assertedEvent.PADKEY = PAD_RIGHT;
					SDL_PushEvent(&assertedEvent);
					break;
				case SDL_HAT_UP:
					assertedEvent.PADKEY = PAD_UP;
					SDL_PushEvent(&assertedEvent);
					break;
				case SDL_HAT_DOWN:
					assertedEvent.PADKEY = PAD_DOWN;
					SDL_PushEvent(&assertedEvent);
					break;
				default:
					break;
			}
			break;

        case SDL_KEYDOWN:
            switch (IDevice->event.PADKEY)
            {
            case PAD_QUIT:
                exit(0); /* FIXME: We need a cleaner way of exiting out of the game */
                break;
            
            case PAD_FULLSCREEN:
                SDL_SetWindowFullscreen(DDevice->Screen, (SDL_GetWindowFlags(DDevice->Screen) & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
                break;

            case PAD_SNAP:
                SDL_SetWindowSize(DDevice->Screen, BASE_RESOLUTION_X * DDevice->IRScalar, BASE_RESOLUTION_Y * DDevice->IRScalar);
                UpdateResolution(DDevice);
                break;

            default:
                break;
            }
            break;

        #endif

        default:
            break;
    }

}