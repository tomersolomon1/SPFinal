/*
 * pieces.h
 *
 *  Created on: 9 баев„ 2017
 *      Author: User
 */

#ifndef PIECES_H_
#define PIECES_H_
#include <stdbool.h>
typedef enum {
	Pawn, /* "Hayal" */
	Knight, /* "Parash */
	Bishop, /* Ratz */
	Rock, /* "Tzarih" */
	Queen,
	King,
} Piece_type;

typedef struct piece_t {
	Piece_type type;
	int colur;
	int row; /* row coordinate */
	int col; /* column coordinate */
	bool alive;
	int piece_indext;
	bool is_moved;
} Piece;

#endif /* PIECES_H_ */
