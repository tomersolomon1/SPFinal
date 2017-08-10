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

typedef struct step_t {
	int srow; //source row
	int scol; //source col
	int drow; //dest row
	int dcol; //dest col
	Piece *prevPiece; //i ate it
	bool is_srcPiece_was_moved;
} Step;

typedef struct piece_t {
	Piece_type type;
	int colur;
	int row; //row coordinate
	int col; //column coordinate
	bool alive;
	int indexat; //index in array of tools
	bool has_moved;
	char sign;

	Step* steps; //all possible steps in a current game
	int amount_steps; //how many possible steps in a current game

	Vector_step* vectors; //all possible vector movements
	int amount_vectors; //how many different vectors
} Piece;

typedef struct vector_step_t{
	int delta_row;
	int delta_col;
	int vector_size;
} Vector_step;
#endif /* PIECES_H_ */
