/*
 * Menu_windows.c
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */
#include "Menu_windows.h"
const char* window_name[] = {"Enterance","LoadGame","GameMode","Difficulty","ChooseColor"};
int num_buttons[] = {3,7,5,7,4};

MenuWindow* create_menu_window(menu_window_type type){
	MenuWindow* src = (MenuWindow*) malloc(sizeof(MenuWindow));
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

	Button** buttons;
	if(type == Enterance){
		buttons = create_enterance_buttons(renderer);
	}
	else if(type == LoadGame){
		buttons = create_load_game_buttons(renderer);
	}
	else if(type == ModeGame){
		buttons = create_game_mode_buttons(renderer);
	}
	else if(type == Difficulty){
		buttons = create_difficulty_buttons(renderer);
	}
	else if(type == ChooseColor){
		buttons = create_choose_color_buttons(renderer);
	}

	if(buttons == NULL){
		free(src);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		return NULL;
	}
	return src;
}

Button** create_enterance_buttons(SDL_Renderer* renderer){
	Button** buttons = malloc(sizeof(Button*) * 3);
	if(buttons == NULL) return NULL;
	int horiz_center = (DEFAULT_MENU_WINDOW_WIDTH - DEFAULT_BTN_WIDTH) / 2;
	//set all parameters
	int x_btn_places[] = {horiz_center, horiz_center, horiz_center};
	int y_btn_places[] = {DEFAULT_GAP_WINDOW, DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL, DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 2};
	ButtonType types[] = {NewGameButton, LoadButton, ExitButton};
	const char* image[] = {IMG(new_game), IMG(load), IMG(exit)};
	const char* image_inavtice[] = {IMG_INCTV(new_game), IMG_INCTV(load), IMG_INCTV(exit)};
	//create buttons:
	for(int i = 0; i < 3; i++){
		SDL_Rect Rec = {.x = x_btn_places[i], .y = y_btn_places[i], .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
		buttons[i] = createButton(renderer, &Rec, image[i], image_inavtice[i], types[i], true, true);
		if(buttons[i] == NULL){
			for(int j = 0; j < i; j++)
				destroyButton(buttons[j]);
			free(buttons);
			return NULL;
		}
	}

	return buttons;
}

Button** create_load_game_buttons(SDL_Renderer* renderer){
	Button** buttons = malloc(sizeof(Button*) * 7);
	if(buttons == NULL) return NULL;
	int horiz_center = (DEFAULT_MENU_WINDOW_WIDTH - DEFAULT_BTN_WIDTH) / 2;
	int horiz_center_for_two = (DEFAULT_MENU_WINDOW_WIDTH - DEFAULT_BTN_WIDTH - DEFAULT_BTN_GAP_HORIZONTAL) / 2;
	//set all parameters:
	int x_btn_places[] = {horiz_center, horiz_center, horiz_center, horiz_center, horiz_center, horiz_center_for_two, horiz_center_for_two + DEFAULT_BTN_GAP_HORIZONTAL};
	int y_btn_places[] = {DEFAULT_GAP_WINDOW,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 2,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 3,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 4,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 7,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 7};
	ButtonType types[] = {GameSlot1, GameSlot2, GameSlot3, GameSlot4, GameSlot5, BackButton, LoadButton};
	const char* image[] = {IMG(slot1), IMG(slot2), IMG(slot3), IMG(slot4), IMG(slot5), IMG(back), IMG(load)};
	const char* image_inavtice[] = {IMG_INCTV(slot1), IMG_INCTV(slot2), IMG_INCTV(slot3), IMG_INCTV(slot4), IMG_INCTV(slot5), IMG_INCTV(back), IMG_INCTV(load)};
	bool active[] = {true, false, false, false, false, true, false};
	bool visible[] = {false, false, false, false, false, true, true};
	//create buttons:
	for(int i = 0; i < 7; i++){
		SDL_Rect Rec = {.x = x_btn_places[i], .y = y_btn_places[i], .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
		buttons[i] = createButton(renderer, &Rec, image[i], image_inavtice[i], types[i], active[i], visible[i]);
		if(buttons[i] == NULL){
			for(int j = 0; j < i; j++)
				destroyButton(buttons[j]);
			free(buttons);
			return NULL;
		}
	}
	return buttons;
}

Button** create_game_mode_buttons(SDL_Renderer* renderer){
	Button** buttons = malloc(sizeof(Button*) * 5);
	if(buttons == NULL) return NULL;
	int horiz_center = (DEFAULT_MENU_WINDOW_WIDTH - DEFAULT_BTN_WIDTH) / 2;
	int horiz_center_for_two = (DEFAULT_MENU_WINDOW_WIDTH - DEFAULT_BTN_WIDTH - DEFAULT_BTN_GAP_HORIZONTAL) / 2;
	//set all parameters:
	int x_btn_places[] = {horiz_center_for_two,
			horiz_center_for_two + DEFAULT_BTN_GAP_HORIZONTAL,
			horiz_center_for_two,
			horiz_center_for_two,
			horiz_center_for_two + DEFAULT_BTN_GAP_HORIZONTAL};

	int y_btn_places[] = {DEFAULT_GAP_WINDOW,
			DEFAULT_GAP_WINDOW,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 6,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 6,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 6};

	ButtonType types[] = {OnePlayer, TwoPlayer, StartButton, NextButton, BackButton};
	const char* image[] = {IMG(one_player), IMG(two_player), IMG(start), IMG(next), IMG(back)};
	const char* image_inavtice[] = {IMG_INCTV(one_player), IMG_INCTV(two_player), IMG_INCTV(start), IMG_INCTV(next), IMG_INCTV(back)};
	bool active[] = {true, false, true, true, true};
	bool visible[] = {true, true, true, false, true};
	//create buttons:
	for(int i = 0; i < 5; i++){
		SDL_Rect Rec = {.x = x_btn_places[i], .y = y_btn_places[i], .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
		buttons[i] = createButton(renderer, &Rec, image[i], image_inavtice[i], types[i], active[i], visible[i]);
		if(buttons[i] == NULL){
			for(int j = 0; j < i; j++)
				destroyButton(buttons[j]);
			free(buttons);
			return NULL;
		}
	}
	return buttons;
}

Button** create_difficulty_buttons(SDL_Renderer* renderer){
	Button** buttons = malloc(sizeof(Button*) * 6);
	if(buttons == NULL) return NULL;
	int horiz_center = (DEFAULT_MENU_WINDOW_WIDTH - DEFAULT_BTN_WIDTH) / 2;
	int horiz_center_for_two = (DEFAULT_MENU_WINDOW_WIDTH - DEFAULT_BTN_WIDTH - DEFAULT_BTN_GAP_HORIZONTAL) / 2;
	//set all parameters:
	int x_btn_places[] = {horiz_center, horiz_center, horiz_center, horiz_center,
			horiz_center_for_two,
			horiz_center_for_two + DEFAULT_BTN_GAP_HORIZONTAL};

	int y_btn_places[] = {DEFAULT_GAP_WINDOW,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 2,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 3,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 6,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 6};

	ButtonType types[] = {NoobDiff, EasyDiff, ModerateDiff, HardDiff, NextButton, BackButton};
	const char* image[] = {IMG(noob), IMG(easy), IMG(moderate), IMG(hard), IMG(next), IMG(back)};
	const char* image_inavtice[] = {IMG_INCTV(noob), IMG_INCTV(easy), IMG_INCTV(moderate), IMG_INCTV(hard), IMG_INCTV(next), IMG_INCTV(back)};
	bool active[] = {true, false, false, false, true, true};
	bool visible[] = {true, true, true, true, true, true};
	//create buttons:
	for(int i = 0; i < 6; i++){
		SDL_Rect Rec = {.x = x_btn_places[i], .y = y_btn_places[i], .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
		buttons[i] = createButton(renderer, &Rec, image[i], image_inavtice[i], types[i], active[i], visible[i]);
		if(buttons[i] == NULL){
			for(int j = 0; j < i; j++)
				destroyButton(buttons[j]);
			free(buttons);
			return NULL;
		}
	}
	return buttons;
}

Button** create_choose_color_buttons(SDL_Renderer* renderer){
	Button** buttons = malloc(sizeof(Button*) * 4);
	if(buttons == NULL) return NULL;
	int horiz_center = (DEFAULT_MENU_WINDOW_WIDTH - DEFAULT_BTN_WIDTH) / 2;
	int horiz_center_for_two = (DEFAULT_MENU_WINDOW_WIDTH - DEFAULT_BTN_WIDTH - DEFAULT_BTN_GAP_HORIZONTAL) / 2;
	//set all parameters:
	int x_btn_places[] = {horiz_center_for_two,
			horiz_center_for_two + DEFAULT_BTN_GAP_HORIZONTAL,
			horiz_center_for_two,
			horiz_center_for_two + DEFAULT_BTN_GAP_HORIZONTAL};

	int y_btn_places[] = {DEFAULT_GAP_WINDOW,
			DEFAULT_GAP_WINDOW,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 6,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 6};

	ButtonType types[] = {SetWhite, SetBlack, StartButton, BackButton};
	const char* image[] = {IMG(one_player), IMG(two_player), IMG(start), IMG(back)};
	const char* image_inavtice[] = {IMG_INCTV(one_player), IMG_INCTV(two_player), IMG_INCTV(start), IMG_INCTV(back)};
	bool active[] = {true, false, true, true};
	bool visible[] = {true, true, true, true};
	//create buttons:
	for(int i = 0; i < 4; i++){
		SDL_Rect Rec = {.x = x_btn_places[i], .y = y_btn_places[i], .h = DEFAULT_BTN_HIGHT, .w = DEFAULT_BTN_WIDTH};
		buttons[i] = createButton(renderer, &Rec, image[i], image_inavtice[i], types[i], active[i], visible[i]);
		if(buttons[i] == NULL){
			for(int j = 0; j < i; j++)
				destroyButton(buttons[j]);
			free(buttons);
			return NULL;
		}
	}
	return buttons;
}

void destroyMenuWindow(MenuWindow* src) {
	if (src == NULL ) {
		return;
	}
	int i = 0;
	for (; i < src->num_buttons; i++) {
		destroyButton(src->buttons[i]);
	}
	SDL_DestroyRenderer(src->windowRenderer);
	SDL_DestroyWindow(src->window);
	free(src->buttons);
	free(src);
}

void drawWindow(MenuWindow* src) {
	if (src == NULL ) {
		return;
	}
	//draw window:
	SDL_SetRenderDrawColor(src->windowRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->windowRenderer);
	int i = 0;
	for (; i < src->num_buttons; i++) {
		drawButton(src->buttons[i]);
	}
	SDL_RenderPresent(src->windowRenderer);
}

ButtonType handleEvenet_enterance(MenuWindow* src, SDL_Event* event){
	if(src == NULL || event==NULL)
		return NoButton;
	if (event->type == SDL_MOUSEBUTTONUP){
		ButtonType type = which_button_clicked(event, src->buttons, src->num_buttons);
		return type;
	}
	else
		return NoButton;
}
