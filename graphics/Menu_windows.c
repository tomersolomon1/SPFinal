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
	if(src == NULL) return NULL;
	src->type = type;

	SDL_Window* window = SDL_CreateWindow(window_name[type], SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, DEFAULT_MENU_WINDOW_WIDTH, DEFAULT_MENU_WINDOW_HIGHT, SDL_WINDOW_OPENGL);
	if(window == NULL) return NULL;
	src->window = window;

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL) return NULL;
	src->windowRenderer = renderer;

	src->num_buttons = num_buttons[type];

	Button** buttons;

		void (*drawWindow)(MenuWindow*);
		void (*handleEventWindow)(MenuWindow* , SDL_Event* );

	return src;
}

Button** crate_enterance_buttons(SDL_Renderer* renderer){
	Button** buttons = malloc(sizeof(Button*) * 3);
	if(buttons == NULL) return NULL;
	int gap_window = 100;
	int i = 0;
	SDL_Rect new_game = {.x = 75, .y = gap_window + DEFAULT_BTN_GAP*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	SDL_Rect load = {.x = 75, .y = gap_window + DEFAULT_BTN_GAP*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	SDL_Rect exit = {.x = 75, .y = gap_window + DEFAULT_BTN_GAP*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};

	buttons[0] = createButton(renderer, new_game, "./images/new_game.bmp",
			"./images/new_game_inactive.bmp", NewGameButton, true, true);
	buttons[1] = createButton(renderer, load, "./images/load.bmp",
				"./images/load_inactive.bmp", LoadButton, true, true);
	buttons[2] = createButton(renderer, exit, "./images/exit.bmp",
					"./images/exit_inactive.bmp", ExitButton, true, true);
	if(buttons[0] == NULL || buttons[1] == NULL || buttons[2] == NULL ){
		destroyButton(buttons[0]);
		destroyButton(buttons[1]);
		destroyButton(buttons[2]);
		free(buttons);
		return NULL;
	}
	return buttons;
}

Button** crate_enterance_buttons(SDL_Renderer* renderer){
	Button** buttons = malloc(sizeof(Button*) * 3);
	if(buttons == NULL) return NULL;
	int i = 0;
	SDL_Rect new_game = {.x = DEFAULT_GAP_WINDOW, .y = DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	SDL_Rect load = {.x = DEFAULT_GAP_WINDOW, .y = DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	SDL_Rect exit = {.x = DEFAULT_GAP_WINDOW, .y = DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};

	buttons[0] = createButton(renderer, new_game, "./images/new_game.bmp",
			"./images/new_game_inactive.bmp", NewGameButton, true, true);
	buttons[1] = createButton(renderer, load, "./images/load.bmp",
				"./images/load_inactive.bmp", LoadButton, true, true);
	buttons[2] = createButton(renderer, exit, "./images/exit.bmp",
					"./images/exit_inactive.bmp", ExitButton, true, true);
	if(buttons[0] == NULL || buttons[1] == NULL || buttons[2] == NULL ){
		destroyButton(buttons[0]);
		destroyButton(buttons[1]);
		destroyButton(buttons[2]);
		free(buttons);
		return NULL;
	}
	return buttons;
}

Button** create_load_game_buttons(SDL_Renderer* renderer){
	Button** buttons = malloc(sizeof(Button*) * 3);
	if(buttons == NULL) return NULL;
	int i = 0;
	SDL_Rect slot1 = {.x = DEFAULT_GAP_WINDOW, .y = DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	SDL_Rect slot2 = {.x = DEFAULT_GAP_WINDOW, .y = DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	SDL_Rect slot3 = {.x = DEFAULT_GAP_WINDOW, .y = DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	SDL_Rect slot4 = {.x = DEFAULT_GAP_WINDOW, .y = DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	SDL_Rect slot5 = {.x = DEFAULT_GAP_WINDOW, .y = DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL*(i++), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
	SDL_Rect back = {.x = DEFAULT_GAP_WINDOW, .y = DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL*(3*i), .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};

	buttons[0] = createButton(renderer, new_game, "./images/new_game.bmp",
			"./images/new_game_inactive.bmp", NewGameButton, true, true);
	buttons[1] = createButton(renderer, load, "./images/load.bmp",
				"./images/load_inactive.bmp", LoadButton, true, true);
	buttons[2] = createButton(renderer, exit, "./images/exit.bmp",
					"./images/exit_inactive.bmp", ExitButton, true, true);
	if(buttons[0] == NULL || buttons[1] == NULL || buttons[2] == NULL ){
		destroyButton(buttons[0]);
		destroyButton(buttons[1]);
		destroyButton(buttons[2]);
		free(buttons);
		return NULL;
	}
	return buttons;
}

Button** create_game_mode_buttons(SDL_Renderer* renderer){}

Button** create_difficulty_buttons(SDL_Renderer* renderer){}

Button** create_choose_color_buttons(SDL_Renderer* renderer){}
