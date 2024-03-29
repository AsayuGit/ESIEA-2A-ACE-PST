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

#include "UI.h"
#include "math.h"

static void (*currentUI)(DisplayDevice*, InputDevice*);

static Items* UIItemBank;
static SceneContext* SContextPtr;
static unsigned int StoredItemID;

/* Surface and rects */
static SDL_Texture* UISurface = NULL;
static unsigned int UILastBlink;
static bool UIBlink;
static unsigned int UIBlinkDelay;

static SDL_Rect UI_ASrcRect[2] = {{0, 33, 192, 39}, {0, 117, 192, 39}};
static SDL_Rect UI_BSrcRect[2] = {{0, 73, 192, 39}, {0, 157, 192, 39}};
static SDL_Rect UI_ADstRect;
static SDL_Rect UI_BDstRect;

static unsigned int UIAnimStart;
static unsigned int UIAnimEnd;
static unsigned char UIWTState = 0;
static unsigned char UIWTAnimFrame;

static unsigned int logoID;
static bool BlowLife = false;
static unsigned int* Lives;

/* Sound Effets */
static Mix_Chunk* Swoosh;
static Mix_Chunk* Mix_Excla[3];
static Mix_Chunk* Damage;
static bool EffectPlaying;

/* Blink delay in miliseconds */
#define UI_BLINK_HIDDEN_DELAY 333
#define UI_BLINK_SHOWN_DELAY 1333

/* Slide time in ms */
#define UI_SLIDE 250
#define UI_WT_NBOFFRAMES 5
#define UI_WT_ANIM_FRAMERATE 125
#define UI_WT_PAUSE 1000

#define UI_LIFEBLOW_FRAMETIME 64

void UI_ShowToCourt(DisplayDevice* DDevice, InputDevice* IDevice){
    const SDL_Rect ItemPos[2] = {{13, 13, 70, 70}, {173, 13, 70, 70}};

    ScaledDraw(DDevice, UIItemBank->ItemSpritesheet, &(UIItemBank->ItemSrcRectArray[StoredItemID]), &(ItemPos[logoID])); /* Draw the item */
}

void UI_Testimony(DisplayDevice* DDevice, InputDevice* IDevice){
    const SDL_Rect UITestimonyRect = {0, 0, 64, 32};

    if (UIBlink){
        ScaledDraw(DDevice, UISurface, &UITestimonyRect, &UITestimonyRect); /* Draw the Icon*/
        UIBlinkDelay = UI_BLINK_SHOWN_DELAY;
    } else {
        UIBlinkDelay = UI_BLINK_HIDDEN_DELAY;
    }
    if (SDL_GetTicks() > UILastBlink + UIBlinkDelay){
        UILastBlink = SDL_GetTicks();
        UIBlink = UIBlink ? false : true;
    }
}

void UI_Lives(DisplayDevice* DDevice, InputDevice* IDevice){
    SDL_Rect SrcLives = {65, 0, 16, 16};
    SDL_Rect DstLives = {239, 16, 16, 16};
    unsigned int i;

    if (*Lives){
        for (i = 1; i < *Lives; i++){
            ScaledDraw(DDevice, UISurface, &SrcLives, &DstLives);
            DstLives.x -= SrcLives.w;
        }

        if (BlowLife){
            switch (UIWTState){
                case 0: /* Setup */
                    IDevice->EventEnabled = false;
                    UIWTAnimFrame = 0;
                    UIAnimEnd = SDL_GetTicks() + UI_LIFEBLOW_FRAMETIME;
                    Mix_PlayChannel(-1, Damage, 0);
                    UIWTState++;
                case 1: /* Animation */
                    SrcLives.x += SrcLives.w * UIWTAnimFrame;
                    if (SDL_GetTicks() > UIAnimEnd){
                        UIWTAnimFrame++;
                        if (UIWTAnimFrame == 8){
                            BlowLife = false;
                            UIWTState = 0;
                            (*Lives)--;
                            IDevice->EventEnabled = true;
                            break;
                        }
                        UIAnimEnd = SDL_GetTicks() + UI_LIFEBLOW_FRAMETIME;
                    }
                    break;
            }
        }

        ScaledDraw(DDevice, UISurface, &SrcLives, &DstLives);
    }
}

