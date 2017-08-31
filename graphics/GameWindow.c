/*
 * GameWindow.c
 *
 *  Created on: 28 баев„ 2017
 *      Author: User
 */

#include <stdlib.h>
#include "GameWindow.h"
#include "../ConsoleMode.h"

char *ABC2[] = { "A", "B", "C", "D","E","F", "G","H" };
const char *white_pieces_images[] = {CHESS_IMAGE(WPawn), CHESS_IMAGE(WKnight), CHESS_IMAGE(WBishop),
		CHESS_IMAGE(WRock),	CHESS_IMAGE(WQueen), CHESS_IMAGE(WKing)};
const char *black_pieces_images[] = {CHESS_IMAGE(BPawn), CHESS_IMAGE(BKnight), CHESS_IMAGE(BBishop),
		CHESS_IMAGE(BRock),	CHESS_IMAGE(BQueen), CHESS_IMAGE(BKing)};

void destroy_all_textures() {
	// to be filled later
}

void recognize_square(Window *window, int x, int y) {
	SDL_Point point = {.x = x, .y = y };
	if(SDL_PointInRect(&point ,window->data->board_widget->location)) { // inside the board
		int relative_x = x - window->data->board_widget->location->x;
		int relative_y = y - window->data->board_widget->location->y;
		int x_board = (8*relative_x / window->data->board_widget->location->w);
		int y_board = 8 - (8*relative_y / window->data->board_widget->location->h);
		printf("hit on board:%s%d\n", ABC2[x_board], y_board);
	} else {
		printf("outside the window!\n");
	}
	fflush(stdout);
}

/* helper function, to be converted to macro later */
bool mouse_in_rec(int x, int y, SDL_Rect *rect) {
	SDL_Point point = {.x = x, .y = y };
	return SDL_PointInRect(&point, rect);

}

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
	GameData *data = (GameData *) malloc(sizeof(GameData));
	assert(data != NULL);
	SDL_Rect board_rec = {.x = DEFAULT_GAME_BUTTON_PANEL_WIDTH, .y = 0, .h = 8*(DEFAULT_BOARD_MAXIMAL_HEIGHT/9), .w = 8*(DEFAULT_BOARD_MAXIMAL_WIDTH/9) };
	data->board_widget = create_widget_board(renderer, board, &board_rec);
	data->picked_piece = false;
	data->selected_piece_color = -1;
	data->selected_piece_index = -1;
	return game_window;
}

Button **create_game_buttons(SDL_Renderer* window_renderer) {
	//set all parameters:
	int x_btn_places[] = {DEFAULT_GAME_BUTTON_HORIZONTAL_GAP, DEFAULT_GAME_BUTTON_HORIZONTAL_GAP,
			DEFAULT_GAME_BUTTON_HORIZONTAL_GAP, DEFAULT_GAME_BUTTON_HORIZONTAL_GAP,
			DEFAULT_GAME_BUTTON_HORIZONTAL_GAP, DEFAULT_GAME_BUTTON_HORIZONTAL_GAP};

	int y_btn_places[] = {0, DEFAULT_GAME_BUTTON_HEIGHT + DEFAULT_GAME_BUTTON_VERTICAL_GAP,
			2*DEFAULT_GAME_BUTTON_HEIGHT + 2*DEFAULT_GAME_BUTTON_VERTICAL_GAP,
			3*DEFAULT_GAME_BUTTON_HEIGHT + 3*DEFAULT_GAME_BUTTON_VERTICAL_GAP,
			5*DEFAULT_GAME_BUTTON_HEIGHT + 4*DEFAULT_GAME_BUTTON_VERTICAL_GAP,
			6*DEFAULT_GAME_BUTTON_HEIGHT + 5*DEFAULT_GAME_BUTTON_VERTICAL_GAP,};

	ButtonType types[] = {RestartButton, SaveButton, LoadButton, UndoButton, MenuButton, ExitButton};
	const char* image[] = {IMG(start), IMG(start), IMG(load), IMG(start), IMG(start), IMG(exit)};
	const char* image_inavtice[] = {IMG_INCTV(white), IMG_INCTV(white), IMG_INCTV(white), IMG_INCTV(white), IMG_INCTV(white), IMG_INCTV(white)};
	bool active[] = {true, true, true, false, true, true};
	bool visible[] = {true, true, true, true, true, true};
	Button **buttons = create_buttons(window_renderer, types, 6, x_btn_places,
			y_btn_places, image, image_inavtice, active, visible, DEFAULT_GAME_BUTTON_HEIGHT, DEFAULT_GAME_BUTTON_WIDTH);
	printf("created all buttons\n");
	fflush(stdout);
	return buttons;
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

void draw_board(BoardWidget *board_widget, SDL_Event* event, int selected_piece_color, int selected_piece_index) {
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
			if (piece->alive && (color != selected_piece_color || i != selected_piece_index)) { /* this piece is alive and not selected, thus need to be drawn in his current place */
				piece_rec.x = board_widget->location->x + HORIZONTAL_CENTERING + piece->col*col_dim;
				piece_rec.y = board_widget->location->y + VERTICAL_CENTERING + (7 - piece->row)*row_dim;
				SDL_RenderCopy(board_widget->renderer, board_widget->piece_textures[piece->colur][piece->type], NULL, &piece_rec);
			}
		}
	}
	if (selected_piece_color != -1 && event != NULL) { // some piece was selected, and it moves around with the mouse
		piece_rec.x = event->motion.x;
		piece_rec.y = event->motion.y;
		Piece *piece = board_widget->board->all_pieces[selected_piece_color][selected_piece_index];
		SDL_RenderCopy(board_widget->renderer, board_widget->piece_textures[selected_piece_color][piece->type], NULL, &piece_rec);
	}
	SDL_RenderPresent(board_widget->renderer);
}

