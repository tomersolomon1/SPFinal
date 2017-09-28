/*
 * pieces.h
 *
 *  Created on: 9 баев„ 2017
 *      Author: User
 */

#ifndef GAMEBASICBUILDINGBLOCKS_H_
#define GAMEBASICBUILDINGBLOCKS_H_
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#define black 0
#define white 1
#define AMOUNT_VECTORS_PAWN 3
#define AMOUNT_VECTORS_KNIGHT 8
#define AMOUNT_VECTORS_BISHOP 4
#define AMOUNT_VECTORS_ROOK 4
#define AMOUNT_VECTORS_QUEEN 8
#define AMOUNT_VECTORS_KING 8

#define AMOUNT_STEPS_PAWN 4
#define AMOUNT_STEPS_KNIGHT 8
#define AMOUNT_STEPS_BISHOP 14
#define AMOUNT_STEPS_ROOK 14
#define AMOUNT_STEPS_QUEEN 27
#define AMOUNT_STEPS_KING 10

#define SIGN_PAWN 'M'
#define SIGN_KNIGHT 'N'
#define SIGN_BISHOP 'B'
#define SIGN_ROOK 'R'
#define SIGN_QUEEN 'Q'
#define SIGN_KING 'K'
#define SIGN_EMPTY '_'

#define AMOUNT_PIECES_PER_COLOR 16
void write_to_log_file(const char *event);

typedef enum {
	Pawn,
	Knight,
	Bishop,
	Rook,
	Queen,
	King,
	Empty
} Piece_type;

typedef enum{
	Was_not_moved,
	Was_moved,
	Was_promoted,
	Castling_Move
} Piece_state;

//A progressing vector on the board:
typedef struct Vector_t{
	int delta_row;
	int delta_col;
	int vector_size;
	bool can_eat; // can the piece move in that direction if there's an enemy piece there?
	bool can_go_to_empty_spot; //can the piece move in that direction if its empty?
} Vector;

//A chess step:
typedef struct step_t {
	int srow; // source row
	int scol; // source col
	int drow; // dest row
	int dcol; // dest col
	struct piece_t *prevPiece; //pointer to the piece that was killed in this step
	Piece_state step_info; //remember the piece state before the move: was moved before? was pawn and promoted?
	bool is_threatened;  //will the source piece be threatened by the other player if it goes with this step?
} Step;

//A chess piece:
typedef struct piece_t {
	Piece_type type;
	int colur; //piece color
	int row; //row coordinate
	int col; //column coordinate
	int indexat; //index of piece in the array of pieces
	char sign; //piece sign
	bool alive; //is piece alive?
	bool has_moved; //has piece moved?
	Step **steps; //all possible steps in a current game (size = 28)
	int amount_steps; //how many possible steps in a current game
	Vector **vectors; //all possible vector movements
	int amount_vectors; //how many different vectors
} Piece;



//-------------------Vector-------------------
/*create vector*/
Vector *create_vector(int delta_row, int delta_col, int vector_size, bool can_eat, bool can_go_to_empty_spot);

/*copy vector*/
Vector *copy_vector(Vector *old);

/*destroy vector*/
void destroy_vector(Vector *v);

//-------------------Step-------------------
/*create step*/
Step *create_step(int srow, int scol, int drow, int dcol, Piece *prevPiece, Piece_state src_prev_state, bool is_threatened);

/*copy step*/
Step *copy_step(Step *old);

/*destroy step*/
void destroy_step(Step *step);

/*print step (for debugging)*/
void print_step(Step *step);

//-------------------Piece-------------------
/*create piece*/
Piece *create_piece(Piece_type type, int colur, int row, int col, int indexat);

/* helping function for create piece
 * adds progressing vectors by piece type
 * The function assumes that the piece is in its initial position
 * therefore the Movement vector size of Pawn is set to 2*/
void set_vectors(Piece_type type, int colur, Vector **vectors);

/*copy_piece, steps will be null*/
Piece *copy_piece(Piece *old);

/*destroy piece*/
void destroy_piece(Piece *piece);

/*print all valid steps of a piece (for debugging)*/
void print_all_steps(Piece *piece);

/*change piece type, for promotion and undo promotion*/
void change_piece_type(Piece *piece, Piece_type new_type);

/*get sign of piece by piece type and color*/
char sign_of_piece(Piece_type type, int colur);

/*get type of piece by its sign*/
Piece_type get_piece_type_by_sign(char sign);

/*get amount of progressing vectors by piece type*/
int amount_vectors_of_piece_type(Piece_type type);

/*get max amount of steps by piece type*/
int amount_steps_of_piece_type(Piece_type type);

#endif /* GAMEBASICBUILDINGBLOCKS_H_ */
