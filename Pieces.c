/*
 * Pieces.c
 *
 *  Created on: Aug 10, 2017
 *      Author: sapir
 */
#include "Pieces.h"
int type_amount_vectors[7] = {3,8,4,4,8,8,0};
int type_max_amount_steps[7] = {3,8,14,14,27,10,0};
//Vector:

Vector *create_vector(int delta_row, int delta_col, int vector_size, bool can_eat, bool can_go_to_empty_spot){
	Vector *v = (Vector*) malloc(sizeof(Vector));
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
	}
}

//Step:

Step *create_step(int srow, int scol, int drow, int dcol, Piece *prevPiece, bool is_srcPiece_was_moved){
	Step *newStep = (Step*) malloc(sizeof(Step));
	assert(newStep != NULL);
	newStep->srow = srow;
	newStep->dcol = dcol;
	newStep->drow = drow;
	newStep->scol = scol;
	newStep->prevPiece = prevPiece;
	newStep->is_srcPiece_was_moved = is_srcPiece_was_moved;
	return newStep;
}

//copy but without pointer to prev Piece
Step *copy_step(Step *old){
	Step *new = (Step*) malloc(sizeof(Step));
	assert(new != NULL);
	new->srow = old->srow;
	new->scol = old->scol;
	new->drow = old->drow;
	new->dcol = old->dcol;
	new->prevPiece = NULL;
	new->is_srcPiece_was_moved = old->is_srcPiece_was_moved;
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
	printf(" eat %c |", step->prevPiece->sign);
	fflush(stdout);
}

//Piece:

Piece *create_piece(Piece_type type, int colur, int row, int col, char sign, int indexat) {
	Piece* newPiece = (Piece*) malloc(sizeof(Piece));
	assert(newPiece != NULL);
	newPiece->alive = true;
	newPiece->col = col;
	newPiece->row = row;
	newPiece->colur = colur;
	newPiece->has_moved = false;
	newPiece->type = type;
	newPiece->sign = sign;
	newPiece->indexat = indexat;
	newPiece->amount_steps = 0;

	int amount_vectors = type_amount_vectors[Pawn + type];
	int max_amount_steps = type_max_amount_steps[Pawn + type];

	Vector **vectors = (Vector**)malloc(sizeof(Vector*) * amount_vectors);
	set_vectors(type, colur, vectors);
	newPiece->amount_vectors = amount_vectors;
	newPiece->vectors = vectors;

	Step **steps = (Step**) malloc(sizeof(Step*) * max_amount_steps);
	assert(steps != NULL);
	for(int i = 0; i < max_amount_steps; i++){
		steps[i] = NULL;
	}
	newPiece->steps = steps;
	return newPiece;
}

//copy_piece but steps remain null
Piece *copy_piece(Piece *old){
	if(old == NULL){
		return NULL;
	}
	Piece* newPiece = (Piece*) malloc(sizeof(Piece));
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
	assert(vectors != NULL);
	for(int i = 0; i < newPiece->amount_vectors; i++){
		vectors[i] = copy_vector(old->vectors[i]);
	}
	newPiece->vectors = vectors;
	int max_amount_steps = type_max_amount_steps[Pawn + newPiece->type];

	Step **steps = (Step**) malloc(sizeof(Step*) * max_amount_steps);
	assert(steps != NULL);
	for(int i = 0; i < max_amount_steps; i++){
		steps[i] = NULL;
	}
	newPiece->steps = steps;
	return newPiece;
}

void destroy_piece(Piece *piece) {
	int max_amount_steps = type_max_amount_steps[Pawn + piece->type];
	if(piece != NULL){
		for(int i = 0; i < max_amount_steps; i++){
			destroy_step(piece->steps[i]);
		}
		for(int i = 0; i < piece->amount_vectors; i++){
			destroy_vector(piece->vectors[i]);
		}
		free(piece->vectors);
		free(piece->steps);
		free(piece);
	}
}

void set_vectors(Piece_type type, int colur, Vector **vectors){
	if(type == Pawn && colur == white){
		vectors[0] = create_vector(1, 0, 2, false, true); //can go straight
		vectors[1] = create_vector(1, 1, 1, true, false); //can eat diag
		vectors[2] = create_vector(1, -1, 1, true, false); //can eat diag
	}
	if(type == Pawn && colur == black){
		vectors[0] = create_vector(-1, 0, 2, false, true); //can go straight
		vectors[1] = create_vector(-1, 1, 1, true, false); //can eat diag
		vectors[2] = create_vector(-1, -1, 1, true, false); //can eat diag
	}
	if(type == Knight){
		vectors[0] = create_vector(1, 2, 1, true, true);
		vectors[1] = create_vector(1, -2, 1, true, true);
		vectors[2] = create_vector(-1, 2, 1, true, true);
		vectors[3] = create_vector(-1, -2, 1, true, true);
		vectors[4] = create_vector(2, 1, 1, true, true);
		vectors[5] = create_vector(2, -1, 1, true, true);
		vectors[6] = create_vector(-2, 1, 1, true, true);
		vectors[7] = create_vector(-2, -1, 1, true, true);
	}
	if(type == Bishop){
		vectors[0] = create_vector(1, -1, 8, true, true);
		vectors[1] = create_vector(-1, 1, 8, true, true);
		vectors[2] = create_vector(1, 1, 8, true, true);
		vectors[3] = create_vector(-1, -1, 8, true, true);
	}
	if(type == Rock){
		vectors[0] = create_vector(1, 0, 8, true, true);
		vectors[1] = create_vector(-1, 0, 8, true, true);
		vectors[2] = create_vector(0, 1, 8, true, true);
		vectors[3] = create_vector(0, -1, 8, true, true);
	}
	if(type == Queen){
		vectors[0] = create_vector(1, 0, 8, true, true);
		vectors[1] = create_vector(-1, 0, 8, true, true);
		vectors[2] = create_vector(0, 1, 8, true, true);
		vectors[3] = create_vector(0, -1, 8, true, true);
		vectors[4] = create_vector(1, -1, 8, true, true);
		vectors[5] = create_vector(-1, 1, 8, true, true);
		vectors[6] = create_vector(1, 1, 8, true, true);
		vectors[7] = create_vector(-1, -1, 8, true, true);
	}
	if(type == King){
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
