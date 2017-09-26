/*
 * GuiManager_Menu.h
 *
 *  Created on: Sep 2, 2017
 *      Author: sapir
 */

#ifndef GRAPHICS_GUIMANAGER_MENU_H_
#define GRAPHICS_GUIMANAGER_MENU_H_
#include "Windows.h"

/* handle events on enterance window
 * returns the next window type to be opened */
Window_type handleEvenet_enterance(Window* wndw, Button* btn);

/* handle events on load game window
 * returns the next window type to be opened
 * if a problem occurred with opening the game file,
 * the function prints that there was a problem,
 * and returns LoadGame so the user could try again*/
Window_type handleEvenet_load_game(Window* wndw, Button* btn, Gameboard** game, Window_type old_type_window);

/* handle events on mode game window
 * returns the next window type to be opened */
Window_type handleEvenet_mode_game(Window* wndw, Button* btn, Gameboard** game);

/* handle events on difficulty window
 * returns the next window type to be opened */
Window_type handleEvenet_difficulty(Window* wndw, Button* btn, Gameboard** game);

/* handle events on choose color window
 * returns the next window type to be opened */
Window_type handleEvenet_choose_color(Window* wndw, Button* btn, Gameboard** game);

#endif /* GRAPHICS_GUIMANAGER_MENU_H_ */
