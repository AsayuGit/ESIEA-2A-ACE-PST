#include "SceneCommon.h"

Vector2d CourtAnimStates[3] = {
    {0.0, 1.6},
    {4.0, 4.0},
    {0.0, 1.6}
};

int CourtAnimRuntime[3] = {
    64,
    320,
    64
};

Vector2d CourtAnimRange[3] = {
    {0.0, 10.0},
    {0.0, 10.56},
    {0.0, 10.0}
};

BGAnimation CourtAnim[1] = {
    3,
    CourtAnimStates,
    CourtAnimRuntime,
    CourtAnimRange,
    {0, 0, 648, 192}
};

Background Scenes[BackgroundsCount] = {
    {
        ROOT""TEXTURES"Places"SL"Courtroom"TEX_EXT,
        CourtAnim
    }
};

SceneContext* InitScene(DisplayDevice* DDevice, int BackgroundID){
    SceneContext* LoadingContext;
    LoadingContext = (SceneContext*)malloc(sizeof(SceneContext));
    LoadingContext->PlayingAnimation = -1;
    LoadingContext->TileID = LoadingContext->SrcRect.x = LoadingContext->SrcRect.y = LoadingContext->ObjectLayerOffset = 0;
    LoadingContext->Surface = LoadSurface((char*)Scenes[BackgroundID].SurfacePath, DDevice, NULL, false);
    #ifdef _SDL
        LoadingContext->SurfaceBounds.x = LoadingContext->Surface->w;
        LoadingContext->SurfaceBounds.y = LoadingContext->Surface->h;
    #else
        SDL_QueryTexture(LoadingContext->Surface, NULL, NULL, &LoadingContext->SurfaceBounds.x, &LoadingContext->SurfaceBounds.y);
    #endif
    LoadingContext->SrcRect.w = DDevice->ScreenResolution.x;
    LoadingContext->SrcRect.h = DDevice->ScreenResolution.y;
    LoadingContext->Animation = Scenes[BackgroundID].Animation;
    return LoadingContext;
}

void MoveTile(SceneContext* Context, int TileID){
    Context->SrcRect.y = ((TileID * Context->SrcRect.w) / Context->SurfaceBounds.x);
    Context->SrcRect.x = (TileID - (Context->SrcRect.y * (Context->SurfaceBounds.x / Context->SrcRect.w))) * Context->SrcRect.w;
    Context->SrcRect.y *=  Context->SrcRect.h;
    Context->ObjectLayerOffset = 0;
}

void BackgroundPlayAnimation(SceneContext* Context, int AnimationID){
    Context->PlayingAnimation = AnimationID;
    Context->StartFrame = Context->CurrentState = Context->AnimOffset = 0;
    Context->ObjectLayerOffset = 0;
}

void DisplayBackground(DisplayDevice* DDevice, SceneContext* Context){
    SDL_Rect AnimSrcRect, AnimDstRect;
    Uint32 TimeProgress;
    double Progress;

    if ((Context->PlayingAnimation >= 0) && (Context->CurrentState < Context->Animation->NbOfAnimStates)){
        
        if (Context->StartFrame == 0){
            Context->StartFrame = SDL_GetTicks();
        }
        TimeProgress = SDL_GetTicks() - Context->StartFrame;
        Progress = (TimeProgress * Context->Animation->AnimRange[Context->CurrentState].y) / Context->Animation->AnimRuntime[Context->CurrentState];
        if (Progress > Context->Animation->AnimRange[Context->CurrentState].y){
            Progress = Context->Animation->AnimRange[Context->CurrentState].y;
        }

        Context->ObjectLayerOffset = (Context->Animation->AnimStates[Context->CurrentState].x * (Progress * Progress)) + (Context->Animation->AnimStates[Context->CurrentState].y * Progress) + Context->AnimOffset;
        AnimSrcRect.x =  Context->ObjectLayerOffset + Context->Animation->AnimRegion.x;
        AnimSrcRect.y = Context->Animation->AnimRegion.y;
        AnimSrcRect.w = DDevice->ScreenResolution.x;
        AnimSrcRect.h = DDevice->ScreenResolution.y;

        AnimDstRect.x = AnimDstRect.y = 0;
        AnimDstRect.w = DDevice->ScreenResolution.x;
        AnimDstRect.h = DDevice->ScreenResolution.y;

        #ifdef _SDL
            SDL_BlitSurface(Context->Surface, &AnimSrcRect, DDevice->Screen, &AnimDstRect); 
        #else
            SDL_RenderCopy(DDevice->Renderer, Context->Surface, &AnimSrcRect, &AnimDstRect);
        #endif

        if (Progress == Context->Animation->AnimRange[Context->CurrentState].y){
            Context->CurrentState++;
            Context->StartFrame = 0;
            Context->AnimOffset = AnimSrcRect.x;
            if (Context->CurrentState == Context->Animation->NbOfAnimStates){
                Context->SrcRect = AnimSrcRect;
                Context->PlayingAnimation = -1;
            }
        }

    }else{
        #ifdef _SDL
            SDL_BlitSurface(Context->Surface, &Context->SrcRect, DDevice->Screen, NULL); 
        #else
            SDL_RenderCopy(DDevice->Renderer, Context->Surface, &Context->SrcRect, NULL);
        #endif
    }
}