void UI_TeCeIntro(DisplayDevice* DDevice, InputDevice* IDevice){ /* Timings to tune */
    int Slide;
    
    Slide = (int)dmap(0.0, 224.0, (double)UIAnimStart, (double)UIAnimEnd, (double)SDL_GetTicks());

    switch (UIWTState){
    case 0: /* Setup */
        UI_ADstRect.x = -192;
        UI_ADstRect.y = 34;
        UI_BDstRect.x = 256;
        UI_BDstRect.y = 74;
        UIAnimStart = SDL_GetTicks();
        UIAnimEnd = UIAnimStart + UI_SLIDE;
        IDevice->EventEnabled = false;
        Slide = 0;
        UIWTState++;
    case 1: /* Slide On Screen */
        UI_ADstRect.x = -192 + Slide;
        UI_BDstRect.x = 256 - Slide;
        if (Slide == 224){
            UIAnimStart = SDL_GetTicks();
            UIAnimEnd = UIAnimStart + UI_SLIDE;
            EffectPlaying = false;
            UIWTState++;
        }
        break;
    case 2: /* Flash screen */ /* TODO */
        if (!EffectPlaying){
            Mix_PlayChannel(-1, Swoosh, 0);
            EffectPlaying = true;
        }
        UIWTAnimFrame = 0;
        UI_ASrcRect[logoID].x = 0;
        UI_BSrcRect[logoID].x = 0;
        UIAnimEnd = SDL_GetTicks() + UI_WT_ANIM_FRAMERATE;
        UIWTState++;
        break;
    case 3: /* Logo Anim */
        if (SDL_GetTicks() > UIAnimEnd){
            UIWTAnimFrame++;
            UIAnimEnd = SDL_GetTicks() + UI_WT_ANIM_FRAMERATE;
            if (UIWTAnimFrame != UI_WT_NBOFFRAMES){
                UI_ASrcRect[logoID].x += UI_ASrcRect[logoID].w;
                UI_BSrcRect[logoID].x += UI_BSrcRect[logoID].w;
            } else {
                UIWTState++;
            }
        }
        break;
    case 4: /* Logo shine */ /* TODO */
        UIAnimEnd = SDL_GetTicks() + UI_WT_PAUSE;
        UIWTState++;
        break;
    case 5: /* Short Pause */
        if (SDL_GetTicks() > UIAnimEnd){
            UIAnimStart = SDL_GetTicks();
            UIAnimEnd = UIAnimStart + UI_SLIDE;
            UIWTState++;
        }
        break;
    case 6: /* Slide out */
        if (logoID){
            UI_ADstRect.y = 34 - Slide;
            UI_BDstRect.y = 74 + Slide;
        } else {
            UI_ADstRect.x = 32 + Slide;
            UI_BDstRect.x = 32 - Slide;
        }
        if (Slide == 224){
            UIWTState++;
        }
        break;
    case 7:
        IDevice->EventEnabled = true;
        currentUI = (logoID) ? &UI_Lives : &UI_Testimony;
        UIWTState = 0;
        break;
    default:
        break;
    }
    
    ScaledDraw(DDevice, UISurface, &UI_ASrcRect[logoID], &UI_ADstRect);
    ScaledDraw(DDevice, UISurface, &UI_BSrcRect[logoID], &UI_BDstRect);
}

