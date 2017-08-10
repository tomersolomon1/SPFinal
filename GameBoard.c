/*
 * Board.c
 *
 *  Created on: 9 баев„ 2017
 *      Author: User
 */

#include "GameBoard.h"



Gameboard *create_board() {
	Gameboard *newBoard = (Gameboard*) malloc(sizeof(Gameboard));
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
	newBoard->empty = Empty_piece;
	return newBoard;
}

void add_piece(Piece* board, Piece* all_pieces, Piece_type type, int colur, int row, int col, int indexat, char sign){
	Piece* piece = create_piece(type, colur, row, col, indexat, sign);
	board[row][col] = piece;
	all_pieces[colur][indexat] = piece;
}

void destroy_board(Gameboard *gameboard) {
	if(gameboard == NULL){
		return;
	}
	ArrayListDestroy(gameboard->history);
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			destroy_piece(gameboard->board[i][j]);
		}
	}
	free(gameboard);
}

Gameboard *copy_board(Gameboard* old) {
	if(old == NULL){
		return NULL;
	}
	Gameboard *newBoard = (Gameboard*) malloc(sizeof(Gameboard));
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
	newBoard->empty = Empty_piece;
	return newBoard;
}

Step create_step(int srow, int scol, int drow, int dcol, Piece *prev){
	Step newStep;
	newStep.srow = srow;
	newStep.dcol = dcol;
	newStep.drow = drow;
	newStep.scol = scol;
	newStep.prevPiece = prev;
	return Step;
}

CHESS_BOARD_MESSAGE set_Step(Gameboard *gameboard, Step step) {
	return CHESS_BOARD_SUCCESS;
}

bool is_valid_Step(Gameboard *gameboard, Step step) {
	return true;
}

bool is_check_curr_player(Gameboard *gameboard) {
	for(int i = 0; i < 16; i++){
		Piece piece = gameboard->all_pieces[gameboard->turn][i];
		if(piece->alive){
			if(piece->type == Pawn){

			}
		}
	}
	return false;
}

bool is_check_by_vector(Gameboard *gameboard, Piece *piece, int delta_row, int delta_col, int amount_going){
	int row = piece->row;
	int col = piece->col;
	while(amount_going > 0){
		amount_going --;
		row = row + delta_row;
		col = col + delta_col;
		if(row < 0 || row > 7 || col < 0 || col > 7){ //out of board
			return false;
		}
		else if(gameboard->board[row][col]->type == Empty){ // can go, empty
			continue;
		}
		else if(gameboard->board[row][col]->type == King &&
				gameboard->board[row][col]->colur != piece->colur){ //eating opponent's king
			return true;
		}
		else{ // seeing your color piece
			return false;
		}
	}
	return false;
}
void set_Steps(Gameboard *gameboard, Piece *piece) {
	int amount_steps = 0;


}

void add_Steps_by_vector(Gameboard *gameboard, Piece *piece, int delta_row, int delta_col, int amount_going, int *amount_steps){
	int row = piece->row;
	int col = piece->col;
	while(amount_going > 0){
		amount_going --;
		row = row + delta_row;
		col = col + delta_col;
		if(row < 0 || row > 7 || col < 0 || col > 7){ //out of board
			break;
		}
		if(gameboard->board[row][col]->type == Empty){ // can go, empty
			Step s = create_step(piece->row, piece->col, row, col, gameboard.empty);
			piece->steps[amount_steps] = s;
			amount_steps++;
		}
		else if(gameboard->board[row][col]->colur != piece->colur){ //eating opponent's piece
			Step s = create_step(piece->row, piece->col, row, col, gameboard->board[row][col]);
			piece->steps[amount_steps] = s;
			amount_steps++;
			break;
		}
		else{ // seeing your color piece
			break;
		}
	}

}


Step *get_all_Steps(Gameboard *board, int colur) {
	return NULL;
}

Piece *get_piece_in_place(Gameboard *board, int row, int col) {
	return NULL;
}

bool is_undo_valid(Gameboard *board) {
	return true;
}

CHESS_BOARD_MESSAGE undo_Step(Gameboard *board) {
	return CHESS_BOARD_SUCCESS;
}

char is_game_over(Gameboard *board) {
	return 'a';
}

bool is_check_mate(Gameboard *board) {
	return true;
}

void print_board(Gameboard *board) {
}


