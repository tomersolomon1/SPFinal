/*
 * Game_Window_Tester.c
 *
 *  Created on: 29 баев„ 2017
 *      Author: User
 */

#include "GameWindow.h"
#include "../GameBoard.h"

void check_game_window() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { // SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return;
	}
	Gameboard *game_board = create_board(1, 1, 1);
	GameWindow *game_window = create_game_window(game_board);
	if(game_window == NULL){
		printf("unable to create window\n");
		return;
	}
	SDL_Event event;
	drawGameWindow(game_window, NULL, -1, -1);
	while(1){
		SDL_WaitEvent(&event);
		if(event.type == SDL_QUIT){
			break;
		}  else {
			handle_game_events(game_window, &event);
		}

	}
	destroy_board(game_board);
}
