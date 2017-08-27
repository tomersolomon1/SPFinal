/*
 * Menu_windows.c
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */
#include "Menu_windows.h"
const char* window_name[] = {"Enterance","LoadGame","GameMode","Difficulty","ChooseColor"};
int num_buttons[] = {3,7,4,7,4};

MenuWindow* create_menu_window(menu_window_type type){
	MenuWindow* src = (MenuWindow*) malloc(sizeof(MenuWindow));
	src->type = type;

	SDL_Window* window = SDL_CreateWindow(window_name[type], SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 700, 700, SDL_WINDOW_OPENGL);
	assert(window != NULL);
	src->window = window;

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	assert(renderer != NULL);
	src->windowRenderer = renderer;

	src->num_buttons = num_buttons[type];

	Button** buttons;

		void (*drawWindow)(MenuWindow*);
		void (*handleEventWindow)(MenuWindow* , SDL_Event* );

	return src;
}

Button** crate_enterance_buttons(SDL_Renderer* renderer){
	Button** buttons = malloc(sizeof(Button*) * 3);
	assert(buttons != NULL);
	int position_buttons = 100;
	int i = 0;
	SDL_Rect new_game = {.x = 75, .y = position_buttons + DEFAULT_BTN_GAP*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	SDL_Rect load = {.x = 75, .y = position_buttons + DEFAULT_BTN_GAP*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	SDL_Rect new_game = {.x = 75, .y = position_buttons + DEFAULT_BTN_GAP*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	buttons[0] = createButton(renderer, new_game, "./images/new_game.bmp",
			"./images/inactivenew_game.bmp", NewGameButton, true, true);
	buttons[1] = createButton(renderer, new_game, "./images/new_game.bmp",
				"./images/inactivenew_game.bmp", NewGameButton, true, true);
}
