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

/* run the gui while the user didn't ask to quit
 * if a problem occurred with SDL during the running,
 * the function destroys the game and closes it*/
void run_gui();

/* initialize the window and handle events while the window is open
 * in case of problem returns SDLErrorWindow
 * otherwise returns the Window_type of the next window should be open */
Window_type handleEvenet(Window* wndw, Gameboard** game, Window_type old_type_window);

/* helping function for handleEvent
 * initialize wndw's buttons state by the game state */
void set_buttons_by_game_params(Window* wndw, Gameboard** game);


#endif /* MENU_WINDOWSTESTER_H_ */
