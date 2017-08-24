/*
 * Board.c
 *
 *  Created on: 9 баев„ 2017
 *      Author: User
 */

//set step, undo step
#include "GameBoard.h"
#define MIN(A,B) (((A) < (B)) ? (A) : (B))
#define MAX(A,B) (((A) > (B)) ? (A) : (B))

Gameboard *create_board(int game_mode, int difficulty, int user_color) {
	Gameboard *newBoard = (Gameboard*) malloc(sizeof(Gameboard));
	assert(newBoard != NULL);
	newBoard->history = ArrayListCreate(history_size * 2);
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
	newBoard->empty = (Piece *) create_piece(Empty, -1, -1, -1, '_', -1);
	for(int i = 2; i < 6; i++){
		for(int j = 0; j < 8; j++){
			newBoard->board[i][j] = newBoard->empty;
		}
	}
	newBoard->turn = white;
	newBoard->user_color = user_color;
	newBoard->difficulty = difficulty;
	newBoard->game_mode = game_mode;
	set_all_valid_steps(newBoard);
	return newBoard;
}

void add_piece(Gameboard* gameboard, Piece_type type, int colur, int row, int col, int indexat, char sign){
	Piece *piece = (Piece *) create_piece(type, colur, row, col, sign, indexat);
	gameboard->board[row][col] = piece;
	gameboard->all_pieces[colur][indexat] = piece;
}

void destroy_board(Gameboard *gameboard) {
	if(gameboard == NULL){
		return;
	}
	ArrayListDestroy(gameboard->history);
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 16; j++){
			destroy_piece(gameboard->all_pieces[i][j]);
		}
	}
	destroy_piece(gameboard->empty);
	free(gameboard);
}

Gameboard *copy_board(Gameboard* old) {
	if(old == NULL){
		return NULL;
	}
	Gameboard *new = (Gameboard*) malloc(sizeof(Gameboard));
	Piece *empty = create_piece(Empty, -1 ,-1 ,-1 ,'_', -1);
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			new->board[i][j] = empty;
		}
	}
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 16; j++){
			new->all_pieces[i][j] = copy_piece(old->all_pieces[i][j]);
			Piece *curr = new->all_pieces[i][j];
			if(curr->alive){
				new->board[curr->row][curr->col] = curr;
			}
		}
	}
	new->turn = old->turn;
	new->empty = empty;
	//when we copy history, inside the steps we still point to piece in previous board
	//so the loop changes it:
	new->history = ArrayListCopy(old->history);
	for(int i = 0; i < new->history->actualSize; i++){
		Piece* old_piece = old->history->elements[i]->prevPiece;
		if(old_piece->type == Empty){
			new->history->elements[i]->prevPiece = new->empty;
		}
		else{
			new->history->elements[i]->prevPiece = new->all_pieces[old_piece->colur][old_piece->indexat];
		}
	}
	//new->history
	new->user_color = old->user_color;
	new->difficulty = old->difficulty;
	new->game_mode = old->game_mode;
	set_all_valid_steps(new);
	return new;
}

CHESS_BOARD_MESSAGE set_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol) {
	CHESS_BOARD_MESSAGE cbm = is_valid_step(gameboard, srow, scol, drow, dcol);
	if(cbm != CHESS_BOARD_SUCCESS){
		return cbm;
	}
	Piece *source_p = gameboard->board[srow][scol];
	Piece *dest_p = gameboard->board[drow][dcol];

	Step *step = create_step(srow, scol, drow, dcol, dest_p, source_p->has_moved);
	ArrayListPushFirst(gameboard->history, step);

	gameboard->board[drow][dcol] = source_p;
	gameboard->board[srow][scol] = gameboard->empty;
	dest_p->alive = false;
	source_p->has_moved = true;
	source_p->row = drow;
	source_p->col = dcol;
	gameboard->turn = abs(1 - gameboard->turn);
	if(source_p->type == Pawn){
		source_p->vectors[0]->vector_size = 1;
	}
	set_all_valid_steps(gameboard);
	return CHESS_BOARD_SUCCESS;
}

