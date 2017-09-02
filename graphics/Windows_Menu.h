/*
 * MenuWindowsCreate.h
 *
 *  Created on: Sep 2, 2017
 *      Author: sapir
 */

#ifndef GRAPHICS_WINDOWS_MENU_H_
#define GRAPHICS_WINDOWS_MENU_H_
#include "SPCommon.h"
#include "Button.h"

Button** create_enterance_buttons(SDL_Renderer* renderer);
Button** create_load_game_buttons(SDL_Renderer* renderer);
Button** create_game_mode_buttons(SDL_Renderer* renderer);
Button** create_difficulty_buttons(SDL_Renderer* renderer);
Button** create_choose_color_buttons(SDL_Renderer* renderer);

#endif /* GRAPHICS_WINDOWS_MENU_H_ */
