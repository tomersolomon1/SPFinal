/*
 * Menu_windows.h
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */

#ifndef GRAPHICS_MENU_WINDOWS_H_
#define GRAPHICS_MENU_WINDOWS_H_
#include "Button.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "../GameBoard.h"
#include "SPCommon.h"
#include "Window.h"
#include "GameWindow.h"


typedef Button** (*buttons_creator)(SDL_Renderer*);

Window* create_window(Window_type type, Gameboard* game);
void set_buttons_by_game_params(Window* wndw, Gameboard** game);
Button** create_enterance_buttons(SDL_Renderer* renderer);
Button** create_load_game_buttons(SDL_Renderer* renderer);
Button** create_game_mode_buttons(SDL_Renderer* renderer);
Button** create_difficulty_buttons(SDL_Renderer* renderer);
Button** create_choose_color_buttons(SDL_Renderer* renderer);
void destroyWindow(Window* src);
void drawWindow(Window* src, SDL_Event* event);
Button* get_button_by_type(Window* wndw, ButtonType type);
Window_type handleEvenet(Window* wndw, Gameboard** game);
Window_type handleEvenet_enterance(Window* wndw, Button* btn);
Window_type handleEvenet_load_game(Window* wndw, Button* btn, Gameboard** game);
Window_type handleEvenet_mode_game(Window* wndw, Button* btn, Gameboard** game);
Window_type handleEvenet_difficulty(Window* wndw, Button* btn, Gameboard** game);
Window_type handleEvenet_choose_color(Window* wndw, Button* btn, Gameboard** game);
#endif /* GRAPHICS_MENU_WINDOWS_H_ */