CHESS_BOARD_MESSAGE is_valid_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol){
	Piece *p = gameboard->board[srow][scol];
	if(p->colur != gameboard->turn){
		return CHESS_BOARD_INVALID_MOVE_NO_PIECE;
	}
	if(!p->alive){
		return CHESS_BOARD_INVALID_MOVE_NO_PIECE;
	}
	if(p->type == Empty){
		return CHESS_BOARD_INVALID_MOVE_NO_PIECE;
	}
	Step *s;
	for(int i = 0; i < p->amount_steps; i++){
		 s = p->steps[i];
		 if(s->dcol == dcol && s->drow == drow){
			 return CHESS_BOARD_SUCCESS;
		 }
	}
	return CHESS_BOARD_INVALID_MOVE_RULES_VIOLATION;
}

bool is_check_curr_player(Gameboard *gameboard){
	return is_check(gameboard, gameboard->turn);
}

bool is_under_check(Gameboard * gameboard){
	return is_check(gameboard, abs(1 - gameboard->turn));
}

//is the player with color colur threating the other player?
bool is_check(Gameboard *gameboard, int colur) {
	for(int i = 0; i < 16; i++){
		Piece *piece = gameboard->all_pieces[colur][i];
		if(piece->alive){
			int amount_v = piece->amount_vectors; //check all vectors
			while(amount_v > 0){
				amount_v --;
				if( is_check_per_vector(gameboard, piece, piece->vectors[amount_v]) ){
					return true;
				}
			}
		}
	}
	return false;
}

