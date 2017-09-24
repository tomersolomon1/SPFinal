
#include "GameBoard.h"

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
	add_piece(newBoard, Rook, black, 7, 0, i++);
	add_piece(newBoard, Rook, black, 7, 7, i++);
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
	add_piece(newBoard, Rook, white, 0, 0, i++);
	add_piece(newBoard, Rook, white, 0, 7, i++);
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

void change_game_difficulty(Gameboard* gameboard, int new_difficulty){
	if(new_difficulty < 1 || new_difficulty > 4)
		return;
	gameboard->difficulty = new_difficulty;
	if(new_difficulty == 1 || new_difficulty == 2)
		set_all_valid_steps(gameboard);
}

//-----------------------Set Step-----------------------

CHESS_BOARD_MESSAGE set_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol, bool is_minimax){
	if(!is_minimax){
		CHESS_BOARD_MESSAGE cbm = is_valid_step(gameboard, srow, scol, drow, dcol);
		if(cbm != CHESS_BOARD_SUCCESS)
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
		if(source_p->type == Pawn)//change pawn to be able to move only 1 step forward
			source_p->vectors[0]->vector_size = 1;
	}
	gameboard->turn = SWITCHED(gameboard->turn);
	if(!is_minimax)
		set_all_valid_steps(gameboard);
	if(src_prev_state == Was_promoted)
		return CHESS_BOARD_PROMOTION;
	return CHESS_BOARD_SUCCESS;
}

void make_promotion(Gameboard *gameboard, int row, int col, Piece_type new_type) {
	Piece *piece = gameboard->board[row][col];
	change_piece_type(piece, new_type);
	set_all_valid_steps(gameboard);
}

CHESS_BOARD_MESSAGE commit_move(Gameboard *gameboard, int srow,
		int scol, int drow, int dcol, bool is_minimax, Piece_type promote_to) {
	//is_minimax = true;
	CHESS_BOARD_MESSAGE msg = set_step(gameboard, srow, scol, drow, dcol, is_minimax);
	if (msg == CHESS_BOARD_PROMOTION && promote_to != Empty) { /* Empty signals that the promotion was made by the user */
		make_promotion(gameboard, drow, dcol, promote_to);
	}
	return msg;
}

void set_castling_move(Gameboard *gameboard, int row, int scol, int dcol){
	int side_rook = ((scol < dcol) ? 1 : -1); // -1 = rook is left to king, 1 = rook is right to king,
	int scol_rook = ((scol < dcol) ? 7 : 0);
	Piece *rook = gameboard->board[row][scol_rook];
	Piece *king = gameboard->board[row][scol];
	gameboard->board[row][scol] = gameboard->empty;
	gameboard->board[row][dcol] = king;
	king->col = dcol;
	gameboard->board[row][scol_rook] = gameboard->empty;
	gameboard->board[row][dcol - side_rook] = rook;
	rook->col = dcol - side_rook;
	rook->has_moved = true;
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
		Piece *attacking = gameboard->all_pieces[SWITCHED(threatened->colur)][i];
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
	return is_check(gameboard, SWITCHED(gameboard->turn));
}

bool is_check(Gameboard *gameboard, int colur) {
	Piece* king_threatened = gameboard->all_pieces[SWITCHED(colur)][15]; //15 is the place of king in all_pieces
	return is_threatening_piece(gameboard, king_threatened);
}


//-----------------------MINIMAX-----------------------

Step **get_all_valid_steps_of_piece_minimax(Gameboard *gameboard, Piece *piece, int *amount_steps){
	*amount_steps = 0;
	int max_amount_steps = amount_steps_of_piece_type(piece->type);

	Step **all_piece_steps = (Step**) malloc(sizeof(Step*) * max_amount_steps);
	assert(all_piece_steps != NULL);
	for(int i = 0; i < max_amount_steps; i++){
		all_piece_steps[i] = NULL;
	}
	if(piece->type == King){
		set_castling_steps(gameboard, piece, all_piece_steps, amount_steps);
	}
	for(int i = 0; i < piece->amount_vectors; i++){
		add_steps_per_vector(gameboard, piece, piece->vectors[i], amount_steps, all_piece_steps, false);
	}
	return all_piece_steps;
}

