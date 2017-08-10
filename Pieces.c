/*
 * Pieces.c
 *
 *  Created on: Aug 10, 2017
 *      Author: sapir
 */
#include <Pieces.h>

Piece *create_piece(Piece_type type, int colur, int row, int col, int indexat, char sign) {
	Piece* newPiece = (Piece*) malloc(sizeof(Piece));
	if(newPiece == NULL){
		return NULL;
	}
	newPiece->alive = true;
	newPiece->col = col;
	newPiece->row = row;
	newPiece->colur = colur;
	newPiece->indext = indexat;
	newPiece->is_moved = false;
	newPiece->type = type;
	newPiece->sign = sign;
	return Piece;
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

Step create_step(int srow, int scol, int drow, int dcol, Piece *prev){
	Step newStep;
	newStep.srow = srow;
	newStep.dcol = dcol;
	newStep.drow = drow;
	newStep.scol = scol;
	newStep.prevPiece = prev;
	return Step;
}
