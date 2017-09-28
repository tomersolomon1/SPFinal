#include <stdlib.h>
#include "GameWindow.h"
#include "SPCommon.h"
#include "../ConsoleMode.h"
#include "../Files.h"


void destroy_board_widget(BoardWidget *board_widget) {
	if (board_widget == NULL)
		return;
	destroy_game_textures(board_widget);
	free(board_widget->location);
	free(board_widget);
}

void destroy_game_textures(BoardWidget *board_widget) {
	if(board_widget->board_grid != NULL)
		SDL_DestroyTexture(board_widget->board_grid);
	if(board_widget->possible_move_texture != NULL)
		SDL_DestroyTexture(board_widget->possible_move_texture);
	if(board_widget->threatened_move_texture != NULL)
		SDL_DestroyTexture(board_widget->threatened_move_texture);
	if(board_widget->capturing_move_texture != NULL)
		SDL_DestroyTexture(board_widget->capturing_move_texture);
	if(board_widget->castling_texture != NULL)
			SDL_DestroyTexture(board_widget->castling_texture);
	for(int color = 0; color < 2; color++) {
		for (int i = 0; i < 6; i++) {
			if(board_widget->piece_textures[color][i] != NULL)
				SDL_DestroyTexture(board_widget->piece_textures[color][i]);
		}
	}
}

void destory_data(GameData *data) {
	if (data == NULL) {
		return;
	}
	destroy_board_widget(data->board_widget);
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
	const char* image_inavtice[] = {IMG_INCTV(white), IMG_INCTV(white), IMG_INCTV(white),
			IMG_INCTV(undo), IMG_INCTV(white), IMG_INCTV(white)}; /* only the undo is relevant */
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
	if (data->board_widget == NULL) {
		destory_data(data);
		return NULL;
	}
	data->picked_piece = false;
	data->selected_piece_color = -1;
	data->selected_piece_index = -1;
	data->saved_game = false;
	data->highlight_moves = false;
	return data;
}

BoardWidget *create_widget_board(SDL_Renderer *window_renderer, Gameboard *board, SDL_Rect* location) {
	const char *white_pieces_images[] = {CHESS_IMAGE(WPawn), CHESS_IMAGE(WKnight), CHESS_IMAGE(WBishop),
			CHESS_IMAGE(WRock),	CHESS_IMAGE(WQueen), CHESS_IMAGE(WKing)}; /* white pieces BMP paths */
	const char *black_pieces_images[] = {CHESS_IMAGE(BPawn), CHESS_IMAGE(BKnight), CHESS_IMAGE(BBishop),
			CHESS_IMAGE(BRock),	CHESS_IMAGE(BQueen), CHESS_IMAGE(BKing)}; /* black pieces BMP paths */

	BoardWidget *board_widget = init_widget_board(board, location);
	char *board_bmps[] = {IMG(ChessBoard), IMG(possible square), IMG(threatened square), IMG(capturing square), IMG(castling square)};
	SDL_Texture **board_textures[] = {&(board_widget->board_grid), &(board_widget->possible_move_texture),
			&(board_widget->threatened_move_texture), &(board_widget->capturing_move_texture), &(board_widget->castling_texture)};
	// create texture for the board and the squares
	for (int i = 0; i < 5; i++) {
		*(board_textures[i]) = create_texure_from_bmp(window_renderer, board_bmps[i], false);
		if (*(board_textures[i]) == NULL) {
			destroy_board_widget(board_widget);
			return NULL;
		}
	}
	// create texture for the pieces
	for (int i = 0; i < 6; i++) {
		board_widget->piece_textures[1][i] = create_texure_from_bmp(window_renderer, white_pieces_images[i], true);
		board_widget->piece_textures[0][i] = create_texure_from_bmp(window_renderer, black_pieces_images[i], true);
		if (board_widget->piece_textures[0][i] == NULL || board_widget->piece_textures[1][i] == NULL) {
			destroy_board_widget(board_widget);
			return NULL;
		}
	}
	return board_widget;
}

BoardWidget *init_widget_board(Gameboard *board, SDL_Rect* location) {
	BoardWidget *board_widget = (BoardWidget *) malloc(sizeof(BoardWidget));
	assert(board_widget != NULL);
	board_widget->location = spCopyRect(location);
	board_widget->board = board;

	// nullifying all the pointers
	board_widget->board_grid = NULL;
	board_widget->possible_move_texture = NULL;
	board_widget->threatened_move_texture = NULL;
	board_widget->capturing_move_texture = NULL;
	board_widget->castling_texture = NULL;
	for (int i = 0; i < 6; i++) {
		board_widget->piece_textures[1][i] = NULL;
		board_widget->piece_textures[0][i] = NULL;
	}
	return board_widget;
}


int highlight_square(GameData* data, SDL_Renderer *renderer, SDL_Texture* texture, int delta_x, int delta_y, int row_dim, int col_dim){
	int x_offset = data->board_widget->location->x + (delta_x * col_dim);
	int y_offset = data->board_widget->location->y + (delta_y * row_dim);
	SDL_Rect step_rec = {.x = x_offset, .y = y_offset, .h = row_dim, .w = col_dim };
	if(SDL_RenderDrawRect(renderer, &step_rec) == -1) { /* some SDL error occurred */
		printf("Error: There was a problem with SDL_RenderDrawRect\n");
		return -1;
	}
	if ((SDL_RenderCopy(renderer, texture, NULL, &step_rec)) == -1) {
		printf("Error: There was a problem with SDL_RenderCopy\n");
		return -1;
	}
	return 0;
}