void UI_Exclamation(DisplayDevice* DDevice, InputDevice* IDevice){
    const SDL_Rect UI_ExclaSrcRect[3] = {{0, 197, 254, 192}, {254, 197, 254, 192}, {510, 197, 254, 192}};
    SDL_Rect UI_ExclaDstRect = {1, 0, 254, 191};

    switch (UIWTState){
    case 0: /* Setup */
        UIAnimStart = SDL_GetTicks();
        UIAnimEnd = UIAnimStart + 600;
        EffectPlaying = false;
        IDevice->EventEnabled = false;
        SContextPtr->DiagShown = false;
        UIWTState++;
    case 1: /* Shake & sound*/
        UI_ExclaDstRect.x += (int)(sin((SDL_GetTicks()/3)*6.0f)*5.0f);
        UI_ExclaDstRect.y += (int)(sin((SDL_GetTicks()/3)*6.5f)*5.0f);
        if (!EffectPlaying){
            Mix_PlayChannel(-1, Mix_Excla[logoID], 0);
            EffectPlaying = true;
        }
        if (SDL_GetTicks() > UIAnimEnd){
            UIAnimStart = SDL_GetTicks();
            UIAnimEnd = UIAnimStart + 200;
            UIWTState++;
        }
        break;
    case 2:
        if (SDL_GetTicks() > UIAnimEnd){
            parseScene(DDevice, SContextPtr);
            currentUI = NULL;
            IDevice->EventEnabled = true;
            SContextPtr->DiagShown = true;
            UIWTState = 0;
            if (StoredItemID)
                currentUI = &UI_Lives;
        }
        break;

    default:
        break;
    }

    ScaledDraw(DDevice, UISurface, &UI_ExclaSrcRect[logoID], &UI_ExclaDstRect);
}

/* Init the notification handler */
void InitUI(DisplayDevice* DDevice, Items* UIItemBankPointer, SceneContext* SContext){
    UIItemBank = UIItemBankPointer;
    SContextPtr = SContext;
    currentUI = NULL;
    StoredItemID = 0;

    UISurface = LoadSurface(ROOT""TEXTURES"UI"SL"UI.bmp", DDevice, 0xff00ff, SURFACE_KEYED);
	if (!UISurface){
		printf("ERROR: Couldn't load UISurface !\n");
		/*exit(-1);*/
	}

    UI_ADstRect.w = UI_ASrcRect[logoID].w;
    UI_ADstRect.h = UI_ASrcRect[logoID].h;

    UI_BDstRect.w = UI_BSrcRect[logoID].w;
    UI_BDstRect.h = UI_BSrcRect[logoID].h;

    /* Sound Effects */
    Swoosh = LoadSoundEffect(EffectPath[CHK_TestimonySwoosh]);
    Mix_Excla[0] = LoadSoundEffect(EffectPath[CHK_HoldIt]);
    Mix_Excla[1] = LoadSoundEffect(EffectPath[CHK_Objection]);
    Mix_Excla[2] = LoadSoundEffect(EffectPath[CHK_TakeThat]);
    Damage = LoadSoundEffect(EffectPath[CHK_Damage]);
    /*Mix_VolumeChunk(HoldIT, 32);*/

    Lives = &SContext->CContext->Lives;
}

/* Add a ui element */
void setUI(unsigned int UIType, unsigned int argument){
    switch (UIType){
        case UI_LIVES:
            currentUI = &UI_Lives;
            BlowLife = argument;
            break;
        case TESTIMONY_ICON:
            currentUI = &UI_TeCeIntro;
            logoID = 0;
            break;
        case CROSS_EXAMINATION:
            currentUI = &UI_TeCeIntro;
            logoID = 1;
            break;
        case SHOW_ITEM_TO_COURTL:
            logoID = 0;
            currentUI = &UI_ShowToCourt;
            StoredItemID = argument;
            break;
        case SHOW_ITEM_TO_COURTR:
            logoID = 1;
            currentUI = &UI_ShowToCourt;
            StoredItemID = argument;
            break;
        case HOLD_IT:
            currentUI = &UI_Exclamation;
            StoredItemID = argument;
            logoID = 0;
            break;
        case OBJECTION:
            currentUI = &UI_Exclamation;
            StoredItemID = argument;
            logoID = 1;
            break;
        case TAKE_THAT:
            currentUI = &UI_Exclamation;
            StoredItemID = argument;
            logoID = 2;
            break;
        default:
            currentUI = NULL;
            break;
    }
}

/* Draw the current notification on screen */
void DrawUI(DisplayDevice* DDevice, InputDevice* IDevice){
    if (currentUI)
        currentUI(DDevice, IDevice);
}

/* FIXME URGENT: Please make a function to free everything after you're done */