void free_all_valid_steps_minimax(Step** all_steps, Piece_type type){
	int max_amount_steps = amount_steps_of_piece_type(type);
	for(int i = 0; i < max_amount_steps; i++){
		destroy_step(all_steps[i]);
	}
	free(all_steps);
}


int is_game_over_minimax(Gameboard *gameboard){
//	write_to_log_file("is_game_over\n");
	for(int i = AMOUNT_PIECES_PER_COLOR - 1; i >= 0 ; i--){ // is there any piece that has legal move?
		Piece *p = gameboard->all_pieces[gameboard->turn][i];
		if(p->alive){
			if(is_piece_having_legal_move_minimax(gameboard, p))
				return -1;
		}
	}
//	write_to_log_file("game_over_minimax != -1\n");
	if(is_check(gameboard, SWITCHED(gameboard->turn))){ // is my king under check?
		return SWITCHED(gameboard->turn);
	}
	return 2;
}

bool is_piece_having_legal_move_minimax(Gameboard *gameboard, Piece *piece){
//	write_to_log_file("is_piece_having_legal_move_minimax\n");
	for(int i = 0; i < piece->amount_vectors; i++){
		if(is_piece_having_legal_move_per_vector_minimax(gameboard, piece, piece->vectors[i])){
			return true;
		}
	}
	return false;
}

bool is_piece_having_legal_move_per_vector_minimax(Gameboard *gameboard, Piece *piece, Vector *v){
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
		if(row < 0 || row > (BOARD_SIZE - 1) || col < 0 || col > (BOARD_SIZE - 1)) //out of board
			break;
		if(gameboard->board[row][col]->type == Empty && can_go_to_empty_spot){ // can go, empty
			if(!is_step_causes_check(gameboard, piece, row, col, gameboard->empty)){
				return true;
			}
		}
		else if(gameboard->board[row][col]->type != Empty &&
				gameboard->board[row][col]->colur != piece->colur && can_eat){ //eating opponent's piece
			if(!is_step_causes_check(gameboard, piece, row, col, gameboard->board[row][col])){
				return true;
			}
			break;
		}
		else{ // seeing your color piece
			break;
		}
	}
	return false;
}

//-----------------------Set all Valid Steps-----------------------

void set_all_valid_steps(Gameboard *gameboard){
	for(int i = 0; i < AMOUNT_PIECES_PER_COLOR; i++){
		Piece *piece = gameboard->all_pieces[gameboard->turn][i];
		if(piece->alive){
			set_all_valid_steps_per_piece(gameboard, piece);
		}
	}
	Piece *king = gameboard->all_pieces[gameboard->turn][15]; //15 is the place of king in all_pieces
	set_castling_steps(gameboard, king, king->steps, &(king->amount_steps));
}

void set_all_valid_steps_per_piece(Gameboard *gameboard, Piece *piece) {
	for(int i = 0; i < piece->amount_steps; i++){
		destroy_step(piece->steps[i]);
		piece->steps[i] = NULL;
	}
	int amount_steps = 0;
	for(int i = 0; i < piece->amount_vectors; i++){
		bool check_is_threatened = (gameboard->difficulty <= 2 ? true : false); // if difiiculty <=2 we want to check if due the step, the piece is threatened
		add_steps_per_vector(gameboard, piece, piece->vectors[i], &amount_steps, piece->steps, check_is_threatened);
	}
	piece->amount_steps = amount_steps;
}

