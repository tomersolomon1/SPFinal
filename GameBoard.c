/*
 * Board.c
 *
 *  Created on: 9 баев„ 2017
 *      Author: User
 */


#include "GameBoard.h"
#define black 0
#define white 1

GameBoard *create_board() {
	GameBoard *newBoard = (GameBoard*) malloc(sizeof(GameBoard));
	Piece *board[8][8];
	Piece *all_pieces[2][16]; //[0] = black, [1] = white
	ArrayList* history = ArrayListCreate(3);
	int i = 0;
	add_piece(board, all_pieces, Pawn, black, 6, 0, i++, 'M');
	add_piece(board, all_pieces, Pawn, black, 6, 1, i++, 'M');
	add_piece(board, all_pieces, Pawn, black, 6, 2, i++, 'M');
	add_piece(board, all_pieces, Pawn, black, 6, 3, i++, 'M');
	add_piece(board, all_pieces, Pawn, black, 6, 4, i++, 'M');
	add_piece(board, all_pieces, Pawn, black, 6, 5, i++, 'M');
	add_piece(board, all_pieces, Pawn, black, 6, 6, i++, 'M');
	add_piece(board, all_pieces, Pawn, black, 6, 7, i++, 'M');
	add_piece(board, all_pieces, Bishop, black, 7, 2, i++, 'B');
	add_piece(board, all_pieces, Bishop, black, 7, 5, i++, 'B');
	add_piece(board, all_pieces, Knight, black, 7, 1, i++, 'N');
	add_piece(board, all_pieces, Knight, black, 7, 6, i++, 'N');
	add_piece(board, all_pieces, Rock, black, 7, 0, i++, 'R');
	add_piece(board, all_pieces, Rock, black, 7, 7, i++, 'R');
	add_piece(board, all_pieces, Queen, black, 7, 3, i++, 'Q');
	add_piece(board, all_pieces, King, black, 7, 4, i++, 'K');
	i = 0;
	add_piece(board, all_pieces, Pawn, white, 1, 0, i++, 'm');
	add_piece(board, all_pieces, Pawn, white, 1, 1, i++, 'm');
	add_piece(board, all_pieces, Pawn, white, 1, 2, i++, 'm');
	add_piece(board, all_pieces, Pawn, white, 1, 3, i++, 'm');
	add_piece(board, all_pieces, Pawn, white, 1, 4, i++, 'm');
	add_piece(board, all_pieces, Pawn, white, 1, 5, i++, 'm');
	add_piece(board, all_pieces, Pawn, white, 1, 6, i++, 'm');
	add_piece(board, all_pieces, Pawn, white, 1, 7, i++, 'm');
	add_piece(board, all_pieces, Bishop, white, 0, 2, i++, 'b');
	add_piece(board, all_pieces, Bishop, white, 0, 5, i++, 'b');
	add_piece(board, all_pieces, Knight, white, 0, 1, i++, 'n');
	add_piece(board, all_pieces, Knight, white, 0, 6, i++, 'n');
	add_piece(board, all_pieces, Rock, white, 0, 0, i++, 'r');
	add_piece(board, all_pieces, Rock, white, 0, 7, i++, 'r');
	add_piece(board, all_pieces, Queen, white, 0, 3, i++, 'q');
	add_piece(board, all_pieces, King, white, 0, 4, i++, 'k');
	Piece* Empty_piece = create_piece(Empty, -1, -1, -1, -1, '');
	for(int i = 2; i < 6; i++){
		for(int j = 0; j < 7; j++){
			board[i][j] = Empty_piece;
		}
	}
	newBoard->board = board;
	newBoard->all_pieces = all_pieces;
	newBoard->turn = white;
	newBoard->history = history;
	return newBoard;
}

void add_piece(Piece* board, Piece* all_pieces, Piece_type type, int colur, int row, int col, int indexat, char sign){
	Piece* piece = create_piece(type, colur, row, col, indexat, sign);
	board[row][col] = piece;
	all_pieces[colur][indexat] = piece;
}

void destroy_board(GameBoard *board) {
	if(board == NULL){
		return;
	}
	ArrayListDestroy(board->history);
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			destroy_piece(board->board[i][j]);
		}
	}
	free(board);
}

GameBoard *copy_board(GameBoard* old) {
	if(old == NULL){
		return NULL;
	}
	GameBoard *newBoard = (GameBoard*) malloc(sizeof(GameBoard));
	Piece *board[8][8];
	Piece *all_pieces[2][16];
	Piece *curr;
	Piece* Empty_piece = create_piece(Empty, -1, -1, -1, -1, '');
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			curr = copy_piece(old->board[i][j]);
			if(curr->type != Empty){
				board[i][j] = curr;
				all_pieces[curr->colur][curr->indext] = curr;
			}
			else{
				board[i][j] = Empty_piece;
			}
		}
	}
	ArrayList* history = ArrayListCopy(old->history);
	newBoard->all_pieces = all_pieces;
	newBoard->board = board;
	newBoard->history = history;
	newBoard->turn = old->turn;
	return newBoard;
}

CHESS_BOARD_MESSAGE set_move(GameBoard *board, Move move) {
	return CHESS_BOARD_SUCCESS;
}

bool is_valid_move(GameBoard *board, Move move) {
	return true;
}

bool is_check_curr_player(GameBoard *board) {
	return false;
}

Move *get_moves(GameBoard *board, Piece *piece) {
	return NULL;
}

Move *get_all_moves(GameBoard *board, int colur) {
	return NULL;
}

Piece *get_piece_in_place(GameBoard *board, int row, int col) {
	return NULL;
}

bool is_undo_valid(GameBoard *board) {
	return true;
}

CHESS_BOARD_MESSAGE undo_move(GameBoard *board) {
	return CHESS_BOARD_SUCCESS;
}

char is_game_over(GameBoard *board) {
	return 'a';
}

bool is_check_mate(GameBoard *board) {
	return true;
}

void print_board(GameBoard *board) {
}


