#include "Dialogue.h"

void ClearDialogueText(DialogueContext* Context){
    #ifdef _SDL
        SDL_FillRect(Context->textLayer, NULL, 0x000000);
    #else
        SDL_SetRenderTarget(Context->DDevice->Renderer, Context->textLayer);
        SDL_RenderClear(Context->DDevice->Renderer);
        SDL_SetRenderTarget(Context->DDevice->Renderer, NULL); // Keep or nah ?
    #endif
}

int SetDialogueText(DialogueContext* Context, char* Text, char SndEffType){
    Context->Text = Text;
    Context->progress = 0;
    Context->DstLetter.x = Context->DialogBoxBounds.x;
    Context->DstLetter.y = Context->DialogBoxBounds.y;
    ClearDialogueText(Context);

    switch (SndEffType){
        case 0:
            // Nothing
            break;
        case 1:
            Mix_PlayChannel(-1, Context->NextLine, 0);
            break;
        case 2:
            Mix_PlayChannel(-1, Context->LineComplete, 0);
            break;
    }

    return strlen(Text);
}

DialogueContext* InitDialog(DisplayDevice* DDevice, BitmapFont* Font){
    
    DialogueContext* DiagContext;
    DiagContext = (DialogueContext*)malloc(sizeof(DialogueContext));
    DiagContext->TextSpeed = 40;
    DiagContext->LastLetter = 0;
    DiagContext->DialogBox = NULL;
    DiagContext->DialogBox = LoadSurface(ROOT""TEXTURES"Menus"SL"Dialog"TEX_EXT, DDevice, NULL, true);
    if (DiagContext->DialogBox == NULL){
        fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
    }
    DiagContext->textLayer = NULL;

    #ifdef _SDL
        DiagContext->DialogBoxBounds.w = DiagContext->DialogBox->w;
        DiagContext->DialogBoxBounds.h = DiagContext->DialogBox->h;
        DiagContext->textLayer = CreateEmptySurface(DiagContext->DialogBoxBounds.w, DiagContext->DialogBoxBounds.h);
    #else
        SDL_QueryTexture(DiagContext->DialogBox, NULL, NULL, &(DiagContext->DialogBoxBounds.w), &(DiagContext->DialogBoxBounds.h));
        DiagContext->textLayer = SDL_CreateTexture(DDevice->Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DiagContext->DialogBoxBounds.w, DiagContext->DialogBoxBounds.h);
        SDL_SetTextureBlendMode(DiagContext->textLayer, SDL_BLENDMODE_BLEND);
    #endif

    if (DiagContext->textLayer == NULL){
        fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
    }
    
    DiagContext->Font = Font;
    DiagContext->DstLetter.x = DiagContext->DialogBoxBounds.x = 5;
    DiagContext->DstLetter.y = DiagContext->DialogBoxBounds.y = 135;
    
    // Text Margin
    DiagContext->DialogBoxBounds.w -= 10;
    DiagContext->DialogBoxBounds.h -= 10;

    DiagContext->letterLag = 0;
    DiagContext->Letter = LoadSoundEffect(EffectPath[CHK_Letter]);
    DiagContext->NextLine = LoadSoundEffect(EffectPath[CHK_NextLine]);
    DiagContext->LineComplete = LoadSoundEffect(EffectPath[CHK_LineComplete]);
    Mix_VolumeChunk(DiagContext->Letter, 32);
    Mix_VolumeChunk(DiagContext->NextLine, 32);
    Mix_VolumeChunk(DiagContext->LineComplete, 32);

    DiagContext->DDevice = DDevice;
    SetDialogueText(DiagContext, "", 0);
    return DiagContext;
}

// Fonction non bloquante gérant les dialogues
void Dialogue(InputDevice* InputDevice, DialogueContext* Context){
    // DialogBox Rendering
    #ifdef _SDL
        SDL_BlitSurface(Context->DialogBox, NULL, Context->DDevice->Screen, NULL);
    #else
        SDL_RenderCopy(Context->DDevice->Renderer, Context->DialogBox, NULL, NULL);
    #endif

    if ((Context->Text[Context->progress] != '\0') && (SDL_GetTicks() >= Context->LastLetter + Context->TextSpeed)){
        #ifndef _SDL
            SDL_SetRenderTarget(Context->DDevice->Renderer, Context->textLayer);
        #endif
        if (!Context->letterLag){
            Mix_PlayChannel(-1, Context->Letter, 0);
            Context->letterLag = 1;
        }else{
            Context->letterLag--;
        }
        Context->DstLetter = gputc(Context->DDevice, Context->Font, Context->Text[Context->progress], Context->DstLetter.x, Context->DstLetter.y, Context->DstLetter.h, &(Context->DialogBoxBounds));
        Context->progress++;
        Context->LastLetter = SDL_GetTicks();
        #ifndef _SDL
            SDL_SetRenderTarget(Context->DDevice->Renderer, NULL);
        #endif
    }

    #ifdef _SDL
        SDL_BlitSurface(Context->textLayer, NULL, Context->DDevice->Screen, NULL);
    #else
        SDL_RenderCopy(Context->DDevice->Renderer, Context->textLayer, NULL, NULL);
    #endif
}