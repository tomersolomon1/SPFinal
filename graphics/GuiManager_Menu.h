/*
 * GuiManager_Menu.h
 *
 *  Created on: Sep 2, 2017
 *      Author: sapir
 */

#ifndef GRAPHICS_GUIMANAGER_MENU_H_
#define GRAPHICS_GUIMANAGER_MENU_H_
#include "Windows.h"

Window_type handleEvenet_enterance(Window* wndw, Button* btn);
Window_type handleEvenet_load_game(Window* wndw, Button* btn, Gameboard** game);
Window_type handleEvenet_mode_game(Window* wndw, Button* btn, Gameboard** game);
Window_type handleEvenet_difficulty(Window* wndw, Button* btn, Gameboard** game);
Window_type handleEvenet_choose_color(Window* wndw, Button* btn, Gameboard** game);

#endif /* GRAPHICS_GUIMANAGER_MENU_H_ */
