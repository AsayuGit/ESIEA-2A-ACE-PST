#include "UI.h"

static void (*currentUI)(DisplayDevice*, InputDevice*);

static Items* UIItemBank;
static unsigned int StoredItemID;
static SDL_Rect ItemPos;

/* Surface and rects */
static Surface* UISurface;
static SDL_Rect UITestimonyRect;
static unsigned int UILastBlink;
static bool UIBlink;
static unsigned int UIBlinkDelay;

static Surface* UIWitnessTestimony;
static SDL_Rect UIWTASrcRect;
static SDL_Rect UIWTBSrcRect;
static SDL_Rect UIWTADstRect;
static SDL_Rect UIWTBDstRect;
static unsigned int UIAnimStart;
static unsigned int UIAnimEnd;
static unsigned char UIWTState;
static unsigned char UIWTAnimFrame;

/* Sound Effets */
static Mix_Chunk* Swoosh;
static bool SwooshPlaying;

/* Blink delay in miliseconds */
#define UI_BLINK_HIDDEN_DELAY 333
#define UI_BLINK_SHOWN_DELAY 1333

/* Slide time in ms */
#define UI_SLIDE 250
#define UI_WT_NBOFFRAMES 5
#define UI_WT_ANIM_FRAMERATE 125
#define UI_WT_PAUSE 1000

void UI_ShowToCourt(DisplayDevice* DDevice, InputDevice* IDevice){
    SDL_RenderCopy(DDevice->Renderer, UIItemBank->ItemSpritesheet, &(UIItemBank->ItemSrcRectArray[StoredItemID]), &(ItemPos)); /* Draw the item */
}

void UI_Testimony(DisplayDevice* DDevice, InputDevice* IDevice){
    if (UIBlink){
        SDL_RenderCopy(DDevice->Renderer, UISurface, &UITestimonyRect, &UITestimonyRect); /* Draw the Icon*/
        UIBlinkDelay = UI_BLINK_SHOWN_DELAY;
    } else {
        UIBlinkDelay = UI_BLINK_HIDDEN_DELAY;
    }
    if (SDL_GetTicks() > UILastBlink + UIBlinkDelay){
        UILastBlink = SDL_GetTicks();
        UIBlink = UIBlink ? false : true;
    }
}

void UI_TestimonyIntro(DisplayDevice* DDevice, InputDevice* IDevice){ /* Timings to tune */
    int Slide;
    
    Slide = (int)dmap(0.0, 224.0, (double)UIAnimStart, (double)UIAnimEnd, (double)SDL_GetTicks());

    switch (UIWTState){
    case 0: /* Setup */
        UIAnimStart = SDL_GetTicks();
        UIAnimEnd = UIAnimStart + UI_SLIDE;
        UIWTState = 0;
        IDevice->EventEnabled = false;
        UIWTState++;
        break;
    case 1: /* Slide On Screen */
        UIWTADstRect.x = -192 + Slide;
        UIWTBDstRect.x = 256 - Slide;
        if (Slide == 224){
            UIAnimStart = SDL_GetTicks();
            UIAnimEnd = UIAnimStart + UI_SLIDE;
            SwooshPlaying = false;
            UIWTState++;
        }
        break;
    case 2: /* Flash screen */ /* TODO */
        if (!SwooshPlaying){
            Mix_PlayChannel(-1, Swoosh, 0);
            SwooshPlaying = true;
        }
        UIWTAnimFrame = 0;
        UIWTASrcRect.x = 0;
        UIWTBSrcRect.x = 0;
        UIAnimEnd = SDL_GetTicks() + UI_WT_ANIM_FRAMERATE;
        UIWTState++;
        break;
    case 3: /* Logo Anim */
        if (SDL_GetTicks() > UIAnimEnd){
            UIWTAnimFrame++;
            UIAnimEnd = SDL_GetTicks() + UI_WT_ANIM_FRAMERATE;
            if (UIWTAnimFrame != UI_WT_NBOFFRAMES){
                UIWTASrcRect.x += UIWTASrcRect.w;
                UIWTBSrcRect.x += UIWTBSrcRect.w;
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
        UIWTADstRect.x = 32 + Slide;
        UIWTBDstRect.x = 32 - Slide;
        if (Slide == 224){
            UIWTState++;
        }
        break;
    case 7:
        IDevice->EventEnabled = true;
        currentUI = &UI_Testimony;
        break;
    default:
        break;
    }

    SDL_RenderCopy(DDevice->Renderer, UIWitnessTestimony, &UIWTASrcRect, &UIWTADstRect);
    SDL_RenderCopy(DDevice->Renderer, UIWitnessTestimony, &UIWTBSrcRect, &UIWTBDstRect);
}

/* Init the notification handler */
void InitUI(DisplayDevice* DDevice, Items* UIItemBankPointer){
    UIItemBank = UIItemBankPointer;
    currentUI = NULL;
    StoredItemID = 0;

    ItemPos.x = 13;
    ItemPos.y = 13;
    ItemPos.w = 70;
    ItemPos.h = 70;

    UISurface = LoadSurface(ROOT""TEXTURES"UI"SL"UI.bmp", DDevice, 0xff00ff, SURFACE_KEYED);
    UITestimonyRect.x = 0;
    UITestimonyRect.y = 0;
    UITestimonyRect.w = 64;
    UITestimonyRect.h = 32;
    /* FIXME : We should really check for the validity of UISurface */

    UIWitnessTestimony = LoadSurface(ROOT""TEXTURES"UI"SL"WitnessTestimony.bmp", DDevice, 0xff00ff, SURFACE_KEYED);
    
    UIWTASrcRect.x = 768;
    UIWTASrcRect.y = 0;
    UIWTASrcRect.w = 192;
    UIWTASrcRect.h = 39;
    
    UIWTBSrcRect.x = 768;
    UIWTBSrcRect.y = 40;
    UIWTBSrcRect.w = 192;
    UIWTBSrcRect.h = 44;

    UIWTADstRect.w = UIWTASrcRect.w;
    UIWTADstRect.h = UIWTASrcRect.h;
    UIWTBDstRect.w = UIWTBSrcRect.w;
    UIWTBDstRect.h = UIWTBSrcRect.h;
    UIWTADstRect.y = 34;
    UIWTBDstRect.y = 74;

    /* Sound Effects */
    Swoosh = LoadSoundEffect(EffectPath[CHK_TestimonySwoosh]);
    /* Mix_VolumeChunk(Swoosh, 64);*/
}

/* Add a ui element */
void setUI(unsigned int UIType, unsigned int argument){
    switch (UIType){
        case UI_EMPTY:
            currentUI = NULL;
            break;
        case TESTIMONY_ICON:
            currentUI = &UI_TestimonyIntro;
            break;
        case SHOW_ITEM_TO_COURT:
            currentUI = &UI_ShowToCourt;
            StoredItemID = argument;
            break;
        default:
            break;
    }
}

/* Draw the current notification on screen */
void DrawUI(DisplayDevice* DDevice, InputDevice* IDevice){
    if (currentUI)
        currentUI(DDevice, IDevice);
}

/* FIXME: Please make a function to free everything after you're done */