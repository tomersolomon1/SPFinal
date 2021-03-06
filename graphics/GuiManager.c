/*
 * Menu_windowsTester.c
 *
 *  Created on: Aug 28, 2017
 *      Author: sapir
 */

#include "GuiManager.h"
#include "../ConsoleMode.h"

void run_gui(Gameboard *game){
	SDL_Init(SDL_INIT_VIDEO);
	Window_type type_new_window = Enterance;
	Window_type type_old_window = Enterance;
	while(1){
		Window* window = create_window(type_new_window, game); //we check if the window is NULL inside handleEvent
		type_new_window = handleEvenet(window, &game, type_old_window);
		if(type_new_window == SDLErrorWindow){
			printf("There was a problem with SDL along the way, we are sorry \n");
			break;
		}
		type_old_window = window->type;
		destroyWindow(window);
		if(type_new_window == ExitGame)
			break;
	}
	SDL_Quit();
	destroy_board(game);
}

Window_type handleEvenet(Window* wndw, Gameboard** game, Window_type old_type_window){
	if(wndw == NULL)
		return SDLErrorWindow;
	Window_type type = Enterance; /* default value */
	SDL_Event event;
	int success;
	set_buttons_by_game_params(wndw, game);
	if (wndw->type == Game) {
		type = gui_begin_game(wndw);
		if (type != Game) { /* either the game is over, or we had problem with SDL */
			return type;
		}
	}
	while(1){
		if (SDL_WaitEvent(&event) == 0) /* catches error in SDL_WaitEvent */
			return SDLErrorWindow;
		if (event.type == SDL_QUIT)
			return ExitGame;
		else if (wndw->type == Game) {
			type = handle_game_events(wndw, &event, game);
			if (type != Game) {
				return type;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			Button* btn = get_button_clicked(&event, wndw->buttons, wndw->num_buttons);
			if(btn == NULL || (event.button.button != SDL_BUTTON_LEFT))
				continue;
			if(wndw->type == Enterance)
				type =  handleEvenet_enterance(wndw, btn);
			else if(wndw->type == LoadGame)
				type = handleEvenet_load_game(wndw, btn, game, old_type_window);
			else if(wndw->type == ModeGame)
				type = handleEvenet_mode_game(wndw, btn, game);
			else if(wndw->type == Difficulty)
				type = handleEvenet_difficulty(wndw, btn, game);
			else if(wndw->type == ChooseColor)
				type = handleEvenet_choose_color(wndw, btn, game);
			if  (type != wndw->type){
				return type;
			}
		}
		success = drawWindow(wndw, &event);
		if(success == -1)
			return SDLErrorWindow;
	}
}

void set_buttons_by_game_params(Window* wndw, Gameboard** game){
	if(wndw->type == ModeGame){
		Button* one_player = get_button_by_type(wndw, OnePlayer);
		Button* two_player = get_button_by_type(wndw, TwoPlayer);
		if((*game)->game_mode == 1){
			one_player->active = true;
			two_player->active = false;
		}
		else if((*game)->game_mode == 2){
			one_player->active = false;
			two_player->active = true;
		}
	}
	else if(wndw->type == Difficulty){
		Button* noob = get_button_by_type(wndw, NoobDiff);
		Button* easy = get_button_by_type(wndw, EasyDiff);
		Button* moder = get_button_by_type(wndw, ModerateDiff);
		Button* expr = get_button_by_type(wndw, HardDiff);
		noob->active = false;
		easy->active = false;
		moder->active = false;
		expr->active = false;
		if((*game)->difficulty == 1)
			noob->active = true;
		else if((*game)->difficulty == 2)
			easy->active = true;
		else if((*game)->difficulty == 3)
			moder->active = true;
		else if((*game)->difficulty == 4)
			expr->active = true;
	}
	else if(wndw->type == ChooseColor){
		Button* wite = get_button_by_type(wndw, SetWhite);
		Button* blck = get_button_by_type(wndw, SetBlack);
		if((*game)->user_color == white){
			wite->active = true;
			blck->active = false;
		}
		else if((*game)->user_color == black){
			wite->active = false;
			blck->active = true;
		}
	}
}
