/*
 * MainWin.h
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */

#ifndef GRAPHICS_MAINWIN_H_
#define GRAPHICS_MAINWIN_H_
#include <assert.h>
#include <SDl2/SDL.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
///mashu
typedef enum{
	MAIN_NEW_GAME,
	MAIN_LOAD,
	MAIN_EXIT,
} MAIN_EVENT;

typedef struct{
	SDL_Window* main_winodw;
	SDL_Renderer* main_renderer;
	SDL_Texture* new_game_tex;
	SDL_Texture* load_tex;
	SDL_Texture* exit_tex;
} MainWin;

#endif /* GRAPHICS_MAINWIN_H_ */
