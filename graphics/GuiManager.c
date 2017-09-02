/*
 * Menu_windowsTester.c
 *
 *  Created on: Aug 28, 2017
 *      Author: sapir
 */

#include "GuiManager.h"

void check_menu_window(){
	Window_type type_window = Enterance;
	Gameboard* game = create_board(1,1,1);
	while(1){
		Window* window = create_window(type_window, game);
		if(window == NULL){
			return;
		}
		type_window = handleEvenet(window, &game);
		destroyWindow(window);
		if(type_window == ExitGame)
			break;
	}
	destroy_board(game);
}

Window_type handleEvenet(Window* wndw, Gameboard** game){
	if(wndw == NULL)
		return ExitGame;
	Window_type type = Enterance;
	SDL_Event event;
	set_buttons_by_game_params(wndw, game);
	while(1){
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT)
			return ExitGame;
		else if (event.type == SDL_MOUSEBUTTONUP){
			Button* btn = get_button_clicked(&event, wndw->buttons, wndw->num_buttons);
			if(wndw->type == Game){
				type = handle_game_events(wndw, &event, game, btn);
			}
			else{
				if(btn == NULL || (event.button.button != SDL_BUTTON_LEFT))
					continue;
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
			}
			if(type == wndw->type){
				continue;
			}
			else{
				return type;
			}
		}
		drawWindow(wndw, &event);
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
		Button* noob = get_button_by_type(wndw, EasyDiff);
		Button* easy = get_button_by_type(wndw, NoobDiff);
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