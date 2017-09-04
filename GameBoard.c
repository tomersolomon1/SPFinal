
#include "GameBoard.h"
#define MIN(A,B) (((A) < (B)) ? (A) : (B))
#define MAX(A,B) (((A) > (B)) ? (A) : (B))
#define TURN_SWITCHED abs(1 - gameboard->turn)

//-----------------------Game Board General functions-----------------------

Gameboard *create_board(int game_mode, int difficulty, int user_color) {
	Gameboard *newBoard = (Gameboard*) malloc(sizeof(Gameboard));
	assert(newBoard != NULL);
	newBoard->history = ArrayListCreate(HISTORY_SIZE * 2);
	int i = 0;
	add_piece(newBoard, Pawn, black, 6, 0, i++);
	add_piece(newBoard, Pawn, black, 6, 1, i++);
	add_piece(newBoard, Pawn, black, 6, 2, i++);
	add_piece(newBoard, Pawn, black, 6, 3, i++);
	add_piece(newBoard, Pawn, black, 6, 4, i++);
	add_piece(newBoard, Pawn, black, 6, 5, i++);
	add_piece(newBoard, Pawn, black, 6, 6, i++);
	add_piece(newBoard, Pawn, black, 6, 7, i++);
	add_piece(newBoard, Bishop, black, 7, 2, i++);
	add_piece(newBoard, Bishop, black, 7, 5, i++);
	add_piece(newBoard, Knight, black, 7, 1, i++);
	add_piece(newBoard, Knight, black, 7, 6, i++);
	add_piece(newBoard, Rock, black, 7, 0, i++);
	add_piece(newBoard, Rock, black, 7, 7, i++);
	add_piece(newBoard, Queen, black, 7, 3, i++);
	add_piece(newBoard, King, black, 7, 4, i++);
	i = 0;
	add_piece(newBoard, Pawn, white, 1, 0, i++);
	add_piece(newBoard, Pawn, white, 1, 1, i++);
	add_piece(newBoard, Pawn, white, 1, 2, i++);
	add_piece(newBoard, Pawn, white, 1, 3, i++);
	add_piece(newBoard, Pawn, white, 1, 4, i++);
	add_piece(newBoard, Pawn, white, 1, 5, i++);
	add_piece(newBoard, Pawn, white, 1, 6, i++);
	add_piece(newBoard, Pawn, white, 1, 7, i++);
	add_piece(newBoard, Bishop, white, 0, 2, i++);
	add_piece(newBoard, Bishop, white, 0, 5, i++);
	add_piece(newBoard, Knight, white, 0, 1, i++);
	add_piece(newBoard, Knight, white, 0, 6, i++);
	add_piece(newBoard, Rock, white, 0, 0, i++);
	add_piece(newBoard, Rock, white, 0, 7, i++);
	add_piece(newBoard, Queen, white, 0, 3, i++);
	add_piece(newBoard, King, white, 0, 4, i++);
	newBoard->empty = (Piece *) create_piece(Empty, -1, -1, -1, -1);
	for(int i = 2; i < 6; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
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

void add_piece(Gameboard* gameboard, Piece_type type, int colur, int row, int col, int indexat){
	Piece *piece = (Piece *) create_piece(type, colur, row, col, indexat);
	gameboard->board[row][col] = piece;
	gameboard->all_pieces[colur][indexat] = piece;
}

void destroy_board(Gameboard *gameboard) {
	if(gameboard == NULL){
		return;
	}
	ArrayListDestroy(gameboard->history);
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < AMOUNT_PIECES_PER_COLOR; j++){
			destroy_piece(gameboard->all_pieces[i][j]);
		}
	}
	destroy_piece(gameboard->empty);
	free(gameboard);
}

Gameboard *copy_board(Gameboard* old) {
	if(old == NULL)
		return NULL;
	Gameboard *new = (Gameboard*) malloc(sizeof(Gameboard));
	Piece *empty = create_piece(Empty, -1 ,-1 ,-1 , -1);
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++)
			new->board[i][j] = empty;
	}
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < AMOUNT_PIECES_PER_COLOR; j++){
			new->all_pieces[i][j] = copy_piece(old->all_pieces[i][j]);
			Piece *curr = new->all_pieces[i][j];
			if(curr->alive)
				new->board[curr->row][curr->col] = curr;
		}
	}
	new->turn = old->turn;
	new->empty = empty;
	//when we copy history, inside the steps we still point to piece in previous board
	//so the loop changes it:
	new->history = ArrayListCopy(old->history);
	for(int i = 0; i < new->history->actualSize; i++){
		Piece* old_piece = old->history->elements[i]->prevPiece;
		if(old_piece->type == Empty)
			new->history->elements[i]->prevPiece = new->empty;
		else
			new->history->elements[i]->prevPiece = new->all_pieces[old_piece->colur][old_piece->indexat];
	}
	//new->history
	new->user_color = old->user_color;
	new->difficulty = old->difficulty;
	new->game_mode = old->game_mode;
	set_all_valid_steps(new);
	return new;
}

