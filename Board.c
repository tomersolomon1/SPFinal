/*
 * Board.c
 *
 *  Created on: 9 баев„ 2017
 *      Author: User
 */

#include <string.h>
#include <Board.h>
#include <stdbool.h>

Board *create_board() {
	return NULL;
}

void destro_board(Board *board) {

}

Board *copy_board() {
	return NULL;
}

SP_CHESS_BOARD_MESSAGE set_move(Board *board, Move move) {
	return SP_CHESS_BOARD_SUCCESS;
}

bool is_valid_move(Board *board, Move move) {
	return true;
}

bool is_check_curr_player(Board *board) {
	return false;
}

Move *get_moves(Board *board, Piece *piece) {
	return NULL;
}

Move *get_all_moves(Board *board, int colur) {
	return NULL;
}

Piece *get_piece_in_place(Board *board, int row, int col) {
	return NULL;
}

bool is_undo_valid(Board *board) {
	return true;
}

SP_CHESS_BOARD_MESSAGE undo_move(Board *board) {
	return SP_CHESS_BOARD_SUCCESS;
}

char is_game_over(Board *board) {
	return 'a';
}

bool is_check_mate(Board *board) {
	return true;
}

void print_board(Board *board) {
}
