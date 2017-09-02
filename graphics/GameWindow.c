/*
 * GameWindow.c
 *
 *  Created on: 28 ����� 2017
 *      Author: User
 */

#include <stdlib.h>
#include "GameWindow.h"
#include "SPCommon.h"
#include "../ConsoleMode.h"
#include "../Files.h"


const char *white_pieces_images[] = {CHESS_IMAGE(WPawn), CHESS_IMAGE(WKnight), CHESS_IMAGE(WBishop),
		CHESS_IMAGE(WRock),	CHESS_IMAGE(WQueen), CHESS_IMAGE(WKing)};
const char *black_pieces_images[] = {CHESS_IMAGE(BPawn), CHESS_IMAGE(BKnight), CHESS_IMAGE(BBishop),
		CHESS_IMAGE(BRock),	CHESS_IMAGE(BQueen), CHESS_IMAGE(BKing)};

/* helper function, to be converted to macro later */
bool mouse_in_rec(int x, int y, SDL_Rect *rect) {
	SDL_Point point = {.x = x, .y = y };
	return SDL_PointInRect(&point, rect);

}

void destroy_game_textures(BoardWidget *board_widget) {
	SDL_DestroyTexture(board_widget->board_grid);
	for (int color = 0; color < 2; color++) {
		for (int i = 0; i < 6; i++) {
			SDL_DestroyTexture(board_widget->piece_textures[color][i]);
		}
	}
}

/*
 * somebody else is incharge of destroying the gameboard itself
 */
void destory_data(GameData *data) {
	if (data == NULL) {
		return;
	}
	// freeing the textures
	destroy_game_textures(data->board_widget);

	free(data->board_widget->location);
	free(data->board_widget);
	free(data);
}

Button **create_game_buttons(SDL_Renderer* window_renderer) {
	//set all parameters:
	int x_btn_places[] = {DEFAULT_GAME_BUTTON_HORIZONTAL_GAP, DEFAULT_GAME_BUTTON_HORIZONTAL_GAP,
			DEFAULT_GAME_BUTTON_HORIZONTAL_GAP, DEFAULT_GAME_BUTTON_HORIZONTAL_GAP,
			DEFAULT_GAME_BUTTON_HORIZONTAL_GAP, DEFAULT_GAME_BUTTON_HORIZONTAL_GAP};

	int y_btn_places[] = {DEFAULT_GAME_BUTTON_VERTICAL_GAP, DEFAULT_GAME_BUTTON_HEIGHT + 2*DEFAULT_GAME_BUTTON_VERTICAL_GAP,
			2*DEFAULT_GAME_BUTTON_HEIGHT + 3*DEFAULT_GAME_BUTTON_VERTICAL_GAP,
			3*DEFAULT_GAME_BUTTON_HEIGHT + 4*DEFAULT_GAME_BUTTON_VERTICAL_GAP,
			5*DEFAULT_GAME_BUTTON_HEIGHT + 5*DEFAULT_GAME_BUTTON_VERTICAL_GAP,
			6*DEFAULT_GAME_BUTTON_HEIGHT + 6*DEFAULT_GAME_BUTTON_VERTICAL_GAP,};

	ButtonType types[] = {RestartButton, SaveButton, LoadButton, UndoButton, MenuButton, ExitButton};
	const char* image[] = {IMG(restart), IMG(save), IMG(load), IMG(undo), IMG(main_menu), IMG(exit)};
	const char* image_inavtice[] = {IMG_INCTV(white), IMG_INCTV(white), IMG_INCTV(white), IMG_INCTV(undo), IMG_INCTV(white), IMG_INCTV(white)};
	bool active[] = {true, true, true, false, true, true};
	bool visible[] = {true, true, true, true, true, true};
	Button **buttons = create_buttons(window_renderer, types, 6, x_btn_places,
			y_btn_places, image, image_inavtice, active, visible, DEFAULT_GAME_BUTTON_HEIGHT, DEFAULT_GAME_BUTTON_WIDTH);
	return buttons;
}

GameData *create_game_data(SDL_Renderer* renderer, Gameboard *board) {
	GameData *data = (GameData *) malloc(sizeof(GameData));
	assert(data != NULL);
	SDL_Rect board_rec = {.x = DEFAULT_GAME_BUTTON_PANEL_WIDTH, .y = DEFAULT_GAME_BUTTON_VERTICAL_GAP,
			.h = 8*(DEFAULT_BOARD_MAXIMAL_HEIGHT/9), .w = 8*(DEFAULT_BOARD_MAXIMAL_WIDTH/9) };
	data->board_widget = create_widget_board(renderer, board, &board_rec);
	data->picked_piece = false;
	data->selected_piece_color = -1;
	data->selected_piece_index = -1;
	return data;
}

/* return NULL on failure */
BoardWidget *create_widget_board(SDL_Renderer *window_renderer, Gameboard *board, SDL_Rect* location) {
	BoardWidget *board_widget = (BoardWidget *) malloc(sizeof(BoardWidget));
	board_widget->location = spCopyRect(location);
	//board_widget->renderer = window_renderer;
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
			destroy_game_textures(board_widget);
			return NULL;
		}
		SDL_SetColorKey(white_piece_surface, SDL_TRUE, SDL_MapRGB(white_piece_surface->format, 255, 0, 255)); // make it's background transparent
		SDL_SetColorKey(black_piece_surface, SDL_TRUE, SDL_MapRGB(black_piece_surface->format, 255, 0, 255)); // make it's background transparent
		board_widget->piece_textures[0][i] = SDL_CreateTextureFromSurface(window_renderer, black_piece_surface);
		board_widget->piece_textures[1][i] = SDL_CreateTextureFromSurface(window_renderer, white_piece_surface);
		SDL_FreeSurface(white_piece_surface);
		SDL_FreeSurface(black_piece_surface);
		if (board_widget->piece_textures[0][i] == NULL || board_widget->piece_textures[1][i] == NULL) { /* see what's in the manual later */
			destroy_game_textures(board_widget);
			return NULL;
		}
	}
	return board_widget;
}

