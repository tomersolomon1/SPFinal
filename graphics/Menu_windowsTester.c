/*
 * Menu_windowsTester.c
 *
 *  Created on: Aug 28, 2017
 *      Author: sapir
 */

#include "Menu_windowsTester.h"
#include "../GameBoard.h"

void check_menu_window(){
	menu_window_type type_window = Enterance;
	Gameboard* game = NULL;
	while(1){
		Window* window = create_menu_window(type_window);
		if(window == NULL){
			return;
		}
		type_window = handleEvenet(window, &game);
		destroyWindow(window);
		if(type_window == ExitGame)
			break;
	}


}
