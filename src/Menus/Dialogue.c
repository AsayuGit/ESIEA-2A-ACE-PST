#include "Dialogue.h"

DialogueContext* InitDialog(BitmapFont* Font){
    DialogueContext* DiagContext;
    DiagContext = (DialogueContext*)malloc(sizeof(DialogueContext));
    DiagContext->DialogBox = NULL;
    DiagContext->textLayer = NULL;
    DiagContext->Font = Font;
    DiagContext->DstLetter.x = DiagContext->DialogBoxBounds.x = 5;
    DiagContext->DstLetter.y = DiagContext->DialogBoxBounds.y = 135;
}

void ClearDialogueText(DisplayDevice* DDevice, DialogueContext* Context){
    #ifdef _SDL
        SDL_FillRect(Context->textLayer, NULL, 0x000000);
    #else
        SDL_SetRenderTarget(DDevice->Renderer, Context->textLayer);
        SDL_RenderClear(DDevice->Renderer);
        SDL_SetRenderTarget(DDevice->Renderer, NULL); // Keep or nah ?
    #endif
}

void SetDialogueText(DialogueContext* Context, char* Text){
    Context->Text = Text;
    Context->progress = 0;
    Context->DstLetter.x = Context->DialogBoxBounds.x;
    Context->DstLetter.y = Context->DialogBoxBounds.y;
}

// Fonction non bloquante gérant les dialogues
void Dialogue(DisplayDevice* DDevice, InputDevice* InputDevice, DialogueContext* Context){
    SDL_Rect SrcLetter;
    int letterX, letterY, letterW, letterID;

    // DialogBox Rendering
    if (Context->DialogBox == NULL){
        Context->DialogBox = LoadSurface(ROOT""TEXTURES"Menus"SL"Dialog"TEX_EXT, DDevice, NULL, true);
        if (Context->DialogBox == NULL){
            fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
        }else{
            #ifdef _SDL
                Context->DialogBoxBounds.w = Context->DialogBox->w;
                Context->DialogBoxBounds.h = Context->DialogBox->h;
            #else
                SDL_QueryTexture(Context->DialogBox, NULL, NULL, &(Context->DialogBoxBounds.w), &(Context->DialogBoxBounds.h));
            #endif
        }
    }
    #ifdef _SDL
        SDL_BlitSurface(Context->DialogBox, NULL, DDevice->Screen, NULL);
    #else
        SDL_RenderCopy(DDevice->Renderer, Context->DialogBox, NULL, NULL);
    #endif

    if (Context->textLayer == NULL){
        #ifdef _SDL
            Context->textLayer = CreateEmptySurface(Context->DialogBoxBounds.w, Context->DialogBoxBounds.h);
        #else
            Context->textLayer = SDL_CreateTexture(DDevice->Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Context->DialogBoxBounds.w, Context->DialogBoxBounds.h);
            SDL_SetTextureBlendMode(Context->textLayer, SDL_BLENDMODE_BLEND);
        #endif
    }

    if (Context->Text[Context->progress] != '\0'){
        #ifndef _SDL
            SDL_SetRenderTarget(DDevice->Renderer, Context->textLayer);
        #endif

        // Text processing
        if (Context->Text[Context->progress] != '\n'){
            letterX = 0;
            letterY = 0;
            //printf("Letter is : >%c< >%d<\n", Context->Text[0], Context->Text[0]);
            for (letterID = 0; letterID < Context->Text[Context->progress] - 31; letterX++){
                if (getpixel(Context->Font->FontSurface, letterX, letterY) == 0x0){
                    letterID++;
                }
            }
            letterW = letterX;
            while (letterID < Context->Text[Context->progress] - 30){
                if (getpixel(Context->Font->FontSurface, letterW, letterY) == 0x0){
                    letterID++;
                }
                letterW++;
            }
            letterW -= letterX + 1;
            letterX--;
            letterY = 1;
            while(getpixel(Context->Font->FontSurface, letterX, letterY) != 0x0){
                letterY++;
            }
            //printf("Yay at %d %d %d\n", letterX, letterY, letterW);


            // Text Rendering
            SrcLetter.x = letterX + 1;
            SrcLetter.y = 1;
            SrcLetter.h = Context->DstLetter.h = letterY - 1;
            SrcLetter.w = Context->DstLetter.w = letterW;

            #ifdef _SDL
                SDL_BlitSurface(Context->Font->FontSurface, &SrcLetter, Context->textLayer, &Context->DstLetter);
            #else
                SDL_RenderCopy(DDevice->Renderer, Context->Font->FontTexture, &SrcLetter, &Context->DstLetter);
            #endif
            
            if (Context->DstLetter.x + SrcLetter.w < Context->DialogBoxBounds.w - Context->DialogBoxBounds.x){
                Context->DstLetter.x += SrcLetter.w + 1;
            }else{
                Context->DstLetter.y += SrcLetter.h + 1;
                Context->DstLetter.x = Context->DialogBoxBounds.x;
            }
        }else{
            Context->DstLetter.y += Context->DstLetter.h + 1; // May change for a standard offset in the future
            Context->DstLetter.x = Context->DialogBoxBounds.x;
        }

        //printf("Compute : %d\n", Context->progress);
        Context->progress++;
        #ifndef _SDL
            SDL_SetRenderTarget(DDevice->Renderer, NULL);
        #endif
    }
    #ifdef _SDL
        SDL_BlitSurface(Context->textLayer, NULL, DDevice->Screen, NULL);
    #else
        SDL_RenderCopy(DDevice->Renderer, Context->textLayer, NULL, NULL);
    #endif
}