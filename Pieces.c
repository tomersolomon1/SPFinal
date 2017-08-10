/*
 * Pieces.c
 *
 *  Created on: Aug 10, 2017
 *      Author: sapir
 */
#include <Pieces.h>

Piece *create_piece(Piece_type type, int colur, int row, int col, int indexat, char sign) {
	Piece* newPiece = (Piece*) malloc(sizeof(Piece));
	assert(newPiece != NULL);
	newPiece->alive = true;
	newPiece->col = col;
	newPiece->row = row;
	newPiece->colur = colur;
	newPiece->indexat = indexat;
	newPiece->has_moved = false;
	newPiece->type = type;
	newPiece->sign = sign;
	newPiece->amount_steps = 0;
	newPiece->steps = NULL;

	Vector_step* vectors;
	if(type == Pawn && colur == white){
		vectors = (Vector_step*) malloc(sizeof(Vector_step) * 1);
		assert();
		vectors[0] = create_vector(1, 0, 2);
		newPiece->amount_vectors = 1;
	}
	if(type == Pawn && colur == black){
		vectors[0] = create_vector(-1, 0, 2);
		newPiece->amount_vectors = 1;
	}
	if(type == Knight){
		Vector_step[0] = create_vector(1, 2, 1);
		Vector_step[1] = create_vector(1, -2, 1);
		Vector_step[2] = create_vector(-1, 2, 1);
		Vector_step[3] = create_vector(-1, -2, 1);
		Vector_step[4] = create_vector(2, 1, 1);
		Vector_step[5] = create_vector(2, -1, 1);
		Vector_step[6] = create_vector(-2, 1, 1);
		Vector_step[7] = create_vector(-2, -1, 1);
		newPiece->amount_vectors = 8;
	}
	if(type == Bishop){
		Vector_step[0] = create_vector(1, -1, 8);
		Vector_step[1] = create_vector(-1, 1, 8);
		Vector_step[2] = create_vector(1, 1, 8);
		Vector_step[3] = create_vector(-1, -1, 8);
		newPiece->amount_vectors = 4;
	}
	if(type == Rock){
		Vector_step[0] = create_vector(1, 0, 8);
		Vector_step[1] = create_vector(-1, 0, 8);
		Vector_step[2] = create_vector(0, 1, 8);
		Vector_step[3] = create_vector(0, -1, 8);
		newPiece->amount_vectors = 4;
	}
	if(type == Queen){
		Vector_step[0] = create_vector(1, 0, 8);
		Vector_step[1] = create_vector(-1, 0, 8);
		Vector_step[2] = create_vector(0, 1, 8);
		Vector_step[3] = create_vector(0, -1, 8);
		Vector_step[4] = create_vector(1, -1, 8);
		Vector_step[5] = create_vector(-1, 1, 8);
		Vector_step[6] = create_vector(1, 1, 8);
		Vector_step[7] = create_vector(-1, -1, 8);
		newPiece->amount_vectors = 8;
	}
	if(type == King){
		Vector_step[0] = create_vector(1, 0, 1);
		Vector_step[1] = create_vector(-1, 0, 1);
		Vector_step[2] = create_vector(0, 1, 1);
		Vector_step[3] = create_vector(0, -1, 1);
		Vector_step[4] = create_vector(1, -1, 1);
		Vector_step[5] = create_vector(-1, 1, 1);
		Vector_step[6] = create_vector(1, 1, 1);
		Vector_step[7] = create_vector(-1, -1, 1);
		newPiece->amount_vectors = 8;
	}
	return newPiece;
}

void destroy_piece(Piece piece) {
	if(piece != NULL){
		free(piece);
	}
}

Piece *copy_piece(Piece old){
	if(old == NULL){
		return NULL;
	}
	Piece* newPiece = (Piece*) malloc(sizeof(Piece));
	if(newPiece == NULL){
		return NULL;
	}
	newPiece->alive = old->alive;
	newPiece->col = old->col;
	newPiece->colur = old->colur;
	newPiece->indext = old->indext;
	newPiece->is_moved = old->is_moved;
	newPiece->row = old->row;
	newPiece->type = old->type;
	newPiece->sign = old->sign;
	return newPiece;
}

Vector_step create_vector(int delta_row, int delta_col, int vector_size){
	Vector_step v;
	v.delta_row = delta_row;
	v.delta_col = delta_col;
	v.vector_size = vector_size;
	return v;
}