void add_steps_per_vector(Gameboard *gameboard, Piece *piece, Vector *v, int *amount_steps, Step **steps_list, bool check_is_threatened){
	int delta_row = v->delta_row;
	int delta_col = v->delta_col;
	int amount_going = v->vector_size;
	bool can_eat = v->can_eat;
	bool can_go_to_empty_spot = v->can_go_to_empty_spot;
	int row = piece->row;
	int col = piece->col;
	Piece_state piece_state = (piece->has_moved ? Was_moved: Was_not_moved);
	while(amount_going > 0){
		amount_going --;
		row = row + delta_row;
		col = col + delta_col;
		if(row < 0 || row > (BOARD_SIZE - 1) || col < 0 || col > (BOARD_SIZE - 1)) //out of board
			break;
		if(gameboard->board[row][col]->type == Empty && can_go_to_empty_spot){ // can go, empty
			if(!is_step_causes_check(gameboard, piece, row, col, gameboard->empty)) {
				if((row == (BOARD_SIZE - 1) || row == 0) && piece->type == Pawn) { // promotion
					piece_state = Was_promoted;
				}
				Step *s = create_step(piece->row, piece->col, row, col, gameboard->empty, piece_state, true);
				s->is_threatened = (check_is_threatened? is_step_threatened(gameboard, piece, s) : true);
				steps_list[*amount_steps] = s;
				(*amount_steps)++;
			}
		}
		else if(gameboard->board[row][col]->type != Empty && gameboard->board[row][col]->colur != piece->colur && can_eat){ //eating opponent's piece
			if(!is_step_causes_check(gameboard, piece, row, col, gameboard->board[row][col])) {
				if((row == (BOARD_SIZE - 1) || row == 0) && piece->type == Pawn) { // promotion
					piece_state = Was_promoted;
				}
				Step *s = create_step(piece->row, piece->col, row, col, gameboard->board[row][col], piece_state, true);
				s->is_threatened = (check_is_threatened? is_step_threatened(gameboard, piece, s) : true);
				steps_list[*amount_steps] = s;
				(*amount_steps)++;
			}
			break;
		}
		else{ // seeing your color piece
			break;
		}
	}
}

bool is_step_causes_check(Gameboard* gameboard, Piece* piece, int drow, int dcol, Piece *prevPiece){
	bool answer = false;
	gameboard->board[drow][dcol] = piece;
	gameboard->board[piece->row][piece->col] = gameboard->empty;
	prevPiece->alive = false;
	gameboard->turn = SWITCHED(gameboard->turn);
	if(is_check_curr_player(gameboard)){
		answer = true;
	}
	gameboard->turn = SWITCHED(gameboard->turn);
	prevPiece->alive = true;
	gameboard->board[drow][dcol] = prevPiece;
	gameboard->board[piece->row][piece->col] = piece;
	return answer;
}

bool is_step_threatened(Gameboard* gameboard, Piece* piece, Step* step){
	bool answer = false;
	gameboard->board[step->drow][step->dcol] = piece;
	gameboard->board[piece->row][piece->col] = gameboard->empty;
	step->prevPiece->alive = false;
	gameboard->turn = SWITCHED(gameboard->turn);
	if(is_threatening_piece(gameboard, piece)){
		answer = true;
	}
	gameboard->turn = SWITCHED(gameboard->turn);
	step->prevPiece->alive = true;
	gameboard->board[step->drow][step->dcol] = step->prevPiece;
	gameboard->board[piece->row][piece->col] = piece;
	return answer;
}

void set_castling_steps(Gameboard * gameboard, Piece *king, Step** steps_list, int *amount_steps){
	int turn = gameboard->turn;
	if(king->has_moved) return;
	if(is_under_check(gameboard)) return;
	if(!king->alive) return;
	Piece *rook;
	int delta_col;
	for(int i = 0; i <= 1; i++){ //go over the two rooks
		rook = gameboard->all_pieces[turn][12 + i]; //12 is the 1st place of rook in all_pieces
		if(is_castling_valid_per_rook(gameboard, king, rook)){
			delta_col = (king->col < rook->col) ? 2 : -2;
			Step *new_step = create_step(king->row, king->col, king->row, king->col + delta_col, gameboard->empty, Castling_Move, false);
			steps_list[*amount_steps] = new_step;
			(*amount_steps)++;
		}
	}

}

