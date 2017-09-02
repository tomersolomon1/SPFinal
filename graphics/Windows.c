/*
 * Menu_windows.c
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */

#include "Windows.h"


const char* window_name[] = {"Chess: Main Menu","Chess: Load Game","Chess: Game Mode","Chess: Difficulty","Chess: Choose Color"};
int num_buttons[] = {3,7,5,6,4,6,0};
buttons_creator creators[] = {create_enterance_buttons, create_load_game_buttons, create_game_mode_buttons,
				create_difficulty_buttons, create_choose_color_buttons, create_game_buttons};
Window*  create_window(Window_type type, Gameboard* game){
	Window* src = (Window*) malloc(sizeof(Window));
	src->type = type;
	SDL_Window* window = SDL_CreateWindow(window_name[type], SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, DEFAULT_MENU_WINDOW_WIDTH, DEFAULT_MENU_WINDOW_HIGHT, SDL_WINDOW_OPENGL);
	src->window = window;
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	src->windowRenderer = renderer;

	if(src == NULL || window == NULL || renderer == NULL){
		free(src);
		free(window);
		free(renderer);
		return NULL;
	}
	src->num_buttons = num_buttons[type];
	src->data = NULL;
	if(type == Game){
		src->data = create_game_data(renderer, game);
	}
	src->buttons = (*creators[type])(renderer);
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
	if (src == NULL ) {
		return;
	}
	//draw window:
	SDL_SetRenderDrawColor(src->windowRenderer, 55, 10, 100, 120);
	SDL_RenderClear(src->windowRenderer);
	if(src->type == Game){
		draw_board(src->data, src->windowRenderer, event);
	}
	for (int i = 0; i < src->num_buttons; i++) {
		drawButton(src->buttons[i]);
	}

	SDL_RenderPresent(src->windowRenderer);
}

Button* get_button_by_type(Window* wndw, ButtonType type){
	for(int i = 0; i < wndw->num_buttons; i++){
		if(wndw->buttons[i]->type == type){
			return wndw->buttons[i];
		}
	}
	return NoButton;
}

