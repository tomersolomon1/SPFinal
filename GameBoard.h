/*
 * Board.h
 *
 *  Created on: 9 баев„ 2017
 *      Author: User
 */

#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_
#include <string.h>
#include <stdbool.h>
#include "Pieces.h"
#include "ArrayList.h"
#define black 0
#define white 1

typedef struct gameboard_t  {
	Piece *board[8][8];
	int turn;
	Piece *all_pieces[2][16];
	ArrayList *history;
	Piece *empty; //the game's empty piece
} Gameboard;

typedef enum chess_board_message_t {
	CHESS_BOARD_INVALID_MOVE,
	CHESS_BOARD_INVALID_ARGUMENT,
	CHESS_BOARD_NO_HISTORY,
	CHESS_BOARD_SUCCESS,
} CHESS_BOARD_MESSAGE;

/*create board*/
Gameboard *create_board();

//helping function for create_board (adding piece to the board)
void add_piece(Gameboard* gameboard, Piece_type type, int colur, int row, int col, int indexat, char sign);

/*destroy board*/
void destroy_board(Gameboard *gameboard);

/* copy board */
Gameboard *copy_board(Gameboard* old);

/*make a step
 * gets source and destination coordinates as 0-7 numbers
 * returns CHESS_BOARD_SUCCESS if legal move
 * returns CHESS_BOARD_INVALID_MOVE if move is NOT legal
 * */
CHESS_BOARD_MESSAGE set_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol);

/* checks if a step is valid
 * gets source and destination coordinates as 0-7 numbers
 * */
bool is_valid_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol);

/* is current player threatening the opponent's king?*/
bool is_check_curr_player(Gameboard *gameboard);

/* is the player with color "colur" threatening the other player's king?*/
bool is_check(Gameboard *gameboard, int colur);

//helping function for is_check
bool is_check_per_vector(Gameboard *gameboard, Piece *piece, Vector *v);

/*set all valid steps, for all pieces of current turn*/
void set_all_valid_steps(Gameboard *gameboard);
/*helping function for set_all_valid_steps*/
void set_all_valid_steps_per_piece(Gameboard *gameboard, Piece *piece);
/*helping function for set_all_valid_steps*/
void add_steps_per_vector(Gameboard *gameboard, Piece *piece, Vector *v, int *amount_steps);
/*helping function for set_all_valid_steps*/
bool is_step_causes_check(Gameboard* gameboard, Piece* piece, Step *step);

/*NEED TO BE IN MACRO - get piece in place*/
Piece *get_piece_in_place(Gameboard *gameboard, int row, int col);

/* undo step
 * returns CHESS_BOARD_INVALID_ARGUMENT if game is null
 * returns CHESS_BOARD_NO_HISTORY if there's no history
 * returns CHESS_BOARD_SUCCESS if succeed*/
CHESS_BOARD_MESSAGE undo_step(Gameboard *gameboard);

/* return the winner's color
 * if tie return 2
 * if not game over return -1*/
int is_game_over(Gameboard *gameboard);

/*print board for in console format*/
void print_board(Gameboard *gameboard);

/*for debugging - print board and details*/
void print_details_game(Gameboard *gameboard);

#endif /* GAMEBOARD_H_ */
