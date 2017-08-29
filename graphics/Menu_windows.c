/*
 * Menu_windows.c
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */
#include "Menu_windows.h"
#include "SPCommon.h"

const char* window_name[] = {"Chess: Main Menu","Chess: Load Game","Chess: Game Mode","Chess: Difficulty","Chess: Choose Color"};
int num_buttons[] = {3,7,5,7,4,0,0};

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

	if(type == Enterance){
		src->buttons = create_enterance_buttons(renderer);
	}
	else if(type == LoadGame){
		src->buttons = create_load_game_buttons(renderer);
	}
	else if(type == ModeGame){
		src->buttons = create_game_mode_buttons(renderer);
	}
	else if(type == Difficulty){
		src->buttons = create_difficulty_buttons(renderer);
	}
	else if(type == ChooseColor){
		src->buttons = create_choose_color_buttons(renderer);
	}

	if(src->buttons == NULL){
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
	const char* image[] = {IMG(start), IMG(load), IMG(exit)};
	const char* image_inavtice[] = {IMG_INCTV(start), IMG_INCTV(load), IMG_INCTV(exit)};
//	const char* image[] = {"./graphics/images/start.bmp", "./graphics/images/load.bmp", "./graphics/images/exit.bmp"};
//	const char* image_inavtice[] = {"./graphics/images/start_inactive.bmp", "./graphics/images/load_inactive.bmp", "./graphics/images/exit_inactive.bmp"};

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
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 5,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 5};
	ButtonType types[] = {GameSlot1, GameSlot2, GameSlot3, GameSlot4, GameSlot5, BackButton, LoadButton};
	const char* image[] = {IMG(gameslot1), IMG(gameslot2), IMG(gameslot3), IMG(gameslot4), IMG(gameslot5), IMG(back), IMG(load)};
	const char* image_inavtice[] = {IMG_INCTV(gameslot1), IMG_INCTV(gameslot2), IMG_INCTV(gameslot3), IMG_INCTV(gameslot4), IMG_INCTV(gameslot5), IMG_INCTV(back), IMG_INCTV(load)};
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
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 5,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 5,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 5};

	ButtonType types[] = {OnePlayer, TwoPlayer, StartButton, NextButton, BackButton};
	const char* image[] = {IMG(one_player), IMG(two_players), IMG(start), IMG(next), IMG(back)};
	const char* image_inavtice[] = {IMG_INCTV(one_player), IMG_INCTV(two_players), IMG_INCTV(start), IMG_INCTV(next), IMG_INCTV(back)};
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
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 5,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 5};

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
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 5,
			DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 5};

	ButtonType types[] = {SetWhite, SetBlack, StartButton, BackButton};
	const char* image[] = {IMG(white), IMG(black), IMG(start), IMG(back)};
	const char* image_inavtice[] = {IMG_INCTV(white), IMG_INCTV(black), IMG_INCTV(start), IMG_INCTV(back)};
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
	SDL_SetRenderDrawColor(src->windowRenderer, 200, 255, 255, 255);
	SDL_RenderClear(src->windowRenderer);
	for (int i = 0; i < src->num_buttons; i++) {
		drawButton(src->buttons[i]);
	}
	SDL_RenderPresent(src->windowRenderer);
}

Button* get_button_by_type(MenuWindow* wndw, ButtonType type){
	for(int i = 0; i < wndw->num_buttons; i++){
		if(wndw->buttons[i]->type == type){
			return wndw->buttons[i];
		}
	}
	return NoButton;
}

menu_window_type handleEvenet_enterance(MenuWindow* wndw, Button* btn){
	if(btn->type == NewGameButton){
		return ModeGame;
	}
	else if(btn->type == ExitButton){
		return ExitGame;
	}
	else if(btn->type == LoadButton){
		return LoadGame;
	}
	return wndw->type;
}
menu_window_type handleEvenet_load_game(MenuWindow* wndw, Button* btn, Gameboard** game){
	if(btn->type == BackButton)
		return Enterance;
	return wndw->type;
}
menu_window_type handleEvenet_mode_game(MenuWindow* wndw, Button* btn, Gameboard** game){

	return wndw->type;
}
menu_window_type handleEvenet_difficulty(MenuWindow* wndw, Button* btn, Gameboard** game){
	return wndw->type;
}
menu_window_type handleEvenet_choose_color(MenuWindow* wndw, Button* btn, Gameboard** game){
	return wndw->type;
}

menu_window_type handleEvenet(MenuWindow* wndw, Gameboard** game){
	if(wndw == NULL)
		return ExitGame;
	menu_window_type type = Enterance;
	SDL_Event event;
	while(1){
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT)
			return ExitGame;
		else if (event.type == SDL_MOUSEBUTTONUP){
			Button* btn = get_button_clicked(&event, wndw->buttons, wndw->num_buttons);
			if (btn == NULL)
				continue;
			else{
				if(wndw->type == Enterance)
					type =  handleEvenet_enterance(wndw, btn);
				else if(wndw->type == LoadGame)
					type = handleEvenet_load_game(wndw, btn, game);
				else if(wndw->type == ModeGame)
					type = handleEvenet_mode_game(wndw, btn, game);
				else if(wndw->type == Difficulty)
					type = handleEvenet_difficulty(wndw, btn, game);
				else if(wndw->type == ChooseColor)
					type = handleEvenet_choose_color(wndw, btn, game);
				if(type == wndw->type){
					continue;
				}
				else{
					return type;
				}
			}
		}
		drawWindow(wndw);
	}
}