void reset_board(Gameboard** gameboard){
	int game_mode = (*gameboard)->game_mode;
	int difficulty = (*gameboard)->difficulty;
	int user_color = (*gameboard)->user_color;
	destroy_board(*gameboard);
	*gameboard = create_board(game_mode, difficulty, user_color);
}

//-----------------------Set Step-----------------------

CHESS_BOARD_MESSAGE set_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol) {
	CHESS_BOARD_MESSAGE cbm = is_valid_step(gameboard, srow, scol, drow, dcol);
	if(cbm != CHESS_BOARD_SUCCESS){
		return cbm;
	}
	Piece_state src_prev_state = Was_not_moved;
	Piece *source_p = gameboard->board[srow][scol];
	Piece *dest_p = gameboard->board[drow][dcol];
	if(source_p->has_moved)
		src_prev_state = Was_moved;
	if((drow == (BOARD_SIZE - 1) || drow == 0) && source_p->type == Pawn) //promotion
		src_prev_state = Was_promoted;
	Step *step = create_step(srow, scol, drow, dcol, dest_p, src_prev_state, false);
	step->is_threatened = is_step_threatened(gameboard, source_p, step);
	ArrayListPushFirst(gameboard->history, step);

	if(source_p->type == King && abs(dcol - scol) > 1 && srow == drow) //castling
		set_castling_move(gameboard, srow, scol, dcol);
	else{
		gameboard->board[drow][dcol] = source_p;
		gameboard->board[srow][scol] = gameboard->empty;
		dest_p->alive = false;
		source_p->has_moved = true;
		source_p->row = drow;
		source_p->col = dcol;
		if(source_p->type == Pawn)
			source_p->vectors[0]->vector_size = 1;
	}
	gameboard->turn = TURN_SWITCHED;
	set_all_valid_steps(gameboard);
	if(src_prev_state == Was_promoted)
		return CHESS_BOARD_PROMOTION;
	return CHESS_BOARD_SUCCESS;
}

void make_promotion(Gameboard *gameboard, int row, int col, Piece_type new_type){
	Piece *piece = gameboard->board[row][col];
	change_piece_type(piece, new_type);
	set_all_valid_steps(gameboard);
}