int highlight_king_for_castling(GameData *data, SDL_Renderer *renderer, Piece* selected_piece, int row_dim, int col_dim){
	int success = 0;
	Piece *king = data->board_widget->board->all_pieces[selected_piece->colur][KING_INDEX];
	for(int i = 0; i < king->amount_steps; i++){
		Step *s = king->steps[i];
		if(s->step_info == Castling_Move && abs(s->dcol - selected_piece->col) < 3){
			SDL_Texture* txtr = data->board_widget->castling_texture;
			success = highlight_square(data, renderer, txtr, 4, 7 - selected_piece->row, row_dim, col_dim);
			break;
		}
	}
	return success;
}


int highlight_moves_feature(GameData *data, SDL_Renderer *renderer, int row_dim, int col_dim) {
	Piece *selected_piece = data->board_widget->board->all_pieces[data->selected_piece_color][data->selected_piece_index];
	int success = 0; /* so far so good */
	SDL_Texture* txtr;
	for (int step_index = 0; step_index < selected_piece->amount_steps; step_index++) {
		Step *step = selected_piece->steps[step_index];
		if (step->is_threatened) {
			txtr = data->board_widget->threatened_move_texture;
			success = highlight_square(data, renderer, txtr, step->dcol, (7-step->drow), row_dim, col_dim);
		}
		else{
			Piece *dest_occupier = data->board_widget->board->board[step->drow][step->dcol];
			if(step->step_info == Castling_Move){
				txtr = data->board_widget->castling_texture;
				success = highlight_square(data, renderer, txtr, step->dcol, (7-step->drow), row_dim, col_dim);
				if(success == 0){
					int delta_y = (step->dcol > step->scol ? 7 : 0);
					success = highlight_square(data, renderer, txtr , delta_y, (7-step->drow), row_dim, col_dim);
				}
			}
			else if (dest_occupier->type == Empty) {
				txtr = data->board_widget->possible_move_texture;
				success = highlight_square(data, renderer, txtr, step->dcol, (7-step->drow), row_dim, col_dim);
			}
			else { /* has to be capturing move */
				txtr = data->board_widget->capturing_move_texture;
				success = highlight_square(data, renderer, txtr, step->dcol, (7-step->drow), row_dim, col_dim);
			}
		}
		if (success == -1)
			return success;
	}
	if(selected_piece->type == Rook)
		success = highlight_king_for_castling(data, renderer, selected_piece, row_dim, col_dim);
	return success;
}

/* return 0 on success, -1 otherwise */
int draw_board(GameData *data, SDL_Renderer *renderer, SDL_Event* event) {
	int success = SDL_RenderCopy(renderer, data->board_widget->board_grid, NULL, data->board_widget->location);
	if (success == -1) {
		printf("Error: There was a problem with SDL_RenderCopy\n");
		return -1;
	}
	int row_rec_dim = data->board_widget->location->h / 9;
	int col_rec_dim = data->board_widget->location->w / 9;
	int row_dim = data->board_widget->location->h / 8;
	int col_dim = data->board_widget->location->w / 8;
	if (data->highlight_moves) {
		highlight_moves_feature(data, renderer, row_dim, col_dim);
		data->highlight_moves = false;
	}
	SDL_Rect piece_rec = {.x = 0, .y = 0, .h = row_rec_dim, .w = col_rec_dim }; // default values
	for (int color = 0; color < 2; color++) {
		for (int i = 0; i < 16; i++) {
			Piece *piece = data->board_widget->board->all_pieces[color][i];
			if (piece->alive && (color != data->selected_piece_color || i != data->selected_piece_index || !data->picked_piece)) {
				/* this piece is alive and not selected, thus need to be drawn in his current place */
				piece_rec.x = data->board_widget->location->x + HORIZONTAL_CENTERING + piece->col*col_dim;
				piece_rec.y = data->board_widget->location->y + VERTICAL_CENTERING + (7 - piece->row)*row_dim;
				success = SDL_RenderCopy(renderer, data->board_widget->piece_textures[piece->colur][piece->type], NULL, &piece_rec);
				if (success == -1) {
					printf("Error: There was a problem with SDL_RenderCopy\n");
					return -1;
				}
			}
		}
	}
	if (data->picked_piece && event != NULL) { // some piece was selected, and it moves around with the mouse
		piece_rec.x = event->motion.x;
		piece_rec.y = event->motion.y;
		Piece *piece = data->board_widget->board->all_pieces[data->selected_piece_color][data->selected_piece_index];
		success = SDL_RenderCopy(renderer, data->board_widget->piece_textures[data->selected_piece_color][piece->type], NULL, &piece_rec);
		if (success == -1) {
			printf("Error: There was a problem with SDL_RenderCopy\n");
			return -1;
		}
	}
	return 0;
}
