/*
 * GuiManager_Menu.c
 *
 *  Created on: Sep 2, 2017
 *      Author: sapir
 */

#include "GuiManager_Menu.h"

Window_type handleEvenet_enterance(Window* wndw, Button* btn){
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

Window_type handleEvenet_load_game(Window* wndw, Button* btn, Gameboard** game, Window_type old_type_window){
	Button *slot1 = get_button_by_type(wndw, GameSlot1);
	Button *slot2 = get_button_by_type(wndw, GameSlot2);
	Button *slot3 = get_button_by_type(wndw, GameSlot3);
	Button *slot4 = get_button_by_type(wndw, GameSlot4);
	Button *slot5 = get_button_by_type(wndw, GameSlot5);
	if(btn->type == BackButton)
		return old_type_window;
	//clicked on one of the slots:
	else if(GameSlot1 <= btn->type && btn->type <= GameSlot5){
		slot1->active = false;
		slot2->active = false;
		slot3->active = false;
		slot4->active = false;
		slot5->active = false;
		btn->active = true;
		get_button_by_type(wndw, LoadButton)->active = true;
	}
	else if(btn->type == LoadButton){
		FILE *f = NULL;
		char game_path[SAVED_GAME_PATH_LENGTH];
		if(slot1->active)
			set_game_path(game_path, 0);
		else if(slot2->active)
			set_game_path(game_path, 1);
		else if(slot3->active)
			set_game_path(game_path, 2);
		else if(slot4->active)
			set_game_path(game_path, 3);
		else if(slot5->active)
			set_game_path(game_path, 4);
		f = fopen(game_path, "r");
		assert(f != NULL);
		destroy_board(*game);
		*game = load_game(f);
		fclose(f);
		return Game;
	}
	return wndw->type;
}

Window_type handleEvenet_mode_game(Window* wndw, Button* btn, Gameboard** game){
	if(btn->type == BackButton)
		return Enterance;
	Button* two_player = get_button_by_type(wndw, TwoPlayer);
	Button* one_player = get_button_by_type(wndw, OnePlayer);
	Button* start = get_button_by_type(wndw, StartButton);
	Button* next = get_button_by_type(wndw, NextButton);
	if(btn->type == OnePlayer){
		one_player->active = true;
		two_player->active = false;
		start->visibility = false;
		next->visibility = true;
	}
	else if(btn->type == TwoPlayer){
		one_player->active = false;
		two_player->active = true;
		start->visibility = true;
		next->visibility = false;
	}
	else if(btn->type == StartButton){
		(*game)->game_mode = 2;
		return Game;
	}
	else if(btn->type == NextButton){
		(*game)->game_mode = 1;
		return Difficulty;
	}
	return wndw->type;
}

Window_type handleEvenet_difficulty(Window* wndw, Button* btn, Gameboard** game){
	if(btn->type == BackButton){
		return ModeGame;
	}
	Button* noob = get_button_by_type(wndw, NoobDiff);
	Button* easy = get_button_by_type(wndw, EasyDiff);
	Button* moder = get_button_by_type(wndw, ModerateDiff);
	Button* hard = get_button_by_type(wndw, HardDiff);
	if(btn->type == NextButton){
		if(noob->active)
			change_game_difficulty((*game), 1);
		if(easy->active)
			change_game_difficulty((*game), 2);
		if(moder->active)
			change_game_difficulty((*game), 3);
		if(hard->active)
			change_game_difficulty((*game), 4);
		return ChooseColor;
	}
	else if(NoobDiff <= btn->type && btn->type <= HardDiff){
		noob->active = false;
		easy->active = false;
		moder->active = false;
		hard->active = false;
		btn->active = true;
	}
	return wndw->type;
}

Window_type handleEvenet_choose_color(Window* wndw, Button* btn, Gameboard** game){
	if(btn->type == BackButton)
		return Difficulty;
	Button* wite = get_button_by_type(wndw, SetWhite);
	Button* blck = get_button_by_type(wndw, SetBlack);
	if(SetBlack <= btn->type && btn->type <= SetWhite){
		wite->active = false;
		blck->active = false;
		btn->active = true;
	}
	else if(btn->type == StartButton){
		if(wite->active)
			(*game)->user_color = white;
		else
			(*game)->user_color = black;
		return Game;
	}
	return wndw->type;
}
