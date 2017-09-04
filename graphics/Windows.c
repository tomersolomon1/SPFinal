/*
 * Menu_windows.c
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */

#include "Windows.h"

Window* create_window(Window_type type, Gameboard* game){
	Window* src = (Window*) malloc(sizeof(Window));
	assert(src != NULL);
	src->type = type;
	SDL_Window* window = SDL_CreateWindow(name_by_window_type(type), SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, DEFAULT_MENU_WINDOW_WIDTH, DEFAULT_MENU_WINDOW_HIGHT, SDL_WINDOW_OPENGL);
	src->window = window;
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	src->windowRenderer = renderer;

	if(window == NULL || renderer == NULL){
		free(src);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		return NULL;
	}
	src->num_buttons = num_buttons_by_window_type(type);
	src->data = NULL;
	if(type == Game){
		src->data = create_game_data(renderer, game);
		if(src->data == NULL){
			free(src);
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);
		}
	}
	src->buttons = (*(button_creator_by_window_type(type)))(renderer);
	if(src->buttons == NULL){
		free(src);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		return NULL;
	}
	return src;
}

void destroyWindow(Window* src) {
	if (src == NULL ) {
		return;
	}
	int i = 0;
	for (; i < src->num_buttons; i++) {
		destroyButton(src->buttons[i]);
	}
	if(src->type == Game){
		//destroyData(src->data);
	}
	SDL_DestroyRenderer(src->windowRenderer);
	SDL_DestroyWindow(src->window);
	free(src->buttons);
	free(src);
}

void drawWindow(Window* src, SDL_Event* event) {
	if (src == NULL ){
		return;
	}
	//draw window:
	SDL_SetRenderDrawColor(src->windowRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->windowRenderer);
	if(src->type == Game){
		draw_board(src->data, src->windowRenderer, event);
	}
	for (int i = 0; i < src->num_buttons; i++) {
		drawButton(src->buttons[i]);
	}
	SDL_RenderPresent(src->windowRenderer);
}

Button *get_button_by_type(Window* wndw, ButtonType type){
	for(int i = 0; i < wndw->num_buttons; i++){
		if(wndw->buttons[i]->type == type)
			return wndw->buttons[i];
	}
	return NoButton;
}

int num_buttons_by_window_type(Window_type type){
	switch(type){
	case Enterance:
		return AMOUNT_BUTTONS_ENTERANCE;
	case LoadGame:
		return AMOUNT_BUTTONS_LOAD_GAME;
	case ModeGame:
		return AMOUNT_BUTTONS_GAME_MODE;
	case Difficulty:
		return AMOUNT_BUTTONS_DIFFICULTY;
	case ChooseColor:
		return AMOUNT_BUTTONS_CHOOSE_COLOR;
	case Game:
		return AMOUNT_BUTTONS_GAME;
	default:
		return 0;
	}
}

buttons_creator button_creator_by_window_type(Window_type type){
	switch(type){
		case Enterance:
			return create_enterance_buttons;
		case LoadGame:
			return create_load_game_buttons;
		case ModeGame:
			return create_game_mode_buttons;
		case Difficulty:
			return create_difficulty_buttons;
		case ChooseColor:
			return create_choose_color_buttons;
		case Game:
			return create_game_buttons;
		default:
			return NULL;
		}
}

const char *name_by_window_type(Window_type type){
	switch(type){
		case Enterance:
			return "Chess: Main Menu";
		case LoadGame:
			return "Chess: Load Game";
		case ModeGame:
			return "Chess: Game Mode";
		case Difficulty:
			return "Chess: Difficulty";
		case ChooseColor:
			return "Chess: Choose Color";
		case Game:
			return "Chess: Game";
		default:
			return NULL;
		}
}