void set_castling_move(Gameboard *gameboard, int row, int scol, int dcol){
	int side_rock = ((scol < dcol) ? 1 : -1); // -1 = left to king, 1 = right to king,
	int scol_rock = ((scol < dcol) ? 7 : 0);
	Piece *rock = gameboard->board[row][scol_rock];
	Piece *king = gameboard->board[row][scol];
	gameboard->board[row][scol] = gameboard->empty;
	gameboard->board[row][dcol] = king;
	king->col = dcol;
	gameboard->board[row][scol_rock] = gameboard->empty;
	gameboard->board[row][dcol - side_rock] = rock;
	rock->col = dcol - side_rock;
	rock->has_moved = true;
	king->has_moved = true;
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

//-----------------------Is (the other player) threatening piece-----------------------

bool is_threatening_piece(Gameboard* gameboard, Piece *threatened){
	for(int i = 0; i < AMOUNT_PIECES_PER_COLOR; i++){
		Piece *attacking = gameboard->all_pieces[abs(1 - threatened->colur)][i];
		if(attacking->alive){
			int amount_v = attacking->amount_vectors; //check all vectors
			while(amount_v > 0){
				amount_v --;
				if(is_threatening_piece_per_vector(gameboard, threatened, attacking, attacking->vectors[amount_v])){
					return true;
				}
			}
		}
	}
	return false;
}

bool is_threatening_piece_per_vector(Gameboard* gameboard, Piece *threatened, Piece* attacking, Vector* v_attacking){
	int delta_row = v_attacking->delta_row;
	int delta_col = v_attacking->delta_col;
	int amount_going = v_attacking->vector_size;
	bool can_eat = v_attacking->can_eat;
	bool can_go_to_empty = v_attacking->can_go_to_empty_spot;
	int row = attacking->row;
	int col = attacking->col;
	while(amount_going > 0){
		amount_going --;
		row = row + delta_row;
		col = col + delta_col;
		if(row < 0 || row > (BOARD_SIZE - 1) || col < 0 || col > (BOARD_SIZE - 1)){ //out of board
			return false;
		}
		else if(gameboard->board[row][col]->type == Empty && can_go_to_empty){ // can go, empty
			continue;
		}
		else if(can_eat && gameboard->board[row][col] == threatened){ //eating opponent's threatened
			return true;
		}
		else{ // seeing another piece
			return false;
		}
	}
	return false;
}

bool is_check_curr_player(Gameboard *gameboard){
	return is_check(gameboard, gameboard->turn);
}

bool is_under_check(Gameboard * gameboard){
	return is_check(gameboard, TURN_SWITCHED);
}

bool is_check(Gameboard *gameboard, int colur) {
	Piece* king_threatened = gameboard->all_pieces[abs(1 - colur)][15]; //15 is the place of king in all_pieces
	return is_threatening_piece(gameboard, king_threatened);
}

//-----------------------Set all Valid Steps-----------------------

void set_all_valid_steps(Gameboard *gameboard){
	for(int i = 0; i < AMOUNT_PIECES_PER_COLOR; i++){
		Piece *piece = gameboard->all_pieces[gameboard->turn][i];
		if(piece->alive){
			set_all_valid_steps_per_piece(gameboard, piece);
		}
	}
	set_castling_steps(gameboard);
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
	Piece_state piece_state = (piece->has_moved? Was_moved: Was_not_moved);
	while(amount_going > 0){
		amount_going --;
		row = row + delta_row;
		col = col + delta_col;
		if(row < 0 || row > (BOARD_SIZE - 1) || col < 0 || col > (BOARD_SIZE - 1)) //out of board
			break;
		if(gameboard->board[row][col]->type == Empty && can_go_to_empty_spot){ // can go, empty
			Step *s = create_step(piece->row, piece->col, row, col, gameboard->empty, piece_state, true);
			if(!is_step_causes_check(gameboard, piece, s)){
				s->is_threatened = is_step_threatened(gameboard, piece, s);
				piece->steps[*amount_steps] = s;
				(*amount_steps)++;
			}
		}
		else if(gameboard->board[row][col]->type != Empty &&
				gameboard->board[row][col]->colur != piece->colur && can_eat){ //eating opponent's piece
			Step *s = create_step(piece->row, piece->col, row, col, gameboard->board[row][col], piece_state, true);
			if(!is_step_causes_check(gameboard, piece, s)){
				s->is_threatened = is_step_threatened(gameboard, piece, s);
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
	gameboard->turn = TURN_SWITCHED;
	if(is_check_curr_player(gameboard)){
		answer = true;
	}
	gameboard->turn = TURN_SWITCHED;
	step->prevPiece->alive = true;
	gameboard->board[step->drow][step->dcol] = step->prevPiece;
	gameboard->board[piece->row][piece->col] = piece;
	return answer;
}

bool is_step_threatened(Gameboard* gameboard, Piece* piece, Step* step){
	bool answer = false;
	gameboard->board[step->drow][step->dcol] = piece;
	gameboard->board[piece->row][piece->col] = gameboard->empty;
	step->prevPiece->alive = false;
	gameboard->turn = TURN_SWITCHED;
	if(is_threatening_piece(gameboard, piece)){
		answer = true;
	}
	gameboard->turn = TURN_SWITCHED;
	step->prevPiece->alive = true;
	gameboard->board[step->drow][step->dcol] = step->prevPiece;
	gameboard->board[piece->row][piece->col] = piece;
	return answer;
}

void set_castling_steps(Gameboard * gameboard){
	int turn = gameboard->turn;
	Piece *king = gameboard->all_pieces[turn][15];
	if(king->has_moved) return;
	if(is_under_check(gameboard)) return;
	if(!king->alive) return;
	Piece *rock;
	int delta_col;
	for(int i = 0; i <= 1; i++){ //go over the two rocks
		rock = gameboard->all_pieces[turn][12 + i]; //12 is the 1st place of rock in all_pieces
		if(is_castling_valid_per_rock(gameboard, king, rock)){
			delta_col = (king->col < rock->col) ? 2 : -2;
			Step *new_step = create_step(king->row, king->col, king->row, king->col + delta_col, gameboard->empty, Was_not_moved, true);
			new_step->is_threatened = is_step_threatened(gameboard, rock, new_step);
			king->steps[king->amount_steps] = new_step;
			king->amount_steps++;
		}
	}

}

bool is_castling_valid_per_rock(Gameboard * gameboard, Piece* king, Piece* rock){
	if(rock->has_moved) return false;
	if(!rock->alive) return false;
	int row = king->row;
	int left_col = MIN(king->col, rock->col);
	int right_col = MAX(king->col, rock->col);
	for(int i = left_col + 1; i < right_col; i++){//there's no tool between rock and king
		if(gameboard->board[row][i]->type != Empty){
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
		if(is_under_check(gameboard)) result = false;
		king->col = col;
		gameboard->board[row][new_col] = gameboard->empty;
		gameboard->board[row][col] = king;
	}
	return result;
}

//-----------------------Undo-----------------------

CHESS_BOARD_MESSAGE undo_step(Gameboard *gameboard) {
	if(gameboard == NULL){
		return CHESS_BOARD_INVALID_ARGUMENT;
	}
	if(ArrayListSize(gameboard->history) == 0){
		return CHESS_BOARD_NO_HISTORY;
	}
	Step *step = ArrayListGetFirst(gameboard->history);
	Piece *source_p = gameboard->board[step->drow][step->dcol];

	if(source_p->type == King && abs(step->dcol - step->scol) > 1){ //castling
		undo_step_castling(gameboard, step);
	}
	else{ //not castling
		Piece *dest_p = step->prevPiece;
		gameboard->board[step->drow][step->dcol] = dest_p;
		gameboard->board[step->srow][step->scol] = source_p;
		dest_p->alive = true;
		if(step->src_previous_state == Was_not_moved)
			source_p->has_moved = false;
		else if(step->src_previous_state == Was_promoted)
			change_piece_type(source_p, Pawn);
		source_p->row = step->srow;
		source_p->col = step->scol;
		dest_p->row = step->drow;
		dest_p->col = step->dcol;

		if(source_p->type == Pawn && !source_p->has_moved){
			source_p->vectors[0]->vector_size = 2;
		}
	}
	gameboard->turn = TURN_SWITCHED;
	ArrayListRemoveFirst(gameboard->history);
	set_all_valid_steps(gameboard);
	return CHESS_BOARD_SUCCESS;
}

CHESS_BOARD_MESSAGE double_undo(Gameboard *gameboard) {
	if(gameboard == NULL){
		return CHESS_BOARD_INVALID_ARGUMENT;
	}
	if (ArrayListSize(gameboard->history) < 2) {
		return CHESS_BOARD_NO_HISTORY;
	}
	undo_step(gameboard);
	undo_step(gameboard);
	return CHESS_BOARD_SUCCESS;
}

void undo_step_castling(Gameboard *gameboard, Step *step){
	int row = step->drow;
	int king_dcol = step->dcol;
	int rock_dcol = (king_dcol == 2 ? 3 : 5);
	int rock_scol = (king_dcol == 2 ? 0 : 7);
	Piece *king = gameboard->board[row][king_dcol];
	Piece *rock = gameboard->board[row][rock_dcol];
	gameboard->board[row][king_dcol] = gameboard->empty;
	gameboard->board[row][rock_dcol] = gameboard->empty;

	gameboard->board[row][step->scol] = king;
	king->col = step->scol;
	gameboard->board[row][rock_scol] = rock;
	rock->col = rock_scol;
	rock->has_moved = false;
	king->has_moved = false;
}

//-----------------------Is Game Over-----------------------

int is_game_over(Gameboard *gameboard) {
	for(int i = 0; i < AMOUNT_PIECES_PER_COLOR; i++){ // is there any piece that has legal move?
		Piece *p = gameboard->all_pieces[gameboard->turn][i];
		if(p->amount_steps > 0 && p->alive){
			return -1;
		}
	}
	if(is_check(gameboard, TURN_SWITCHED)){ // is my kind under check?
		return TURN_SWITCHED;
	}
	return 2;
}

//-----------------------Print Board-----------------------

void print_board(Gameboard *gameboard) {
	for(int i = (BOARD_SIZE - 1); i >= 0; i--){
		printf("%d| ", i + 1);
		fflush(stdout);
		for(int j = 0; j < BOARD_SIZE; j++){
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
	for(int i = 0; i < AMOUNT_PIECES_PER_COLOR; i++){
		printf("%c: ", gameboard->all_pieces[gameboard->turn][i]->sign);
		fflush(stdout);
		print_all_steps(gameboard->all_pieces[gameboard->turn][i]);
		printf("\n");
		fflush(stdout);
	}
}
