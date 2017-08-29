/*
 * GameWindow.c
 *
 *  Created on: 28 ����� 2017
 *      Author: User
 */

#include <stdlib.h>
#include "GameWindow.h"

char *ABC2[] = { "A", "B", "C", "D","E","F", "G","H" };
const char *white_pieces_images[] = {CHESS_IMAGE(WPawn), CHESS_IMAGE(WKnight), CHESS_IMAGE(WBishop),
		CHESS_IMAGE(WRock),	CHESS_IMAGE(WQueen), CHESS_IMAGE(WKing)};
const char *black_pieces_images[] = {CHESS_IMAGE(BPawn), CHESS_IMAGE(BKnight), CHESS_IMAGE(BBishop),
		CHESS_IMAGE(BRock),	CHESS_IMAGE(BQueen), CHESS_IMAGE(BKing)};

void destroy_all_textures() {
	// to be filled later
}

GameWindow *create_game_window(Gameboard *board) {
	GameWindow *game_window = (GameWindow *) malloc(sizeof(GameWindow));
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
	game_window->num_buttons = 0; // to be changed
	game_window->buttons = NULL;
	SDL_Rect board_rec = {.x = 0, .y = 0, .h = 8*(DEFAULT_BOARD_MAXIMAL_HEIGHT/9), .w = 8*(DEFAULT_BOARD_MAXIMAL_WIDTH/9) };
	game_window->board_widget = create_widget_board(renderer, board, &board_rec);

	return game_window;
}

BoardWidget *create_widget_board(SDL_Renderer *window_renderer, Gameboard *board, SDL_Rect* location) {
	BoardWidget *board_widget = (BoardWidget *) malloc(sizeof(BoardWidget));
	board_widget->location = spCopyRect(location);
	board_widget->renderer = window_renderer;
	board_widget->board = board;
	SDL_Surface *board_grid = SDL_LoadBMP(IMG(ChessBoard)); // don't forget to check the board later
	board_widget->board_grid = SDL_CreateTextureFromSurface(window_renderer, board_grid);
	SDL_FreeSurface(board_grid);
	// create texture for the pieces
	for (int i = 0; i < 6; i++) {
		SDL_Surface *white_piece_surface = SDL_LoadBMP(white_pieces_images[i]);
		SDL_Surface *black_piece_surface = SDL_LoadBMP(black_pieces_images[i]);
		if(white_piece_surface == NULL || black_piece_surface == NULL) { /* see what's in the manual later */
			SDL_FreeSurface(white_piece_surface);
			SDL_FreeSurface(black_piece_surface);
			printf("couldn't create surfaces:\n");
			printf("black: %s\n", black_pieces_images[i]);
			printf("white: %s\n", white_pieces_images[i]);
			return NULL;
		}
		SDL_SetColorKey(white_piece_surface, SDL_TRUE, SDL_MapRGB(white_piece_surface->format, 255, 0, 255)); // make it's background transparent
		SDL_SetColorKey(black_piece_surface, SDL_TRUE, SDL_MapRGB(black_piece_surface->format, 255, 0, 255)); // make it's background transparent
		board_widget->piece_textures[0][i] = SDL_CreateTextureFromSurface(window_renderer, black_piece_surface);
		board_widget->piece_textures[1][i] = SDL_CreateTextureFromSurface(window_renderer, white_piece_surface);
		SDL_FreeSurface(white_piece_surface);
		SDL_FreeSurface(black_piece_surface);
		if (board_widget->piece_textures[0][i] == NULL || board_widget->piece_textures[1][i] == NULL) { /* see what's in the manual later */
			destroy_all_textures();
		}
	}
	printf(CHESS_IMAGE(WQueen));
	printf("\n");
	printf("created all surfaces!\n");
	return board_widget;
}

void draw_board(BoardWidget *board_widget) {
	SDL_RenderClear(board_widget->renderer);
	SDL_RenderCopy(board_widget->renderer, board_widget->board_grid, NULL, board_widget->location);
	int row_rec_dim = board_widget->location->h / 9;
	int col_rec_dim = board_widget->location->w / 9;
	int row_dim = board_widget->location->h / 8;
	int col_dim = board_widget->location->w / 8;
	SDL_Rect piece_rec = {.x = 0, .y = 0, .h = row_rec_dim, .w = col_rec_dim }; // default values
	for (int color = 0; color < 2; color++) {
		for (int i = 0; i < 16; i++) {
			Piece *piece = board_widget->board->all_pieces[color][i];
			if (piece->alive) { /* this piece is alive and need to be drawn */
				piece_rec.x = board_widget->location->x + HORIZONTAL_CENTERING + piece->col*col_dim;
				piece_rec.y = board_widget->location->y + VERTICAL_CENTERING + (7 - piece->row)*row_dim;
				SDL_RenderCopy(board_widget->renderer, board_widget->piece_textures[piece->colur][piece->type], NULL, &piece_rec);
			}
		}
	}
}

void drawGameWindow(GameWindow* src) {
	if (src == NULL ) {
		return;
	}
	// draw window:
	SDL_SetRenderDrawColor(src->windowRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->windowRenderer);
	draw_board(src->board_widget);
	SDL_RenderPresent(src->windowRenderer);
}

void recognize_square(GameWindow *window, int x, int y) {
	SDL_Point point = {.x = x, .y = y };
	if(SDL_PointInRect(&point ,window->board_widget->location)) { // inside the board
		int x_board = (8*point.x / window->board_widget->location->w);
		int y_board = 8 - (8*point.y / window->board_widget->location->h);
		printf("hit on board:%s%d\n", ABC2[x_board], y_board);
	} else {
		printf("outside the window!\n");
	}
	fflush(stdout);
}

void handle_game_events(GameWindow *window, SDL_Event* event) {
	if (event == NULL || window == NULL ) {
		return;
	}
	switch(event->type) {
		case SDL_MOUSEBUTTONUP:
			recognize_square(window, event->button.x, event->button.y);
	}
}




