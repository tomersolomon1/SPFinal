/*
 * Menu_windows.h
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */

#ifndef GRAPHICS_WINDOWS_H_
#define GRAPHICS_WINDOWS_H_

#include "SPCommon.h"
#include "Windows_Menu.h"
#include "GameWindow.h"
#include "WindowDataStruct.h"
typedef Button** (*buttons_creator)(SDL_Renderer*);

/*create window:*/
Window* create_window(Window_type type, Gameboard* game);

/*destroy window:*/
void destroyWindow(Window* src);

/*draw window:
 * return 0 if success
 * return -1 if fail, and prints a relevant error message*/
int drawWindow(Window* src, SDL_Event* event);

/*get pointer to the button on the window by its type*/
Button* get_button_by_type(Window* wndw, ButtonType type);

/*helping function for create_window: get num of buttons by window type */
int num_buttons_by_window_type(Window_type type);

/*helping function for create_window: get pointer to button creator by window type */
buttons_creator button_creator_by_window_type(Window_type type);

/*helping function for create_window: get window name by window type */
const char *name_by_window_type(Window_type type);
#endif /* GRAPHICS_WINDOWS_H_ */
