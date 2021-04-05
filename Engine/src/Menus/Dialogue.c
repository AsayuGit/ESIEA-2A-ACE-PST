#include "Dialogue.h"

void ClearDialogueText(DialogueContext* Context){
    #ifdef _SDL
        SDL_FillRect(Context->textLayer, NULL, 0x000000);
        SDL_FillRect(Context->nameLayer, NULL, 0x000000);
    #else
        SDL_SetRenderTarget(Context->DDevice->Renderer, Context->textLayer);
        SDL_RenderClear(Context->DDevice->Renderer);
        SDL_SetRenderTarget(Context->DDevice->Renderer, Context->nameLayer);
        SDL_RenderClear(Context->DDevice->Renderer);        
        SDL_SetRenderTarget(Context->DDevice->Renderer, NULL);
    #endif
}

int SetDialogueText(DialogueContext* Context, char* Name, char* Text, char SndEffType){
    SDL_Rect NameDest;

    Context->Text = Text;
    Context->progress = 0;
    Context->DstLetter.x = 0;
    Context->DstLetter.y = 0;
    ClearDialogueText(Context);

    #ifndef _SDL
        SDL_SetRenderTarget(Context->DDevice->Renderer, Context->nameLayer);
    #endif
        NameDest.w = gstrlen(Context->NameFont, Name, 1);
        NameDest.x = ((Context->NameBounds.w - Context->NameBounds.x) - NameDest.w) / 2;
        NameDest.h = Context->NameFont->FontHeight;
        NameDest.y = (Context->NameBounds.h - NameDest.h) / 2;
        NameDest.y = 0;
        gprintf(Context->DDevice, Context->NameFont, Name, -1, &NameDest);
    #ifndef _SDL
        SDL_SetRenderTarget(Context->DDevice->Renderer, NULL);
    #endif

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

DialogueContext* InitDialog(DisplayDevice* DDevice, BitmapFont* MainFont, BitmapFont* NameFont){
    
    DialogueContext* DiagContext;
    const int NameMargin = 3;
    const int TextMargin = 10;

    DiagContext = (DialogueContext*)malloc(sizeof(DialogueContext));
    DiagContext->TextSpeed = 40;
    DiagContext->LastLetter = 0;

    DiagContext->DialogBox = NULL;
    DiagContext->DialogBox = LoadSurface(ROOT""TEXTURES"Menus"SL"Dialog"TEX_EXT, DDevice, NULL, true);
    if (DiagContext->DialogBox == NULL){
        fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
    }
    #ifdef _SDL
        DiagContext->DialogBoxBounds.w = DiagContext->DialogBox->w;
        DiagContext->DialogBoxBounds.h = DiagContext->DialogBox->h;
    #else
        SDL_QueryTexture(DiagContext->DialogBox, NULL, NULL, &(DiagContext->DialogBoxBounds.w), &(DiagContext->DialogBoxBounds.h));
    #endif

    DiagContext->DialogBoxBounds.x = 0;
    DiagContext->DialogBoxBounds.y = DDevice->ScreenResolution.y - DiagContext->DialogBoxBounds.h;

    DiagContext->NameBounds.w = 42;
    DiagContext->NameBounds.h = 8;
    DiagContext->NameBounds.x = NameMargin;
    DiagContext->NameBounds.y = DiagContext->DialogBoxBounds.y + NameMargin + 1;

    DiagContext->TextBounds = DiagContext->DialogBoxBounds;
    DiagContext->TextBounds.x += TextMargin;
    DiagContext->TextBounds.y += DiagContext->NameBounds.h + NameMargin + TextMargin - 2;
    DiagContext->TextBounds.w -= (TextMargin << 1); // To inspect
    DiagContext->TextBounds.h -= (DiagContext->NameBounds.h + (TextMargin << 1) + NameMargin + 1);
    

    DiagContext->textLayer = NULL;
    DiagContext->nameLayer = NULL;
    #ifdef _SDL
        DiagContext->textLayer = CreateEmptySurface(DiagContext->TextBounds.w, DiagContext->TextBounds.h);
        DiagContext->nameLayer = CreateEmptySurface(DiagContext->NameBounds.w, DiagContext->NameBounds.h);
    #else
        DiagContext->textLayer = SDL_CreateTexture(DDevice->Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DiagContext->TextBounds.w, DiagContext->TextBounds.h);
        DiagContext->nameLayer = SDL_CreateTexture(DDevice->Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DiagContext->NameBounds.w, DiagContext->NameBounds.h);
        SDL_SetTextureBlendMode(DiagContext->textLayer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureBlendMode(DiagContext->nameLayer, SDL_BLENDMODE_BLEND);
    #endif

    if ((DiagContext->textLayer == NULL) || (DiagContext->nameLayer == NULL)){
        fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
    }

    DiagContext->letterLag = 0;
    DiagContext->Letter = LoadSoundEffect(EffectPath[CHK_Letter]);
    DiagContext->NextLine = LoadSoundEffect(EffectPath[CHK_NextLine]);
    DiagContext->LineComplete = LoadSoundEffect(EffectPath[CHK_LineComplete]);
    Mix_VolumeChunk(DiagContext->Letter, 32);
    Mix_VolumeChunk(DiagContext->NextLine, 32);
    Mix_VolumeChunk(DiagContext->LineComplete, 32);

    DiagContext->MainFont = MainFont;
    DiagContext->NameFont = NameFont;
    DiagContext->DDevice = DDevice;
    SetDialogueText(DiagContext, "", "", 0);
    return DiagContext;
}

// Fonction non bloquante gérant les dialogues
void Dialogue(InputDevice* InputDevice, DialogueContext* Context){
    SDL_Rect InLayerTextBounds;

    InLayerTextBounds = Context->TextBounds;
    InLayerTextBounds.x = InLayerTextBounds.y = 0;

    // DialogBox Rendering
    #ifdef _SDL
        SDL_BlitSurface(Context->DialogBox, NULL, Context->DDevice->Screen, &(Context->DialogBoxBounds));
    #else
        SDL_RenderCopy(Context->DDevice->Renderer, Context->DialogBox, NULL, &(Context->DialogBoxBounds));
    #endif

    // Write text
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
        // Here we use gputc() instead than gprintf() because we want to be able to print out the dialogue character by character
        Context->DstLetter = gputc(Context->DDevice, Context->MainFont, Context->Text[Context->progress], Context->DstLetter.x, Context->DstLetter.y, Context->DstLetter.h + 2, -1, &(InLayerTextBounds));
        Context->progress++;
        Context->LastLetter = SDL_GetTicks();
        #ifndef _SDL
            SDL_SetRenderTarget(Context->DDevice->Renderer, NULL);
        #endif
    }

    #ifdef _SDL
        SDL_BlitSurface(Context->textLayer, NULL, Context->DDevice->Screen, &(Context->TextBounds));
    #else
        SDL_RenderCopy(Context->DDevice->Renderer, Context->textLayer, NULL, &(Context->TextBounds));
        //printf("Name bounds %d %d %d %d\n", Context->NameBounds.x, Context->NameBounds.y, Context->NameBounds.w, Context->NameBounds.h);
        SDL_RenderCopy(Context->DDevice->Renderer, Context->nameLayer, NULL, &(Context->NameBounds));
    #endif
}