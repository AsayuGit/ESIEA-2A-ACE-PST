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

#ifndef _UI_H
#define _UI_H

    #include "include.h"
    #include "CourtRecord.h"
    #include "SceneCommon.h"

    enum {
        UI_EMPTY = -1,
        UI_LIVES,
        TESTIMONY_ICON,
        CROSS_EXAMINATION,
        SHOW_ITEM_TO_COURTL,
        SHOW_ITEM_TO_COURTR,
        HOLD_IT,
        OBJECTION,
        TAKE_THAT
    };

    void InitUI(DisplayDevice* DDevice, Items* UIItemBankPointer, SceneContext* SContext);
    void setUI(unsigned int notifType, unsigned int argument);
    void DrawUI(DisplayDevice* DDevice, InputDevice* IDevice);

#endif