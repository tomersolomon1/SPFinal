/*
 * Board.c
 *
 *  Created on: 9 баев„ 2017
 *      Author: User
 */

#include "GameBoard.h"

Gameboard *create_board() {
	Gameboard *newBoard = (Gameboard*) malloc(sizeof(Gameboard));
	assert(newBoard != NULL);
	newBoard->history = ArrayListCreate(3 * 2);
	int i = 0;
	add_piece(newBoard, Pawn, black, 6, 0, i++, 'M');
	add_piece(newBoard, Pawn, black, 6, 1, i++, 'M');
	add_piece(newBoard, Pawn, black, 6, 2, i++, 'M');
	add_piece(newBoard, Pawn, black, 6, 3, i++, 'M');
	add_piece(newBoard, Pawn, black, 6, 4, i++, 'M');
	add_piece(newBoard, Pawn, black, 6, 5, i++, 'M');
	add_piece(newBoard, Pawn, black, 6, 6, i++, 'M');
	add_piece(newBoard, Pawn, black, 6, 7, i++, 'M');
	add_piece(newBoard, Bishop, black, 7, 2, i++, 'B');
	add_piece(newBoard, Bishop, black, 7, 5, i++, 'B');
	add_piece(newBoard, Knight, black, 7, 1, i++, 'N');
	add_piece(newBoard, Knight, black, 7, 6, i++, 'N');
	add_piece(newBoard, Rock, black, 7, 0, i++, 'R');
	add_piece(newBoard, Rock, black, 7, 7, i++, 'R');
	add_piece(newBoard, Queen, black, 7, 3, i++, 'Q');
	add_piece(newBoard, King, black, 7, 4, i++, 'K');
	i = 0;
	add_piece(newBoard, Pawn, white, 1, 0, i++, 'm');
	add_piece(newBoard, Pawn, white, 1, 1, i++, 'm');
	add_piece(newBoard, Pawn, white, 1, 2, i++, 'm');
	add_piece(newBoard, Pawn, white, 1, 3, i++, 'm');
	add_piece(newBoard, Pawn, white, 1, 4, i++, 'm');
	add_piece(newBoard, Pawn, white, 1, 5, i++, 'm');
	add_piece(newBoard, Pawn, white, 1, 6, i++, 'm');
	add_piece(newBoard, Pawn, white, 1, 7, i++, 'm');
	add_piece(newBoard, Bishop, white, 0, 2, i++, 'b');
	add_piece(newBoard, Bishop, white, 0, 5, i++, 'b');
	add_piece(newBoard, Knight, white, 0, 1, i++, 'n');
	add_piece(newBoard, Knight, white, 0, 6, i++, 'n');
	add_piece(newBoard, Rock, white, 0, 0, i++, 'r');
	add_piece(newBoard, Rock, white, 0, 7, i++, 'r');
	add_piece(newBoard, Queen, white, 0, 3, i++, 'q');
	add_piece(newBoard, King, white, 0, 4, i++, 'k');
	Piece* Empty_piece = create_piece(Empty, -1, -1, -1, '_');
	for(int i = 2; i < 6; i++){
		for(int j = 0; j < 8; j++){
			newBoard->board[i][j] = Empty_piece;
		}
	}
	newBoard->turn = white;
	newBoard->empty = Empty_piece;
	set_all_valid_steps(newBoard);
	return newBoard;
}

void add_piece(Gameboard* gameboard, Piece_type type, int colur, int row, int col, int indexat, char sign){
	Piece* piece = create_piece(type, colur, row, col, sign);
	gameboard->board[row][col] = piece;
	gameboard->all_pieces[colur][indexat] = piece;
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
	Piece *curr;
	Piece* Empty_piece = create_piece(Empty, -1, -1, -1, '_');
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			newBoard->board[i][j] = Empty_piece;
		}
	}
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 16; j++){
			curr = copy_piece(old->all_pieces[i][j]);
			newBoard->board[curr->row][curr->col] = curr;
			newBoard->all_pieces[i][j] = curr;
		}
	}
	newBoard->history = ArrayListCopy(old->history);
	newBoard->turn = old->turn;
	newBoard->empty = Empty_piece;
	return newBoard;
}

Step create_step(int srow, int scol, int drow, int dcol, Piece *prev, bool is_srcPiece_was_moved){
	Step newStep;
	newStep.srow = srow;
	newStep.dcol = dcol;
	newStep.drow = drow;
	newStep.scol = scol;
	newStep.prevPiece = prev;
	newStep.is_srcPiece_was_moved = is_srcPiece_was_moved;
	return newStep;
}

CHESS_BOARD_MESSAGE set_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol) {
	if(!is_valid_step(gameboard, srow, scol, drow, dcol)){
		return CHESS_BOARD_INVALID_MOVE;
	}
	Piece *source_p = gameboard->board[srow][scol];
	Piece *dest_p = gameboard->board[drow][dcol];

	Step step = create_step(srow, scol, drow, dcol, dest_p, source_p->has_moved);
	ArrayListPushFirst(gameboard->history, step);

	gameboard->board[drow][dcol] = source_p;
	gameboard->board[srow][scol] = gameboard->empty;
	dest_p->alive = false;
	source_p->has_moved = true;
	source_p->row = drow;
	source_p->col = dcol;
	gameboard->turn = abs(1 - gameboard->turn);
	if(source_p->type == Pawn){
		source_p->vectors[0].vector_size = 1;
	}
	set_all_valid_steps(gameboard);
	return CHESS_BOARD_SUCCESS;
}

