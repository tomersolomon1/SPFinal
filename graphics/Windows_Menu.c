/*
 * MenuWindowsCreate.c
 *
 *  Created on: Sep 2, 2017
 *      Author: sapir
 */


#include "Windows_Menu.h"

Button** create_enterance_buttons(SDL_Renderer* renderer) {
	int horiz_center = (DEFAULT_MENU_WINDOW_WIDTH - DEFAULT_BTN_WIDTH) / 2;
	//set all parameters:
	int x_btn_places[] = {horiz_center, horiz_center, horiz_center};
	int y_btn_places[] = {DEFAULT_GAP_WINDOW, DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL, DEFAULT_GAP_WINDOW + DEFAULT_BTN_GAP_VERTICAL * 2};
	ButtonType types[] = {NewGameButton, LoadButton, ExitButton};
	const char* image[] = {IMG(new_game), IMG(load), IMG(exit)};
	const char* image_inavtice[] = {IMG_INCTV(new_game), IMG_INCTV(load), IMG_INCTV(exit)};
	bool active[] = {true, true, true};
	bool visible[] = {true, true, true};

	Button **buttons = create_buttons(renderer, types, AMOUNT_BUTTONS_ENTERANCE, x_btn_places,
				y_btn_places, image, image_inavtice, active, visible, DEFAULT_BTN_HIGHT,DEFAULT_BTN_WIDTH);

	return buttons;
}

Button** create_load_game_buttons(SDL_Renderer* renderer){
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
	bool active[] = {false, false, false, false, false, true, false};
	bool visible[] = {false, false, false, false, false, true, true};
	int amount_files = count_saves();
	for(int i = 0; i < amount_files; i++) //make exist slots visible
		visible[i] = true;

	Button **buttons = create_buttons(renderer, types, AMOUNT_BUTTONS_LOAD_GAME, x_btn_places,
			y_btn_places, image, image_inavtice, active, visible, DEFAULT_BTN_HIGHT,DEFAULT_BTN_WIDTH);

	return buttons;
}

Button** create_game_mode_buttons(SDL_Renderer* renderer){
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
	bool visible[] = {true, true, false, true, true};

	Button **buttons = create_buttons(renderer, types, AMOUNT_BUTTONS_GAME_MODE, x_btn_places,
					y_btn_places, image, image_inavtice, active, visible, DEFAULT_BTN_HIGHT,DEFAULT_BTN_WIDTH);

	return buttons;
}

Button** create_difficulty_buttons(SDL_Renderer* renderer){
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

	Button **buttons = create_buttons(renderer, types, AMOUNT_BUTTONS_DIFFICULTY, x_btn_places,
			y_btn_places, image, image_inavtice, active, visible, DEFAULT_BTN_HIGHT,DEFAULT_BTN_WIDTH);

	return buttons;
}

Button** create_choose_color_buttons(SDL_Renderer* renderer){
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

	Button **buttons = create_buttons(renderer, types, AMOUNT_BUTTONS_CHOOSE_COLOR, x_btn_places,
			y_btn_places, image, image_inavtice, active, visible, DEFAULT_BTN_HIGHT,DEFAULT_BTN_WIDTH);

	return buttons;
}
