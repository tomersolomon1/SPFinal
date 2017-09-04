/*
 * Board.h
 *
 *  Created on: 9 ����� 2017
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
#define history_size 3
#define AMOUNT_ROWS 8
#define AMOUNT_COLS 8

typedef struct gameboard_t  {
	Piece *board[8][8];
	int turn;
	Piece *all_pieces[2][16];
	ArrayList *history;
	Piece *empty; //the game's empty piece
	int game_mode; //=1 for 1-player, =2 for 2-player
	int difficulty;
	int user_color;
} Gameboard;

typedef enum chess_board_message_t {
	CHESS_BOARD_INVALID_MOVE_NO_PIECE,
	CHESS_BOARD_INVALID_MOVE_RULES_VIOLATION,
	CHESS_BOARD_INVALID_ARGUMENT,
	CHESS_BOARD_NO_HISTORY,
	CHESS_BOARD_SUCCESS,
	CHESS_BOARD_PROMOTION
} CHESS_BOARD_MESSAGE;

/*create board*/
Gameboard *create_board(int game_mode, int difficulty, int user_color);
//helping function for create_board (adding piece to the board)
void add_piece(Gameboard* gameboard, Piece_type type, int colur, int row, int col, int indexat);

/*destroy board*/
void destroy_board(Gameboard *gameboard);

/* copy board */
Gameboard *copy_board(Gameboard* old);

/* reset board with same parameters like *gameboard */
void reset_board(Gameboard** gameboard);

/*make a step
 * gets source and destination coordinates as 0-7 numbers
 * returns CHESS_BOARD_SUCCESS if legal move
 * returns CHESS_BOARD_INVALID_MOVE_<error-type> if move is NOT legal
 * */
CHESS_BOARD_MESSAGE set_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol);

//helping function for set_step: setting step when castling
void set_castling_move(Gameboard *gameboard, int row, int scol, int dcol);

//make promotion for piece in board[row][col]
void make_promotion(Gameboard *gameboard, int row, int col, Piece_type new_type);

/* checks if a step is valid
 * returns CHESS_BOARD_SUCCESS if legal move
 * returns CHESS_BOARD_INVALID_MOVE_<error-type> if move is NOT legal
 * gets source and destination coordinates as 0-7 numbers
 * */
CHESS_BOARD_MESSAGE is_valid_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol);

/* is (Piece *threatened) threatened by the other player?
 * when the "other player" is the one with the other color of (Piece *threatened)*/
bool is_threatening_piece(Gameboard* gameboard, Piece *threatened);
/*helping function for is_threatening_piece: check per vector*/
bool is_threatening_piece_per_vector(Gameboard* gameboard, Piece *threatened, Piece* attacking, Vector* v_attacking);
/*is the player with color "colur" threatening the other player's king?*/
bool is_check(Gameboard *gameboard, int colur);
/* is current player threatening the opponent's king?*/
bool is_check_curr_player(Gameboard *gameboard);
/*is current player under check?*/
bool is_under_check(Gameboard * gameboard);

/*set all valid steps, for all pieces of current turn*/
void set_all_valid_steps(Gameboard *gameboard);
/*helping function for set_all_valid_steps*/
void set_all_valid_steps_per_piece(Gameboard *gameboard, Piece *piece);
/*helping function for set_all_valid_steps*/
void add_steps_per_vector(Gameboard *gameboard, Piece *piece, Vector *v, int *amount_steps);
/*helping function for set_all_valid_steps*/
bool is_step_causes_check(Gameboard* gameboard, Piece* piece, Step *step);
/*helping function for set_all_valid_steps*/
bool is_step_threatened(Gameboard* gameboard, Piece* piece, Step* step);

//set castling setps:
void set_castling_steps(Gameboard * gameboard);
//helping function for set_castling_steps
bool is_castling_valid_per_rock(Gameboard * gameboard, Piece* king, Piece* rock);

/*NEED TO BE IN MACRO - get piece in place*/
Piece *get_piece_in_place(Gameboard *gameboard, int row, int col);

/* undo step
 * returns CHESS_BOARD_INVALID_ARGUMENT if game is null
 * returns CHESS_BOARD_NO_HISTORY if there's no history
 * returns CHESS_BOARD_SUCCESS if succeed */
CHESS_BOARD_MESSAGE undo_step(Gameboard *gameboard);

/* double_undo - performing double undo if possible
 * returns CHESS_BOARD_INVALID_ARGUMENT if game is null
 * returns CHESS_BOARD_NO_HISTORY if there isn't enough history
 * returns CHESS_BOARD_SUCCESS otherwise */
CHESS_BOARD_MESSAGE double_undo(Gameboard *gameboard);

//helping function for undo_step, doing undo to castling step
void undo_step_castling(Gameboard *gameboard, Step* step);
/* return the winner's color
 * if tie return 2
 * if not game over return -1*/
int is_game_over(Gameboard *gameboard);

/*print board for in console format*/
void print_board(Gameboard *gameboard);

/*for debugging - print board and details*/
void print_details_game(Gameboard *gameboard);

#endif /* GAMEBOARD_H_ */
