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

Window* create_window(Window_type type, Gameboard* game);
void destroyWindow(Window* src);
void drawWindow(Window* src, SDL_Event* event);
Button* get_button_by_type(Window* wndw, ButtonType type);

#endif /* GRAPHICS_WINDOWS_H_ */
