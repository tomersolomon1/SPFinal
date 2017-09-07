/*
 * Pieces.c
 *
 *  Created on: Aug 10, 2017
 *      Author: sapir
 */
#include "GameBasicBuildingBlocks.h"

//LOG
//extern FILE *f_log;
void write_to_log_file(const char *event){
//	FILE *f_log = fopen("log.txt","a");
//	fprintf(f_log, event);
//	fclose(f_log);
	printf(event); fflush(stdout);
}

//-------------------Vectors-------------------


Vector *create_vector(int delta_row, int delta_col, int vector_size, bool can_eat, bool can_go_to_empty_spot){
	Vector *v = (Vector*) malloc(sizeof(Vector));
	//write_to_log_file("malloc vector\n"); fflush(stdout);
	assert(v != NULL);
	v->delta_row = delta_row;
	v->delta_col = delta_col;
	v->vector_size = vector_size;
	v->can_eat = can_eat;
	v->can_go_to_empty_spot = can_go_to_empty_spot;
	return v;
}

Vector *copy_vector(Vector *old){
	Vector *v = (Vector *) malloc(sizeof(Vector));
	//write_to_log_file("malloc vector\n"); fflush(stdout);
	assert(v != NULL);
	v->delta_col = old->delta_col;
	v->delta_row = old->delta_row;
	v->vector_size = old->vector_size;
	v->can_eat = old->can_eat;
	v->can_go_to_empty_spot = old->can_go_to_empty_spot;
	return v;
}

void destroy_vector(Vector *v){
	if(v != NULL){
		free(v);
		//write_to_log_file("free vector\n"); fflush(stdout);
	}
}

//-------------------Steps-------------------

Step *create_step(int srow, int scol, int drow, int dcol, Piece *prevPiece, Piece_state src_prev_state, bool is_threatened){
	Step *newStep = (Step*) malloc(sizeof(Step));
	//write_to_log_file("malloc step\n"); fflush(stdout);
	assert(newStep != NULL);
	newStep->srow = srow;
	newStep->dcol = dcol;
	newStep->drow = drow;
	newStep->scol = scol;
	newStep->prevPiece = prevPiece;
	newStep->src_previous_state = src_prev_state;
	newStep->is_threatened = is_threatened;
	return newStep;
}

Step *copy_step(Step *old){
	Step *new = (Step*) malloc(sizeof(Step));
	//write_to_log_file("malloc step\n"); fflush(stdout);
	assert(new != NULL);
	new->srow = old->srow;
	new->scol = old->scol;
	new->drow = old->drow;
	new->dcol = old->dcol;
	new->prevPiece = NULL;
	new->src_previous_state = old->src_previous_state;
	new->is_threatened = old->is_threatened;
	return new;
}

void destroy_step(Step *step){
	if(step != NULL){
		free(step);
	}
}

void print_step(Step *step){
	printf("[%d%c]->[%d%c]", step->srow+1, 'A'+step->scol, step->drow+1, 'A' +step->dcol);
	fflush(stdout);
	printf(" eat:%c", step->prevPiece->sign);
	fflush(stdout);
	printf(" threatened:%c | ", (step->is_threatened ? 'Y' : 'N'));
}

//-------------------Pieces-------------------

Piece *create_piece(Piece_type type, int colur, int row, int col, int indexat) {
	Piece* newPiece = (Piece*) malloc(sizeof(Piece));
	//write_to_log_file("malloc piece\n"); fflush(stdout);
	assert(newPiece != NULL);
	newPiece->alive = true;
	newPiece->col = col;
	newPiece->row = row;
	newPiece->colur = colur;
	newPiece->has_moved = false;
	newPiece->type = type;
	newPiece->sign =  sign_of_piece(type, colur);
	newPiece->indexat = indexat;
	newPiece->amount_steps = 0;

	int amount_vectors = amount_vectors_of_piece_type(type);
	int max_amount_steps = amount_steps_of_piece_type(type);

	Vector **vectors = (Vector**)malloc(sizeof(Vector*) * amount_vectors);
	//write_to_log_file("malloc piece_vectors\n"); fflush(stdout);
	assert(vectors != NULL);
	set_vectors(type, colur, vectors);
	newPiece->amount_vectors = amount_vectors;
	newPiece->vectors = vectors;

	Step **steps = (Step**) malloc(sizeof(Step*) * max_amount_steps);
	//write_to_log_file("malloc piece_steps\n"); fflush(stdout);
	assert(steps != NULL);
	for(int i = 0; i < max_amount_steps; i++){
		steps[i] = NULL;
	}
	newPiece->steps = steps;
	return newPiece;
}