bool is_valid_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol){
	Piece *p = gameboard->board[srow][scol];
	if(p->colur != gameboard->turn){
		return false;
	}
	if(!p->alive){
		return false;
	}
	if(p->type == Empty){
		return false;
	}
	Step s;
	for(int i = 0; i < p->amount_steps; i++){
		 s = p->steps[i];
		 if(s.dcol == dcol && s.drow == drow){
			 return true;
		 }
	}
	return false;
}

bool is_check_curr_player(Gameboard *gameboard){
	return is_check(gameboard, gameboard->turn);
}

//is the player with color colur threating the other player?
bool is_check(Gameboard *gameboard, int colur) {
	for(int i = 0; i < 16; i++){
		Piece *piece = gameboard->all_pieces[colur][i];
		if(piece->alive){
			int amount_v = piece->amount_vectors; //check all vectors
			while(amount_v > 0){
				amount_v --;
				if(is_check_by_vector(gameboard, piece, piece->vectors[amount_v])){
					return true;
				}
			}
		}
	}
	return false;
}

bool is_check_per_vector(Gameboard *gameboard, Piece *piece, Vector_step v){
	int delta_row = v.delta_row;
	int delta_col = v.delta_col;
	int amount_going = v.vector_size;
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
		else{ // seeing another piece
			return false;
		}
	}
	return false;
}
//---

void set_all_valid_steps(Gameboard *gameboard){
	for(int i = 0; i < 16; i++){
		Piece *piece = gameboard->all_pieces[gameboard->turn][i];
		if(piece->alive){
			set_all_valid_steps_per_piece(gameboard, piece);
		}
	}
}

void set_all_valid_steps_per_piece(Gameboard *gameboard, Piece *piece) {
	int amount_steps = 0;
	for(int i = 0; i < piece->amount_vectors; i++){
		add_steps_by_vector(gameboard, piece, piece->vectors[i], &amount_steps);
	}
	piece->amount_steps = amount_steps;
}

void add_steps_per_vector(Gameboard *gameboard, Piece *piece, Vector_step v, int *amount_steps){
	int delta_row = v.delta_row;
	int delta_col = v.delta_col;
	int amount_going = v.vector_size;
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
			Step s = create_step(piece->row, piece->col, row, col, gameboard->empty, piece->has_moved);
			if(!is_step_causes_check(gameboard, piece, s)){
				piece->steps[*amount_steps] = s;
				(*amount_steps)++;
			}
		}
		else if(gameboard->board[row][col]->colur != piece->colur){ //eating opponent's piece
			Step s = create_step(piece->row, piece->col, row, col, gameboard->board[row][col], piece->has_moved);
			if(!is_step_causes_check(gameboard, piece, s)){
				piece->steps[*amount_steps] = s;
				(*amount_steps)++;
			}
			break;
		}
		else{ // seeing your color piece
			break;
		}
	}

}

bool is_step_causes_check(Gameboard* gameboard, Piece* piece, Step step){
	bool answer = false;
	gameboard->board[step.drow][step.dcol] = piece;
	gameboard->board[piece->row][piece->col] = gameboard->empty;
	gameboard->turn = abs(1-gameboard->turn);
	if(is_check_curr_player(gameboard)){
		answer = true;
	}
	gameboard->turn = abs(1-gameboard->turn);
	gameboard->board[step.drow][step.dcol] = step.prevPiece;
	gameboard->board[piece->row][piece->col] = piece;
	return answer;
}
//---

Piece *get_piece_in_place(Gameboard *gameboard, int row, int col) {
	if(row < 0 || row > 7 || col < 0 || col > 7){
		return NULL;
	}
	return gameboard->board[row][col];
}

CHESS_BOARD_MESSAGE undo_Step(Gameboard *gameboard) {
	if(gameboard == NULL){
		return CHESS_BOARD_INVALID_ARGUMENT;
	}
	if(ArrayListSize(gameboard->history) == 0){
		return CHESS_BOARD_NO_HISTORY;
	}
	Step step = ArrayListGetFirst(gameboard->history);
	ArrayListRemoveFirst(gameboard->history);

	Piece *source_p = gameboard->board[step.drow][step.dcol];
	Piece *dest_p = step.prevPiece;

	gameboard->board[step.drow][step.dcol] = dest_p;
	gameboard->board[step.srow][step.scol] = source_p;
	dest_p->alive = true;
	if(!step.is_srcPiece_was_moved){
		source_p->has_moved = false;
	}
	source_p->row = step.srow;
	source_p->col = step.scol;
	dest_p->row = step.drow;
	dest_p->col = step.dcol;
	gameboard->turn = abs(1 - gameboard->turn);
	if(source_p->type == Pawn && !source_p->has_moved){
		source_p->vectors[0].vector_size = 2;
	}
	set_all_valid_steps(gameboard);
	return CHESS_BOARD_SUCCESS;
}

// return the winner's color
// if tie return 2
// if not game over return -1
int is_game_over(Gameboard *gameboard) {
	for(int i = 0; i < 16; i++){ //is there any piece that has legal move?
		Piece *p = gameboard->all_pieces[gameboard->turn][i];
		if(p->amount_steps > 0 && p->alive){
			return -1;
		}
	}
	if(is_check(gameboard, abs(1 - gameboard->turn))){ //is the other player threating me with check?
		return abs(1 - gameboard->turn);
	}
	return 2;
}

void print_board(Gameboard *gameboard) {
	for(int i = 7; i >= 0; i--){
		printf("%d| ", i + 1);
		fflush(stdout);
		for(int j = 0; j < 8; j++){
			printf("%c ", gameboard->board[i][j]->sign);
			fflush(stdout);
		}
		printf("|\n", i + 1);
		fflush(stdout);
	}
	printf("  -----------------\n");
	printf("   A B C D E F G H");
	fflush(stdout);
}
