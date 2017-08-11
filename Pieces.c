/*
 * Pieces.c
 *
 *  Created on: Aug 10, 2017
 *      Author: sapir
 */
#include "Pieces.h"

Piece *create_piece(Piece_type type, int colur, int row, int col, char sign) {
	Piece* newPiece = (Piece*) malloc(sizeof(Piece));
	assert(newPiece != NULL);
	newPiece->alive = true;
	newPiece->col = col;
	newPiece->row = row;
	newPiece->colur = colur;
	newPiece->has_moved = false;
	newPiece->type = type;
	newPiece->sign = sign;
	newPiece->amount_steps = 0;
	int amount_vectors = 0;
	if(type == Pawn){
		amount_vectors = 1;
	}
	if(type == Knight || type == Queen || type == King){
		amount_vectors = 8;
	}
	if(type == Bishop || type == Rock){
		amount_vectors = 4;
	}
	Vector_step* vectors;
	vectors = (Vector_step*) malloc(sizeof(Vector_step) * amount_vectors);
	assert(vectors != NULL);
	set_vectors(type, colur, vectors);
	newPiece->amount_vectors = amount_vectors;
	newPiece->vectors = vectors;

	int max_amount_steps = 0;
	if(type == Pawn){
		max_amount_steps = 2;
	}
	if(type == Knight || type == King){
		max_amount_steps = 8;
	}
	if(type == Bishop || type == Rock){
		max_amount_steps = 14;
	}
	if(type == Queen){
		max_amount_steps = 28;
	}
	Step *steps = (Step*) malloc(sizeof(Step) * max_amount_steps);
	assert(steps != NULL);
	for(int i = 0; i < max_amount_steps; i++){
		Step[i] = NULL;
	}
	newPiece->steps = steps;
	return newPiece;
}

void set_vectors(Piece_type type, int colur, Vector_step * vectors){
	if(type == Pawn && colur == white){
		vectors[0] = create_vector(1, 0, 2);
	}
	if(type == Pawn && colur == black){
		vectors[0] = create_vector(-1, 0, 2);
	}
	if(type == Knight){
		vectors[0] = create_vector(1, 2, 1);
		vectors[1] = create_vector(1, -2, 1);
		vectors[2] = create_vector(-1, 2, 1);
		vectors[3] = create_vector(-1, -2, 1);
		vectors[4] = create_vector(2, 1, 1);
		vectors[5] = create_vector(2, -1, 1);
		vectors[6] = create_vector(-2, 1, 1);
		vectors[7] = create_vector(-2, -1, 1);
	}
	if(type == Bishop){
		vectors[0] = create_vector(1, -1, 8);
		vectors[1] = create_vector(-1, 1, 8);
		vectors[2] = create_vector(1, 1, 8);
		vectors[3] = create_vector(-1, -1, 8);
	}
	if(type == Rock){
		vectors[0] = create_vector(1, 0, 8);
		vectors[1] = create_vector(-1, 0, 8);
		vectors[2] = create_vector(0, 1, 8);
		vectors[3] = create_vector(0, -1, 8);
	}
	if(type == Queen){
		vectors[0] = create_vector(1, 0, 8);
		vectors[1] = create_vector(-1, 0, 8);
		vectors[2] = create_vector(0, 1, 8);
		vectors[3] = create_vector(0, -1, 8);
		vectors[4] = create_vector(1, -1, 8);
		vectors[5] = create_vector(-1, 1, 8);
		vectors[6] = create_vector(1, 1, 8);
		vectors[7] = create_vector(-1, -1, 8);
	}
	if(type == King){
		vectors[0] = create_vector(1, 0, 1);
		vectors[1] = create_vector(-1, 0, 1);
		vectors[2] = create_vector(0, 1, 1);
		vectors[3] = create_vector(0, -1, 1);
		vectors[4] = create_vector(1, -1, 1);
		vectors[5] = create_vector(-1, 1, 1);
		vectors[6] = create_vector(1, 1, 1);
		vectors[7] = create_vector(-1, -1, 1);
	}
}

void destroy_piece(Piece piece) {
	if(piece != NULL){
		free(piece->vectors);
		free(piece->steps);
		free(piece);
	}
}

Piece *copy_piece(Piece old){
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
	newPiece->amount_steps = old->amount_steps;
	newPiece->amount_vectors = old->amount_vectors;

	Vector_step *vectors = (Vector_step*) malloc(sizeof(Vector_step) * newPiece->amount_vectors);
	assert(vectors != NULL);
	for(int i = 0; i < newPiece->amount_vectors; i++){
		vectors[i] = old->vectors[i];
	}
	newPiece->vectors = vectors;

	Step *steps = (Step*) malloc(sizeof(Step) * newPiece->amount_steps);
	assert(steps != NULL);
	for(int i = 0; i < newPiece->amount_steps; i++){
		steps[i] = old->steps[i];
	}
	newPiece->steps = steps;
	return newPiece;
}

Vector_step create_vector(int delta_row, int delta_col, int vector_size){
	Vector_step v;
	v.delta_row = delta_row;
	v.delta_col = delta_col;
	v.vector_size = vector_size;
	return v;
}