Piece *copy_piece(Piece *old){
	if(old == NULL){
		return NULL;
	}
	Piece* newPiece = (Piece*) malloc(sizeof(Piece));
	//write_to_log_file("malloc piece\n"); fflush(stdout);
	assert(newPiece != NULL);
	newPiece->alive = old->alive;
	newPiece->col = old->col;
	newPiece->colur = old->colur;
	newPiece->has_moved = old->has_moved;
	newPiece->row = old->row;
	newPiece->type = old->type;
	newPiece->sign = old->sign;
	newPiece->indexat = old->indexat;
	newPiece->amount_steps = 0;
	newPiece->amount_vectors = old->amount_vectors;

	Vector **vectors = (Vector**) malloc(sizeof(Vector*) * newPiece->amount_vectors);
	//write_to_log_file("malloc piece_vectors\n"); fflush(stdout);
	assert(vectors != NULL);
	for(int i = 0; i < newPiece->amount_vectors; i++){
		vectors[i] = copy_vector(old->vectors[i]);
	}
	newPiece->vectors = vectors;
	int max_amount_steps = amount_steps_of_piece_type(newPiece->type);

	Step **steps = (Step**) malloc(sizeof(Step*) * max_amount_steps);
	//write_to_log_file("malloc piece_steps\n"); fflush(stdout);
	assert(steps != NULL);
	for(int i = 0; i < max_amount_steps; i++){
		steps[i] = NULL;
	}
	newPiece->steps = steps;
	return newPiece;
}

void destroy_piece(Piece *piece) {
	int max_amount_steps = amount_steps_of_piece_type(piece->type);
	if(piece != NULL){
		for(int i = 0; i < max_amount_steps; i++){
			destroy_step(piece->steps[i]);
		}
		for(int i = 0; i < piece->amount_vectors; i++){
			destroy_vector(piece->vectors[i]);
		}
		free(piece->vectors);
		//write_to_log_file("free piece_vectors\n"); fflush(stdout);
		free(piece->steps);
		//write_to_log_file("free piece_steps\n"); fflush(stdout);
		free(piece);
		//write_to_log_file("free piece\n"); fflush(stdout);
	}
}

void change_piece_type(Piece *piece, Piece_type new_type){
	piece->type = new_type;
	piece->sign = sign_of_piece(new_type, piece->colur);
	int amount_vectors = amount_vectors_of_piece_type(new_type);
	int max_amount_steps = amount_steps_of_piece_type(new_type);

	//free steps and vectors:
	for(int i = 0; i < max_amount_steps; i++){
		destroy_step(piece->steps[i]);
	}
	for(int i = 0; i < piece->amount_vectors; i++){
		destroy_vector(piece->vectors[i]);
	}
	free(piece->vectors);
	free(piece->steps);

	//set new steps and vectors:
	Vector **vectors = (Vector**)malloc(sizeof(Vector*) * amount_vectors);
	assert(vectors != NULL);
	set_vectors(new_type, piece->colur, vectors);
	piece->amount_vectors = amount_vectors;
	piece->vectors = vectors;
	Step **steps = (Step**) malloc(sizeof(Step*) * max_amount_steps);
	assert(steps != NULL);
	for(int i = 0; i < max_amount_steps; i++){
		steps[i] = NULL;
	}
	piece->steps = steps;

	if(new_type == Pawn){
		vectors[0]->vector_size = 1;
	}
}

