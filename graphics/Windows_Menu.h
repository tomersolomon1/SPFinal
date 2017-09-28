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
#include "WindowDataStruct.h"

#define AMOUNT_BUTTONS_ENTERANCE 3
#define AMOUNT_BUTTONS_LOAD_GAME 7
#define AMOUNT_BUTTONS_GAME_MODE 5
#define AMOUNT_BUTTONS_DIFFICULTY 6
#define AMOUNT_BUTTONS_CHOOSE_COLOR 4

/* create buttons to "enterance" window
 * sets all parameters and create the buttons
 * returns NULL on failure */
Button** create_enterance_buttons(SDL_Renderer* renderer);

/* create buttons to "load game" window
 * sets all parameters and create the buttons
 * returns NULL on failure */
Button** create_load_game_buttons(SDL_Renderer* renderer);

/* create buttons to "game mode" window
 * sets all parameters and create the buttons
 * returns NULL on failure */
Button** create_game_mode_buttons(SDL_Renderer* renderer);

/* create buttons to "difficulty" window
 * sets all parameters and create the buttons
 * returns NULL on failure */
Button** create_difficulty_buttons(SDL_Renderer* renderer);

/* create buttons to "choose color" window
 * sets all parameters and create the buttons
 * returns NULL on failure */
Button** create_choose_color_buttons(SDL_Renderer* renderer);

#endif /* GRAPHICS_WINDOWS_MENU_H_ */
