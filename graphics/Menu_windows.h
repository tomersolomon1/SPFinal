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
#include "GameWindow.h"

typedef enum menu_window_type_t{
	Enterance,
	LoadGame,
	ModeGame,
	Difficulty,
	ChooseColor,
	Game,
	ExitGame
} menu_window_type;

typedef struct window_t{
	menu_window_type type;
	SDL_Window* window;
	SDL_Renderer* windowRenderer;
	int num_buttons;
	Button** buttons;
	BoardWidget* data;
} Window;

typedef Button** (*buttons_creator)(SDL_Renderer*);

Window* create_window(menu_window_type type);
Button** create_enterance_buttons(SDL_Renderer* renderer);
Button** create_load_game_buttons(SDL_Renderer* renderer);
Button** create_game_mode_buttons(SDL_Renderer* renderer);
Button** create_difficulty_buttons(SDL_Renderer* renderer);
Button** create_choose_color_buttons(SDL_Renderer* renderer);
void destroyWindow(Window* src);
void drawWindow(Window* src);
Button* get_button_by_type(Window* wndw, ButtonType type);
menu_window_type handleEvenet(Window* wndw, Gameboard** game);
menu_window_type handleEvenet_enterance(Window* wndw, Button* btn);
menu_window_type handleEvenet_load_game(Window* wndw, Button* btn, Gameboard** game);
menu_window_type handleEvenet_mode_game(Window* wndw, Button* btn, Gameboard** game);
menu_window_type handleEvenet_difficulty(Window* wndw, Button* btn, Gameboard** game);
menu_window_type handleEvenet_choose_color(Window* wndw, Button* btn, Gameboard** game);
#endif /* GRAPHICS_MENU_WINDOWS_H_ */