void set_vectors(Piece_type type, int colur, Vector **vectors){
	if(type == Pawn && colur == white){
		vectors[0] = create_vector(1, 0, 2, false, true); //can go straight
		vectors[1] = create_vector(1, 1, 1, true, false); //can eat diag
		vectors[2] = create_vector(1, -1, 1, true, false); //can eat diag
	}
	else if(type == Pawn && colur == black){
		vectors[0] = create_vector(-1, 0, 2, false, true); //can go straight
		vectors[1] = create_vector(-1, 1, 1, true, false); //can eat diag
		vectors[2] = create_vector(-1, -1, 1, true, false); //can eat diag
	}
	else if(type == Knight){
		vectors[0] = create_vector(1, 2, 1, true, true);
		vectors[1] = create_vector(1, -2, 1, true, true);
		vectors[2] = create_vector(-1, 2, 1, true, true);
		vectors[3] = create_vector(-1, -2, 1, true, true);
		vectors[4] = create_vector(2, 1, 1, true, true);
		vectors[5] = create_vector(2, -1, 1, true, true);
		vectors[6] = create_vector(-2, 1, 1, true, true);
		vectors[7] = create_vector(-2, -1, 1, true, true);
	}
	else if(type == Bishop){
		vectors[0] = create_vector(1, -1, 8, true, true);
		vectors[1] = create_vector(-1, 1, 8, true, true);
		vectors[2] = create_vector(1, 1, 8, true, true);
		vectors[3] = create_vector(-1, -1, 8, true, true);
	}
	else if(type == Rock){
		vectors[0] = create_vector(1, 0, 8, true, true);
		vectors[1] = create_vector(-1, 0, 8, true, true);
		vectors[2] = create_vector(0, 1, 8, true, true);
		vectors[3] = create_vector(0, -1, 8, true, true);
	}
	else if(type == Queen){
		vectors[0] = create_vector(1, 0, 8, true, true);
		vectors[1] = create_vector(-1, 0, 8, true, true);
		vectors[2] = create_vector(0, 1, 8, true, true);
		vectors[3] = create_vector(0, -1, 8, true, true);
		vectors[4] = create_vector(1, -1, 8, true, true);
		vectors[5] = create_vector(-1, 1, 8, true, true);
		vectors[6] = create_vector(1, 1, 8, true, true);
		vectors[7] = create_vector(-1, -1, 8, true, true);
	}
	else if(type == King){
		vectors[0] = create_vector(1, 0, 1, true, true);
		vectors[1] = create_vector(-1, 0, 1, true, true);
		vectors[2] = create_vector(0, 1, 1, true, true);
		vectors[3] = create_vector(0, -1, 1, true, true);
		vectors[4] = create_vector(1, -1, 1, true, true);
		vectors[5] = create_vector(-1, 1, 1, true, true);
		vectors[6] = create_vector(1, 1, 1, true, true);
		vectors[7] = create_vector(-1, -1, 1, true, true);
	}
}

int amount_vectors_of_piece_type(Piece_type type){
	switch (type){
		case Pawn:
			return AMOUNT_VECTORS_PAWN;
		case Knight:
			return AMOUNT_VECTORS_KNIGHT;
		case Bishop:
			return AMOUNT_VECTORS_BISHOP;
		case Rock:
			return AMOUNT_VECTORS_ROCK;
		case Queen:
			return AMOUNT_VECTORS_QUEEN;
		case King:
			return AMOUNT_VECTORS_KING;
		default:
			return 0;
	}
}

int amount_steps_of_piece_type(Piece_type type){
	switch (type){
		case Pawn:
			return AMOUNT_STEPS_PAWN;
		case Knight:
			return AMOUNT_STEPS_KNIGHT;
		case Bishop:
			return AMOUNT_STEPS_BISHOP;
		case Rock:
			return AMOUNT_STEPS_ROCK;
		case Queen:
			return AMOUNT_STEPS_QUEEN;
		case King:
			return AMOUNT_STEPS_KING;
		default:
			return 0;
	}
}

char sign_of_piece(Piece_type type, int colur){
	int add = (colur == black ? 0 : ('a' - 'A'));
	switch (type){
		case Pawn:
			return SIGN_PAWN + add;
		case Knight:
			return SIGN_KNIGHT + add;
		case Bishop:
			return SIGN_BISHOP + add;
		case Rock:
			return SIGN_ROCK + add;
		case Queen:
			return SIGN_QUEEN + add;
		case King:
			return SIGN_KING + add;
		case Empty:
			return SIGN_EMPTY;
	}
	return ' ';
}

void print_all_steps(Piece *piece){
	if(piece->alive){
		for(int i = 0; i < piece->amount_steps; i++){
			print_step(piece->steps[i]);
		}
	}
	else{
		printf("dead");
		fflush(stdout);
	}
}

