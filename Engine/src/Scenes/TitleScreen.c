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

#include "TitleScreen.h"
#include "SceneCommon.h"
#include "Buttons.h"

/* CourtContext->ButtonFont */

int Scene_TitleScreen(DisplayDevice* DDevice, InputDevice* IDevice){
    int ReturnValue;
    BackgroundContext* SContext;
    ButtonsContext* BContext;
    BitmapFont* Font;

    int Slide;
    char Menu;

    Mix_Chunk* ToMenu;
	
	#ifdef _XBOX
		SContext = InitBackground(DDevice, "D:\\Assets\\Anim\\TitleScreen.xml");
	#else
		SContext = InitBackground(DDevice, "Assets/Anim/TitleScreen.xml");
	#endif

    Font = LoadBitmapFont(ROOT""FONTS"MainMenuFont"TEX_EXT, DDevice, 0xff00ff);
    BContext = InitButtons(DDevice, SContext, Font, 148, NULL);

    AddButton(BContext, "New Game");
    /* AddButton(BContext, "Continue"); When the save system is implemented */
    AddButton(BContext, "Back");

    MoveButtonsToCoordinates(BContext, BASE_RESOLUTION_X, 0);
    SetButtonClkSndEffect(BContext, 0, CHK_Gravel);
    SetButtonClkSndEffect(BContext, 1, CHK_ButtonBack);

    Slide = 0; /* Which side of the slide we're on */
    Menu = 0; /* Wether the menu should be activated or not */

    ToMenu = LoadSoundEffect(EffectPath[CHK_ButtonClicked]);

    while (1){
        /* Events Loop */
        while(SDL_PollEvent(&IDevice->event)){
            SystemEvents(DDevice, IDevice);

            if (Menu)
                HandleButtonsEvents(BContext, IDevice);
            switch (IDevice->event.type)
            {
            case SDL_QUIT:
                ReturnValue = -1;
                goto Exit;
                break;

            case PAD_KEYDOWN:
                if (!IDevice->event.key.repeat){
                    switch (IDevice->event.PADKEY)
                    {
                    case PAD_SELECT:
                        if (Slide == 0){
                            Mix_PlayChannel(-1, ToMenu, 0);
                            SetSlkdButtonID(BContext, 0);
                            BackgroundPlayAnimation(SContext, 0, &IDevice->EventEnabled, true);
                            Slide = 1;
                        } else {
                            switch (GetClkdButtonID(BContext))
                            {
                            case 0:
                                ReturnValue = 0;
                                goto Exit;
                                break;
                            case 1:
                                BackgroundPlayAnimation(SContext, 1, &IDevice->EventEnabled, true);
                                Slide = 0;
                                Menu = 0;
                                break;
                            
                            default:
                                break;
                            }
                        }
                        break;
                    
                    default:
                        break;
                    }
                }
                break;
            default:
                break;
            }
            
        }
/*		
		SDL_JoystickUpdate();
		if (SDL_JoystickGetButton(IDevice->Joy1, 0)){
			goto PRESS;
        }
*/

        if (IDevice->EventEnabled){
            Menu = Slide;
        }

        /* Rendering */
        DisplayBackground(DDevice, SContext); /* Background */
        DrawButtons(BContext);
        
        FinishFrame(DDevice);
    }

Exit:
    if (SContext)
        FreeBackground(SContext);

    return ReturnValue;
}