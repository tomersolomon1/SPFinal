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

typedef enum {
	Pawn, /* "Hayal" */
	Knight, /* "Parash */
	Bishop, /* Ratz */
	Rock, /* "Tzarih" */
	Queen,
	King,
	Empty
} Piece_type;
//Piece* Empty_piece = create_piece(Empty, -1, -1, -1, -1, '');


typedef struct piece_t {
	Piece_type type;
	int colur;
	int row; /* row coordinate */
	int col; /* column coordinate */
	bool alive;
	int indext; /*index in array of tools*/
	bool is_moved;
	char sign;
} Piece;

#endif /* PIECES_H_ */
