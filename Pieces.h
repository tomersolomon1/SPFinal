/*
 * pieces.h
 *
 *  Created on: 9 баев„ 2017
 *      Author: User
 */

#ifndef PIECES_H_
#define PIECES_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#define black 0
#define white 1

typedef enum {
	Pawn, // Hayal
	Knight, // Parash
	Bishop, // Ratz
	Rock, // Tzarih
	Queen,
	King,
	Empty
} Piece_type;
//Piece* Empty_piece = create_piece(Empty, -1, -1, -1, -1, '');

typedef struct Vector_t{
	int delta_row;
	int delta_col;
	int vector_size;
} Vector;

typedef struct step_t {
	int srow; //source row
	int scol; //source col
	int drow; //dest row
	int dcol; //dest col
	int prevPiece_colur; //colur of prev piece
	int prevPiece_index; //index in "all_pieces array" of prev piece
	bool is_srcPiece_was_moved;
} Step;

typedef struct piece_t {
	Piece_type type;
	int colur;
	int row; //row coordinate
	int col; //column coordinate
	int indexat; //index of piece in the array of pieces
	bool alive;
	bool has_moved;
	char sign;
	Step **steps; //all possible steps in a current game (size = 28)
	int amount_steps; //how many possible steps in a current game
	Vector **vectors; //all possible vector movements
	int amount_vectors; //how many different vectors
} Piece;



#endif /* PIECES_H_ */