bool is_castling_valid_per_rook(Gameboard * gameboard, Piece* king, Piece* rook){
	if(rook->has_moved) return false;
	if(!rook->alive) return false;
	int row = king->row;
	int left_col = MIN(king->col, rook->col);
	int right_col = MAX(king->col, rook->col);
	for(int i = left_col + 1; i < right_col; i++){//there's no tool between rook and king
		if(gameboard->board[row][i]->type != Empty){
			return false;
		}
	}
	int delta_col = (king->col < rook->col) ? 1 : -1;
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


CHESS_BOARD_MESSAGE undo_step(Gameboard *gameboard, bool is_minimax) {
	if (gameboard == NULL) {
		return CHESS_BOARD_INVALID_ARGUMENT;
	}
	if (ArrayListSize(gameboard->history) == 0) {
		return CHESS_BOARD_NO_HISTORY;
	}
	Step *step = ArrayListGetFirst(gameboard->history);
	Piece *source_p = gameboard->board[step->drow][step->dcol];

	if (source_p->type == King && abs(step->dcol - step->scol) > 1) { // castling
		undo_step_castling(gameboard, step);
	}
	else { //not castling
		Piece *dest_p = step->prevPiece;
		gameboard->board[step->drow][step->dcol] = dest_p;
		gameboard->board[step->srow][step->scol] = source_p;
		dest_p->alive = true;
		if (step->src_previous_state == Was_not_moved) {
			source_p->has_moved = false;
		}
		else if (step->src_previous_state == Was_promoted) { // this step was a promotion step
			change_piece_type(source_p, Pawn);
		}
		source_p->row = step->srow;
		source_p->col = step->scol;
		dest_p->row = step->drow;
		dest_p->col = step->dcol;
		if (source_p->type == Pawn && !source_p->has_moved) { //if pawn in its initial spot - change it to be able to move 2 steps forward
			source_p->vectors[0]->vector_size = 2;
		}
	}
	gameboard->turn = SWITCHED(gameboard->turn);
	ArrayListRemoveFirst(gameboard->history);
	if (!is_minimax) {
		set_all_valid_steps(gameboard);
	}
	return CHESS_BOARD_SUCCESS;
}


CHESS_BOARD_MESSAGE double_undo(Gameboard *gameboard) {
	if(gameboard == NULL) {
		return CHESS_BOARD_INVALID_ARGUMENT;
	}
	if (ArrayListSize(gameboard->history) < 2) {
		return CHESS_BOARD_NO_HISTORY;
	}
	undo_step(gameboard, false);
	undo_step(gameboard, false);
	return CHESS_BOARD_SUCCESS;
}


void undo_step_castling(Gameboard *gameboard, Step *step){
	int row = step->drow;
	int king_dcol = step->dcol;
	int rook_dcol = (king_dcol == 2 ? 3 : 5);
	int rook_scol = (king_dcol == 2 ? 0 : 7);
	Piece *king = gameboard->board[row][king_dcol];
	Piece *rook = gameboard->board[row][rook_dcol];
	gameboard->board[row][king_dcol] = gameboard->empty;
	gameboard->board[row][rook_dcol] = gameboard->empty;

	gameboard->board[row][step->scol] = king;
	king->col = step->scol;
	gameboard->board[row][rook_scol] = rook;
	rook->col = rook_scol;
	rook->has_moved = false;
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
	if(is_check(gameboard, SWITCHED(gameboard->turn))){ // is my kind under check?
		return SWITCHED(gameboard->turn);
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