void drawGameWindow(Window* src, SDL_Event* event, int selected_piece_color, int selected_piece_index) {
	if (src == NULL ) {
		return;
	}
	// draw window:
	SDL_SetRenderDrawColor(src->windowRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->windowRenderer);
	draw_board(src->data->board_widget, event, selected_piece_color, selected_piece_index); /* no piece was selected */
	for (int i = 0; i < src->num_buttons; i++) {
		drawButton(src->buttons[i]);
	}
	SDL_RenderPresent(src->windowRenderer);
}



/*
 * assuming the move is legal
 * set the step, and show a SimpleMessageBox if the game is over
 * return:
 * 		true  - the game is over
 * 		false - the game isn't over
 */

bool graphical_handle_single_move(Window *window, int srow, int scol, int drow, int dcol) {
	Gameboard *board = window->data->board_widget->board;
	set_step(board, srow, scol, drow, dcol);
	drawGameWindow(window, NULL, -1, -1); /* drawing the board, no piece is selected */
	int game_over = is_game_over(board);
	char mssg[50];
	if (game_over == 0 || game_over == 1 || game_over == 2) { /* the game is over */

		if (game_over == 2) { /* it's a tie */
			strcpy(mssg, "It's a tie!");
		} else { /* somebody won the game */
			sprintf(mssg, "Checkmate! %s player wins the game", colors[game_over]);
		}
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", mssg, NULL);
		printf("notice from graphical handler: the game is over");
		return true;
	} else if (is_under_check(board)) {
		sprintf(mssg, "Check: %s King is threatend!", colors[board->turn]);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Check!", mssg, NULL);
	}
	return false;
}

bool graphical_handle_move(Window *window, int srow, int scol, int drow, int dcol) {
	bool game_over = graphical_handle_single_move(window, srow, scol, drow, dcol);
	if (game_over) {
		return true;
	} else if (window->data->board_widget->board->game_mode == 1) { /* the game is not over, and we need to play the computer's turn */
		Gameboard *copy = copy_board(window->data->board_widget->board);
		Move move = find_best_move(copy, copy->difficulty);
		destroy_board(copy);
		return graphical_handle_single_move(window, move.srow, move.scol, move.drow, move.dcol);
	}
	return false; /* the game is not over yet */
}

Window_type handle_game_events(Window *window, SDL_Event* event) {
	//SDL_Point point;
	if (event == NULL || window == NULL ) {
		return ExitGame;
	}
	switch(event->type) {
		case SDL_MOUSEBUTTONDOWN:
			if (mouse_in_rec(event->button.x, event->button.y, window->data->board_widget->location)
					&& (event->button.button == SDL_BUTTON_LEFT)) {
				int relative_x = event->button.x - window->data->board_widget->location->x;
				int relative_y = event->button.y - window->data->board_widget->location->y;
				int x_board = (8*relative_x / window->data->board_widget->location->w);
				int y_board = 7 - (8*relative_y / window->data->board_widget->location->h);
				recognize_square(window, event->button.x, event->button.y); // for debug
				Piece *piece = window->data->board_widget->board->board[y_board][x_board];
				if (piece->type != Empty && piece->colur == window->data->board_widget->board->turn) { /* this piece will be selected now */
					window->data->selected_piece_color = piece->colur;
					window->data->selected_piece_index = piece->indexat;
					window->data->picked_piece  = true;
				}
			} else if (event->button.button == SDL_BUTTON_LEFT) { /* maybe we clicked some button? */
				Button *clicked_button = get_button_clicked(event, window->buttons, window->num_buttons);
				if (clicked_button != NULL) { /* some button was clicked */
					switch(clicked_button->type) {
						case RestartButton:
							printf("restart!\n");
							break;
						case ExitButton:
							return ExitGame;
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event->button.button == SDL_BUTTON_LEFT && window->data->picked_piece) { /* the selected piece was dropped */
				window->data->picked_piece  = false;
				if (mouse_in_rec(event->button.x, event->button.y, window->data->board_widget->location)) {
					int relative_x = event->button.x - window->data->board_widget->location->x;
					int relative_y = event->button.y - window->data->board_widget->location->y;
					int x_board = (8*relative_x / window->data->board_widget->location->w);
					int y_board = 7 - (8*relative_y / window->data->board_widget->location->h);
					Piece *piece = window->data->board_widget->board->all_pieces[window->data->selected_piece_color][window->data->selected_piece_index];
					CHESS_BOARD_MESSAGE mssg = is_valid_step(window->data->board_widget->board, piece->row, piece->col, y_board, x_board);
					window->data->selected_piece_color = -1;
					window->data->selected_piece_index = -1;
					window->data->picked_piece  = false;
					if (mssg == CHESS_BOARD_SUCCESS) {
						if (graphical_handle_move(window, piece->row, piece->col, y_board, x_board)) { /* the game is over */
							return;
						}
					} else {
						drawGameWindow(window, event, window->data->selected_piece_color, window->data->selected_piece_index); /* not -1, -1?????*/
					}
				}
			}
			break;
		case SDL_MOUSEMOTION:
			if(mouse_in_rec(event->motion.x, event->motion.y, window->data->board_widget->location) && window->data->picked_piece) {
				drawGameWindow(window, event, window->data->selected_piece_color, window->data->selected_piece_index);
			}
			break;
	}
}




