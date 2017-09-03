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

Window_type handleEvenet_load_game(Window* wndw, Button* btn, Gameboard** game){
	Button *slot1 = get_button_by_type(wndw, GameSlot1);
	Button *slot2 = get_button_by_type(wndw, GameSlot2);
	Button *slot3 = get_button_by_type(wndw, GameSlot3);
	Button *slot4 = get_button_by_type(wndw, GameSlot4);
	Button *slot5 = get_button_by_type(wndw, GameSlot5);
	if(btn->type == BackButton)
		return Enterance;
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
		if(slot1->active)
			f = fopen(saved_files[0], "r");
		else if(slot2->active)
			f = fopen(saved_files[1], "r");
		else if(slot3->active)
			f = fopen(saved_files[2], "r");
		else if(slot4->active)
			f = fopen(saved_files[3], "r");
		else if(slot5->active)
			f = fopen(saved_files[4], "r");
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
			(*game)->difficulty = 1;
		if(easy->active)
			(*game)->difficulty = 2;
		if(moder->active)
			(*game)->difficulty = 3;
		if(hard->active)
			(*game)->difficulty = 4;
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
