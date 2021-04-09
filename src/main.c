#include "include.h"
#include "Scenes.h"
#include "CHAR_Index.h"

int main(int argc, char* argv[]){

    DisplayDevice* DDevice;
    InputDevice* IDevice;
    /*SoundDevice* SDevice; */

    BitmapFont* MainFont;
    BitmapFont* NameFont;
    BitmapFont* MainMenuFont;
    Uint32 FontColorKey = 0xff00ff;
    CourtroomContext* CourtContext;

    CourtContext = (CourtroomContext*)malloc(sizeof(CourtroomContext));

    InitSDL();
    DDevice = CreateDisplayDevice(256, 192, "Mia's Legacy");
    IDevice = InitInputs(false);
    /*SDevice = CreateSoundDevice();  */ /* Always return null so we don't need to store it for now */
    CreateSoundDevice();

    MainFont = LoadBitmapFont(ROOT""FONTS"AceAttorneyFont"TEX_EXT, DDevice, FontColorKey);
    NameFont = LoadBitmapFont(ROOT""FONTS"NameFont"TEX_EXT, DDevice, FontColorKey);
    MainMenuFont = LoadBitmapFont(ROOT""FONTS"MainMenuFont"TEX_EXT, DDevice, FontColorKey);
    CourtContext->MainFont = MainFont;
    CourtContext->NameFont = NameFont;
    CourtContext->ButtonFont = MainMenuFont;

    Characters* CharactersIndex[CharactersCount] = {
        &CHAR_PhoenixWright,
        &CHAR_MiaFey,
        &CHAR_MilesEdgeworth,
        &CHAR_Judge,
        &CHAR_WinstonPayne,
        &CHAR_Desk
    };

    /* First we start the title screen */
    if (Scene_TitleScreen(DDevice, IDevice, MainMenuFont) == 0){
        /* We start the courtroom Scene */
        Scene_Courtroom(DDevice, IDevice, CourtContext, CharactersIndex, CharactersCount);
    }
    
    free(CourtContext);

    free(DDevice);

    return 0;
}