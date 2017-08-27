/*
 * MainWin.c
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */

#include "MainWin.h"



Window* create_MainWindow(){
	Window* mainwin = malloc(sizeof(Window));
	assert(mainwin != NULL);
	SDL_Window* window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_OPENGL);
	assert
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	return main;
}
