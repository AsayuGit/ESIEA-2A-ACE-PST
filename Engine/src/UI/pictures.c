#include "pictures.h"

static Pics loadedScene;
static int currentPicture;

#define PIC_SIZEX 256
#define PIC_SIZEY 192

/* FIXME: Please make a file format for this*/
/* FIXME: We should use backgrounds instead of this */

void InitPictures(DisplayDevice* DDevice){
    loadedScene.PicsPath = "Assets/Textures/Scenes/Scene1.bmp"; /* FIXME: Maybe useless in this context*/
    loadedScene.PicsSurface = LoadSurface(loadedScene.PicsPath, DDevice, 0xff00ff, SURFACE_KEYED);
    loadedScene.SizeX = 3;
    loadedScene.SizeY = 1;
    currentPicture = -1;
}

void SetPicture(int PictureID){
    currentPicture = PictureID;
}

void DrawPictures(DisplayDevice* DDevice){
    SDL_Rect picSrdRect;

    if (currentPicture >= 0) {
        picSrdRect.x = (currentPicture % loadedScene.SizeX) * PIC_SIZEX;
        picSrdRect.y = (currentPicture / loadedScene.SizeX) * PIC_SIZEX;
        picSrdRect.w = PIC_SIZEX;
        picSrdRect.h = PIC_SIZEY;

        #ifdef _SDL
            SDL_BlitSurface(loadedScene.PicsSurface, &picSrdRect, DDevice->Renderer, NULL);
        #else
            SDL_RenderCopy(DDevice->Renderer, loadedScene.PicsSurface, &picSrdRect, NULL);
        #endif
    }
}