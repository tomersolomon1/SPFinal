/*
 * MainWin.c
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */

#include "MainWin.h"
#include <assert.h>

MainWin* create_MainWindow(){
	MainWin* mainwin = NULL;
	SDL_Surface* loadingSurface = NULL;
	mainwin = (MainWin*) calloc(sizeof(MainWin), sizeof(char));
	assert(main != NULL);

	mainwin->main_winodw = SDL_CreateWindow("Chess ~IM IN HEAVEN~",	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			400, 450, SDL_WINDOW_OPENGL);
	assert(mainwin->main_winodw != NULL);
	mainwin->main_renderer = SDL_CreateRenderer(mainwin->main_winodw, -1, SDL_RENDERER_ACCELERATED);
	assert(mainwin->main_renderer != NULL);
	return main;
}
