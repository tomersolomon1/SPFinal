/*
 * Game_Window_Tester.c
 *
 *  Created on: 29 баев„ 2017
 *      Author: User
 */


#include "../GameBoard.h"
#include "Windows.h"
#include "GameWindow.h"
#include "GuiManager_Game.h"

/* debugging function */
Window *create_game_window(Gameboard *board) {
	Window *game_window = (Window *) malloc(sizeof(Window));
	//game_window->board = board;
	SDL_Window* window = SDL_CreateWindow("Chess Game",  // window title
				SDL_WINDOWPOS_CENTERED,           // initial x position
				SDL_WINDOWPOS_CENTERED,           // initial y position
				DEFAULT_GAME_WINDOW_WIDTH,        // width, in pixels
				DEFAULT_GAME_WINDOW_HEIGHT,       // height, in pixels
				SDL_WINDOW_OPENGL                 // flags
				);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	game_window->window = window;
	game_window->windowRenderer = renderer;
	game_window->num_buttons = 6;
	game_window->buttons = create_game_buttons(renderer);
	assert(game_window->buttons != NULL);
	game_window->data = create_game_data(renderer, board);
	return game_window;
}


void check_game_window() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { // SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return;
	}
	Gameboard *game_board = create_board(1, 1, 1);
	Window *game_window = create_game_window(game_board);
	if(game_window == NULL){
		printf("unable to create window\n");
		return;
	}
	SDL_Event event;
	drawGameWindow(game_window, NULL);
	while(1){
		SDL_WaitEvent(&event);
		if(event.type == SDL_QUIT){
			break;
		}  else {
			//Button *btn = NULL;
			handle_game_events(game_window, &event, &game_board);
			drawGameWindow(game_window, &event);
		}

	}
	destroy_board(game_board);
}

// for debugging purposes
void drawGameWindow(Window* src, SDL_Event* event) {
	if (src == NULL ) {
		return;
	}
	// draw window:
	SDL_SetRenderDrawColor(src->windowRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->windowRenderer);
	draw_board(src->data, src->windowRenderer, event); /* no piece was selected */
	for (int i = 0; i < src->num_buttons; i++) {
		drawButton(src->buttons[i]);
	}
	SDL_RenderPresent(src->windowRenderer);
}



// ------------------------- old code ---------------------------
/* from create-board: */
/*
	// create board texture
	board_widget->board_grid = create_board_grid(window_renderer, IMG(ChessBoard));
	if (board_widget->board_grid) {
		return NULL;
	}
	SDL_Surface *possible_move_surface   = SDL_LoadBMP(IMG(possible square));
	SDL_Surface *threatened_move_surface   = SDL_LoadBMP(IMG(threatened square));
	SDL_Surface *capturing_move_surface   = SDL_LoadBMP(IMG(capturing square));
	if (possible_move_surface == NULL || threatened_move_surface == NULL || capturing_move_surface == NULL) {
		SDL_FreeSurface(possible_move_surface);
		SDL_FreeSurface(threatened_move_surface);
		SDL_FreeSurface(capturing_move_surface);
		return NULL;
	}
	board_widget->possible_move_texture = SDL_CreateTextureFromSurface(window_renderer, possible_move_surface);
	board_widget->threatened_move_texture = SDL_CreateTextureFromSurface(window_renderer, threatened_move_surface);
	board_widget->capturing_move_texture = SDL_CreateTextureFromSurface(window_renderer, capturing_move_surface);
	SDL_FreeSurface(possible_move_surface);
	SDL_FreeSurface(threatened_move_surface);
	SDL_FreeSurface(capturing_move_surface);
	if (board_widget->possible_move_texture == NULL || board_widget->threatened_move_texture == NULL || board_widget->capturing_move_texture == NULL) {
		destroy_game_textures(board_widget);
		return NULL;
	}


	for (int i = 0; i < 6; i++) {
		SDL_Surface *white_piece_surface = SDL_LoadBMP(white_pieces_images[i]);
		SDL_Surface *black_piece_surface = SDL_LoadBMP(black_pieces_images[i]);
		if(white_piece_surface == NULL || black_piece_surface == NULL) {
			SDL_FreeSurface(white_piece_surface);
			SDL_FreeSurface(black_piece_surface);
			destroy_game_textures(board_widget);
			return NULL;
		}
		SDL_SetColorKey(white_piece_surface, SDL_TRUE, SDL_MapRGB(white_piece_surface->format, 255, 0, 255)); // make it's background transparent
		SDL_SetColorKey(black_piece_surface, SDL_TRUE, SDL_MapRGB(black_piece_surface->format, 255, 0, 255)); // make it's background transparent
		board_widget->piece_textures[0][i] = SDL_CreateTextureFromSurface(window_renderer, black_piece_surface);
		board_widget->piece_textures[1][i] = SDL_CreateTextureFromSurface(window_renderer, white_piece_surface);
		SDL_FreeSurface(white_piece_surface);
		SDL_FreeSurface(black_piece_surface);
		if (board_widget->piece_textures[0][i] == NULL || board_widget->piece_textures[1][i] == NULL) { //see what's in the manual later
			destroy_game_textures(board_widget);
			return NULL;
		}
	}
	  */
