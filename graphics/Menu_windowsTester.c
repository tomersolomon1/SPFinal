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

	while(1){
		MenuWindow* window = create_menu_window(type_window);
		if(window == NULL){
			return;
		}
		if(type_window == Enterance)
			type_window = handleEvenet_enterance(window);


		destroyMenuWindow(window);
		if(type_window == None)
			break;
	}


}