void draw_board(GameData *data, SDL_Renderer *renderer, SDL_Event* event) {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, data->board_widget->board_grid, NULL, data->board_widget->location);
	int row_rec_dim = data->board_widget->location->h / 9;
	int col_rec_dim = data->board_widget->location->w / 9;
	int row_dim = data->board_widget->location->h / 8;
	int col_dim = data->board_widget->location->w / 8;
	SDL_Rect piece_rec = {.x = 0, .y = 0, .h = row_rec_dim, .w = col_rec_dim }; // default values
	for (int color = 0; color < 2; color++) {
		for (int i = 0; i < 16; i++) {
			Piece *piece = data->board_widget->board->all_pieces[color][i];
			if (piece->alive && (color != data->selected_piece_color || i != data->selected_piece_index || !data->picked_piece)) { /* this piece is alive and not selected, thus need to be drawn in his current place */
				piece_rec.x = data->board_widget->location->x + HORIZONTAL_CENTERING + piece->col*col_dim;
				piece_rec.y = data->board_widget->location->y + VERTICAL_CENTERING + (7 - piece->row)*row_dim;
				SDL_RenderCopy(renderer, data->board_widget->piece_textures[piece->colur][piece->type], NULL, &piece_rec);
			}
		}
	}
	if (data->picked_piece && event != NULL) { // some piece was selected, and it moves around with the mouse
		piece_rec.x = event->motion.x;
		piece_rec.y = event->motion.y;
		Piece *piece = data->board_widget->board->all_pieces[data->selected_piece_color][data->selected_piece_index];
		SDL_RenderCopy(renderer, data->board_widget->piece_textures[data->selected_piece_color][piece->type], NULL, &piece_rec);
	}
	SDL_RenderPresent(renderer);
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
	drawGameWindow(window, NULL); /* drawing the board, no piece is selected */
	int game_over = is_game_over(board);
	char mssg[50];
	if (game_over == 0 || game_over == 1 || game_over == 2) { /* the game is over */

		if (game_over == 2) { /* it's a tie */
			strcpy(mssg, "It's a tie!");
		} else { /* somebody won the game */
			sprintf(mssg, "Checkmate! %s player wins the game", colors[game_over]);
		}
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", mssg, NULL);
		printf("notice from graphical handler: the game is over\n");
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

void save_game_from_gui(Gameboard *game) {
	promote_saves();
	FILE *file = fopen(saved_files[0], "w");
	assert(file != NULL);
	save_xml(file, game);
	fclose(file);
}

Window_type handle_game_events(Window *window, SDL_Event *event,  Gameboard **game) {
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
				Piece *piece = window->data->board_widget->board->board[y_board][x_board];
				if (piece->type != Empty && piece->colur == window->data->board_widget->board->turn) { /* this piece will be selected now */
					window->data->selected_piece_color = piece->colur;
					window->data->selected_piece_index = piece->indexat;
					window->data->picked_piece  = true;
				}
			}
			else { /* the click wasn't inside the board, or it wasn't a LEFT button */
				Button* clicked_button = get_button_clicked(event, window->buttons, window->num_buttons);
				if (clicked_button != NULL) { /* some button was clicked */
					switch(clicked_button->type) {
						case RestartButton:
							reset_board(game);
							window->data->board_widget->board = *game;
							return Game;
						case SaveButton:
							save_game_from_gui(*game);
							return Game;
						case LoadButton:
							printf("are you sure?????????????\n"); /* to be updated */
							return LoadGame;
						case UndoButton:
							if (clicked_button->active) {
								double_undo(window->data->board_widget->board);
								if (ArrayListSize(window->data->board_widget->board->history) < 2) {
									window->buttons[UndoButtonIndex]->active = false;
								}
							}
							return Game;
						case MenuButton:
							destroy_board(*game);
							*game = create_board(1,1,1); /* reseting the settings */
							return Enterance;
						case ExitButton:
							return ExitGame;
						default: /* something went wrong somewhere */
							return ExitGame;
					}
				}
			}
			return Game; /* if no case was applied */
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
					if (mssg == CHESS_BOARD_SUCCESS && graphical_handle_move(window, piece->row, piece->col, y_board, x_board)) {
						return Game;
					}
				}
			}
			if (ArrayListSize(window->data->board_widget->board->history) > 1 && window->data->board_widget->board->game_mode == 1) {
				window->buttons[UndoButtonIndex]->active = true;
			}
			return Game;;
		case SDL_MOUSEMOTION:
			if(!window->data->picked_piece || !mouse_in_rec(event->motion.x, event->motion.y, window->data->board_widget->location)) {
				window->data->picked_piece = false;
			}
			return Game;
		default:
			return Game;
	}
	return Game;
}

// ------------------------------- for debugging only --------------------------------------
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

/* debugging function */
char *ABC2[] = { "A", "B", "C", "D","E","F", "G","H" };
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

