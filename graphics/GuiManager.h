/*
 * Menu_windowsTester.h
 *
 *  Created on: Aug 28, 2017
 *      Author: sapir
 */

#ifndef MENU_WINDOWSTESTER_H_
#define MENU_WINDOWSTESTER_H_
#include "Windows.h"
#include "GuiManager_Menu.h"
#include "GuiManager_Game.h"

#define GAME_DEFAULT_DIFFICULTY 1
#define GAME_DEFAULT_MODE 1
#define GAME_DEFAULT_COLOR white

void run_gui();
void set_buttons_by_game_params(Window* wndw, Gameboard** game);
Window_type handleEvenet(Window* wndw, Gameboard** game, Window_type old_type_window);

#endif /* MENU_WINDOWSTESTER_H_ */