bool is_check_per_vector(Gameboard *gameboard, Piece *piece, Vector *v){
	int delta_row = v->delta_row;
	int delta_col = v->delta_col;
	int amount_going = v->vector_size;
	bool can_eat = v->can_eat;
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
		else if(can_eat && gameboard->board[row][col]->type == King &&
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
	//set_hazraha_steps(gameboard);
}

void set_all_valid_steps_per_piece(Gameboard *gameboard, Piece *piece) {
	for(int i = 0; i < piece->amount_steps; i++){
		destroy_step(piece->steps[i]);
		piece->steps[i] = NULL;
	}
	int amount_steps = 0;
	for(int i = 0; i < piece->amount_vectors; i++){
		add_steps_per_vector(gameboard, piece, piece->vectors[i], &amount_steps);
	}
	piece->amount_steps = amount_steps;
}

void add_steps_per_vector(Gameboard *gameboard, Piece *piece, Vector *v, int *amount_steps){
	int delta_row = v->delta_row;
	int delta_col = v->delta_col;
	int amount_going = v->vector_size;
	bool can_eat = v->can_eat;
	bool can_go_to_empty_spot = v->can_go_to_empty_spot;
	int row = piece->row;
	int col = piece->col;
	while(amount_going > 0){
		amount_going --;
		row = row + delta_row;
		col = col + delta_col;
		if(row < 0 || row > 7 || col < 0 || col > 7){ //out of board
			break;
		}
		if(gameboard->board[row][col]->type == Empty && can_go_to_empty_spot){ // can go, empty
			Step *s = create_step(piece->row, piece->col, row, col, gameboard->empty, piece->has_moved);
			if(!is_step_causes_check(gameboard, piece, s)){
				piece->steps[*amount_steps] = s;
				(*amount_steps)++;
			}
		}
		else if(gameboard->board[row][col]->type != Empty &&
				gameboard->board[row][col]->colur != piece->colur && can_eat){ //eating opponent's piece
			Step *s = create_step(piece->row, piece->col, row, col, gameboard->board[row][col], piece->has_moved);
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

bool is_step_causes_check(Gameboard* gameboard, Piece* piece, Step* step){
	bool answer = false;
	gameboard->board[step->drow][step->dcol] = piece;
	gameboard->board[piece->row][piece->col] = gameboard->empty;
	step->prevPiece->alive = false;
	gameboard->turn = abs(1-gameboard->turn);
	if(is_check_curr_player(gameboard)){
		answer = true;
	}
	gameboard->turn = abs(1-gameboard->turn);
	step->prevPiece->alive = true;
	gameboard->board[step->drow][step->dcol] = step->prevPiece;
	gameboard->board[piece->row][piece->col] = piece;
	return answer;
}

void set_hazraha_steps(Gameboard * gameboard){
	int turn = gameboard->turn;
	Piece * king = gameboard->all_pieces[turn][15];
	if(king->has_moved) return;
	if(is_under_check(gameboard)) return;
	if(!king->alive) return;
	Piece * rock;
	int delta_col;
	for(int i = 0; i <= 1; i++){ //go over the two rocks
		rock = gameboard->all_pieces[turn][12+i];
		if(is_hazraha_valid_with_rock_curr_player(gameboard, king, rock)){
			delta_col = (king->col < rock->col) ? 2 : -2;
			Step * new_step = create_step(king->row, king->col, king->row, delta_col, gameboard->empty, false);
			king->steps[king->amount_steps] = new_step;
			king->amount_steps++;
		}
	}

}

bool is_hazraha_valid_with_rock_curr_player(Gameboard * gameboard, Piece* king, Piece* rock){
	if(rock->has_moved) return false;
	if(!rock->alive) return false;
	int row = king->row;
	int left_col = MIN(king->col, rock->col);
	int right_col = MAX(king->col, rock->col);
	for(int i = left_col + 1; i < right_col; i++){//there's no tool between rock and king
		if(gameboard->board[row][i] != gameboard->empty){
			return false;
		}
	}
	int delta_col = (king->col < rock->col) ? 1 : -1;
	int col = king->col;
	int new_col;
	bool result = true;
	for(int i = 1; i <=2 ; i++){ //there's no check in king's path
		new_col = king->col + i * delta_col;
		gameboard->board[row][col] = gameboard->empty;
		gameboard->board[row][new_col] = king;
		king->col = new_col;
		if(is_under_check) result = false;
		king->col = col;
		gameboard->board[row][new_col] = gameboard->empty;
		gameboard->board[row][col] = king;
	}
	return result;
}

//---

Piece *get_piece_in_place(Gameboard *gameboard, int row, int col) {
	if(row < 0 || row > 7 || col < 0 || col > 7){
		return NULL;
	}
	return gameboard->board[row][col];
}

CHESS_BOARD_MESSAGE undo_step(Gameboard *gameboard) {
	if(gameboard == NULL){
		return CHESS_BOARD_INVALID_ARGUMENT;
	}
	if(ArrayListSize(gameboard->history) == 0){
		return CHESS_BOARD_NO_HISTORY;
	}
	Step *step = ArrayListGetFirst(gameboard->history);
	ArrayListRemoveFirst(gameboard->history);

	Piece *source_p = gameboard->board[step->drow][step->dcol];
	Piece *dest_p = step->prevPiece;

	gameboard->board[step->drow][step->dcol] = dest_p;
	gameboard->board[step->srow][step->scol] = source_p;
	dest_p->alive = true;
	if(!step->is_srcPiece_was_moved){
		source_p->has_moved = false;
	}
	source_p->row = step->srow;
	source_p->col = step->scol;
	dest_p->row = step->drow;
	dest_p->col = step->dcol;
	gameboard->turn = abs(1 - gameboard->turn);
	if(source_p->type == Pawn && !source_p->has_moved){
		source_p->vectors[0]->vector_size = 2;
	}
	set_all_valid_steps(gameboard);
	return CHESS_BOARD_SUCCESS;
}

// return the winner's color
// if tie return 2
// if not game over return -1
int is_game_over(Gameboard *gameboard) {
	for(int i = 0; i < 16; i++){ // is there any piece that has legal move?
		Piece *p = gameboard->all_pieces[gameboard->turn][i];
		if(p->amount_steps > 0 && p->alive){
			return -1;
		}
	}
	if(is_check(gameboard, abs(1 - gameboard->turn))){ // is my kind under check?
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
		printf("|\n");
		fflush(stdout);
	}
	printf("  -----------------\n");
	printf("   A B C D E F G H\n\n");
	fflush(stdout);
}

void print_details_game(Gameboard *gameboard){
	printf("\n");
	fflush(stdout);
	print_board(gameboard);
	printf("history: ");
	fflush(stdout);
	ArrayListPrint(gameboard->history);
	printf("all valid steps:\n");
	fflush(stdout);
	for(int i = 0; i < 16; i++){
		printf("%c: ", gameboard->all_pieces[gameboard->turn][i]->sign);
		fflush(stdout);
		print_all_steps(gameboard->all_pieces[gameboard->turn][i]);
		printf("\n");
		fflush(stdout);
	}
}
