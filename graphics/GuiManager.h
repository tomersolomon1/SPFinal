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

void check_menu_window();
void set_buttons_by_game_params(Window* wndw, Gameboard** game);
Window_type handleEvenet(Window* wndw, Gameboard** game, Window_type old_type_window);

#endif /* MENU_WINDOWSTESTER_H_ */
