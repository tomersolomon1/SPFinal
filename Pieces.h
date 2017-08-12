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
	bool can_eat;
	bool can_go_to_empty_spot;
} Vector;

typedef struct step_t {
	int srow; //source row
	int scol; //source col
	int drow; //dest row
	int dcol; //dest col
	struct piece_t *prevPiece;
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



//Vector:
Vector *create_vector(int delta_row, int delta_col, int vector_size, bool can_eat, bool can_go_to_empty_spot);
Vector *copy_vector(Vector *old);
void destroy_vector(Vector *v);

//Step
Step *create_step(int srow, int scol, int drow, int dcol, Piece *prevPiece, bool is_srcPiece_was_moved);
Step *copy_step(Step *old);
void destroy_step(Step *step);


//Piece:
Piece *create_piece(Piece_type type, int colur, int row, int col, char sign, int indexat);
//copy_piece but steps remain null
Piece *copy_piece(Piece *old);
void destroy_piece(Piece *piece);
void set_vectors(Piece_type type, int colur, Vector **vectors);


#endif /* PIECES_H_ */
