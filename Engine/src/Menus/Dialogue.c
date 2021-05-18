#include "Dialogue.h"
#include "math.h"

#define LETTER_VOLUME 20

void ClearDialogueText(DialogueContext* Context){
    #ifdef _SDL
        SDL_FillRect(Context->textLayer, NULL, 0x000000);
        SDL_FillRect(Context->nameLayer, NULL, 0x000000);
    #else
        #ifndef _XBOX
			SDL_SetRenderTarget(Context->DDevice->Renderer, Context->textLayer);
			SDL_RenderClear(Context->DDevice->Renderer);
			SDL_SetRenderTarget(Context->DDevice->Renderer, Context->nameLayer);
			SDL_RenderClear(Context->DDevice->Renderer);        
			SDL_SetRenderTarget(Context->DDevice->Renderer, NULL);
		#endif
    #endif
}

size_t SetDialogueText(DialogueContext* Context, char* Name, char* Text, char SndEffType){
    SDL_Rect NameDest;

    Context->Text = Text;
    Context->progress = 0;
    Context->DstLetter.x = 0;
    Context->DstLetter.y = 0;
    ClearDialogueText(Context);
	
	#ifndef _XBOX
		ACE_SetRenderTarget(Context->DDevice, Context->nameLayer);
	#endif
    NameDest.w = gstrlen(Context->NameFont, Name, 1).x;
    NameDest.x = (Context->NameBounds.w - NameDest.w) >> 1;
    NameDest.h = Context->NameFont->Rects[0].h;
    NameDest.y = (Context->NameBounds.h - NameDest.h) >> 1;
    NameDest.y = 0;
    gprintf(Context->DDevice, Context->NameFont, Name, 1, &NameDest);
    #ifndef _XBOX
		ACE_SetRenderTarget(Context->DDevice, NULL);
	#endif

    switch (SndEffType){
        case 0:
            /* Nothing */
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
    DiagContext->letterLag = 0;

    DiagContext->DialogBox = NULL;
    DiagContext->DialogBox = LoadSurface(ROOT""TEXTURES"Menus"SL"Dialog"TEX_EXT, DDevice, 0x0, SURFACE_ALPHA);
    if (DiagContext->DialogBox == NULL){
        fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
    }
    
    DiagContext->DialogBoxSrcBounds = InitRect(0, 0, 256, 76);

    DiagContext->DialogBoxBounds.w = DiagContext->DialogBoxSrcBounds.w;
    DiagContext->DialogBoxBounds.h = DiagContext->DialogBoxSrcBounds.h;

    DiagContext->DialogBoxBounds.x = 0;
    DiagContext->DialogBoxBounds.y = BASE_RESOLUTION_Y - DiagContext->DialogBoxBounds.h;

    DiagContext->NameBounds.w = 42;
    DiagContext->NameBounds.h = 8;
    DiagContext->NameBounds.x = NameMargin;
    DiagContext->NameBounds.y = DiagContext->DialogBoxBounds.y + ((NameMargin + 1));

    DiagContext->TextBounds = DiagContext->DialogBoxBounds;
    DiagContext->TextBounds.x += TextMargin;
    DiagContext->TextBounds.y += DiagContext->NameBounds.h + (NameMargin + TextMargin - 2);
    DiagContext->TextBounds.w -= (TextMargin << 1); /* To inspect */
    DiagContext->TextBounds.h -= (DiagContext->NameBounds.h + (TextMargin << 1) + NameMargin + 1);
    
    DiagContext->ArrowDstRect = InitRect(0, 172, 9, 9);

    DiagContext->textLayer = NULL;
    DiagContext->nameLayer = NULL;
    #ifdef _SDL
        DiagContext->textLayer = CreateEmptySurface(SDL_HWSURFACE, DiagContext->TextBounds.w, DiagContext->TextBounds.h);
        DiagContext->nameLayer = CreateEmptySurface(SDL_HWSURFACE, DiagContext->NameBounds.w, DiagContext->NameBounds.h);
    #else
        DiagContext->textLayer = SDL_CreateTexture(DDevice->Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DiagContext->TextBounds.w, DiagContext->TextBounds.h);
        DiagContext->nameLayer = SDL_CreateTexture(DDevice->Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DiagContext->NameBounds.w, DiagContext->NameBounds.h);
        SDL_SetTextureBlendMode(DiagContext->textLayer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureBlendMode(DiagContext->nameLayer, SDL_BLENDMODE_BLEND);
    #endif

    if ((DiagContext->textLayer == NULL) || (DiagContext->nameLayer == NULL)){
        fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
    }

    /* SFXs */
    DiagContext->Letter = LoadSoundEffect(EffectPath[CHK_Letter]);
    DiagContext->NextLine = LoadSoundEffect(EffectPath[CHK_NextLine]);
    DiagContext->LineComplete = LoadSoundEffect(EffectPath[CHK_LineComplete]);

    Mix_VolumeChunk(DiagContext->Letter, LETTER_VOLUME);
    Mix_VolumeChunk(DiagContext->NextLine, 32);
    Mix_VolumeChunk(DiagContext->LineComplete, 32);

    /* Fonts */
    DiagContext->MainFont = MainFont;
    DiagContext->NameFont = NameFont;
    DiagContext->DDevice = DDevice;

    DiagContext->DstLetter.h = MainFont->Rects[0].h;

    SetDialogueText(DiagContext, "", "", 0);
    return DiagContext;
}

/* Fonction non bloquante gérant les dialogues */
void Dialogue(DialogueContext* Context, bool bothWay){
    const SDL_Rect ArrowSrcRect[2] = {{256, 0, 9, 9}, {256, 9, 9, 9}};
    SDL_Rect InLayerTextBounds;
    double Wobble;

    InLayerTextBounds = Context->TextBounds;
    InLayerTextBounds.x = InLayerTextBounds.y = 0;

    Wobble = sin((double)SDL_GetTicks() / 50.0f) * 2.0f;

    /* DialogBox Rendering */
    ScaledDraw(Context->DDevice, Context->DialogBox, &(Context->DialogBoxSrcBounds), &(Context->DialogBoxBounds));

    /* Write text */
    if (Context->Text[Context->progress] != '\0') {
        if (SDL_GetTicks() >= Context->LastLetter + Context->TextSpeed){
			#ifndef _XBOX
				ACE_SetRenderTarget(Context->DDevice, Context->textLayer);
			#endif
            if (!Context->letterLag){
                Mix_PlayChannel(-1, Context->Letter, 0);
                Context->letterLag = 1;
            }else{
                Context->letterLag--;
            }

            /* Print the next letter */
            if ((Context->DstLetter.x + Context->MainFont->Rects[MAX(Context->Text[Context->progress] - 32, 0)].w < InLayerTextBounds.x + InLayerTextBounds.w) && (Context->Text[Context->progress] != '\n')){
                Context->DstLetter.x += gputc(Context->DDevice, Context->MainFont, Context->Text[Context->progress], Context->DstLetter.x, Context->DstLetter.y) + 2;
            } else {
                Context->DstLetter.x = InLayerTextBounds.x;
                Context->DstLetter.y += Context->MainFont->Rects[0].h;
            }
            
            Context->progress++;
            Context->LastLetter = SDL_GetTicks();
			#ifndef _XBOX
				ACE_SetRenderTarget(Context->DDevice, NULL);
			#endif
        }
    } else {
        if (bothWay){
            Context->ArrowDstRect.x = (int)(7 - Wobble);
            ScaledDraw(Context->DDevice, Context->DialogBox, &ArrowSrcRect[1], &Context->ArrowDstRect); /* Right Arrow */
        }
        
        Context->ArrowDstRect.x = (int)(240 + Wobble);
        ScaledDraw(Context->DDevice, Context->DialogBox, &ArrowSrcRect[0], &Context->ArrowDstRect); /* Right Arrow */
    }

    ScaledDraw(Context->DDevice, Context->textLayer, NULL, &(Context->TextBounds));
    ScaledDraw(Context->DDevice, Context->nameLayer, NULL, &(Context->NameBounds));
}