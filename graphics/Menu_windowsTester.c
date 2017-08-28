/*
 * Menu_windowsTester.c
 *
 *  Created on: Aug 28, 2017
 *      Author: sapir
 */

#include "Menu_windowsTester.h"

void check_menu_window(){
	MenuWindow* enter = create_menu_window(Enterance);
	if(enter == NULL){
		return;
	}
	SDL_Event event;
	while(1){
		SDL_WaitEvent(&event);
		if(event.type == SDL_QUIT){
			break;
		}
		drawWindow(enter);
	}

}
