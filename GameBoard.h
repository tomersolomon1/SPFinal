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
#include "ArrayList.h"
#include "GameBasicBuildingBlocks.h"

#define HISTORY_SIZE 3
#define BOARD_SIZE 8
#define AMOUNT_PIECES_PER_COLOR 16

#define MIN(A,B) (((A) < (B)) ? (A) : (B))
#define MAX(A,B) (((A) > (B)) ? (A) : (B))
#define SWITCHED(x) abs(1 - (x))

typedef struct gameboard_t  {
	/*all_pieces: a list of all game pieces
	*all_pieces[black=0] for black pieces
	*all_pieces[white=1] for white pieces*/
	Piece *all_pieces[2][AMOUNT_PIECES_PER_COLOR];
	Piece *board[BOARD_SIZE][BOARD_SIZE]; //the board itself, with pointers to the pieces by their place
	int turn; //current turn
	ArrayList *history; //array list for saving history
	Piece *empty; //the game's empty piece
	int game_mode; //=1 for 1-player, =2 for 2-player
	int difficulty; //=1-4 by the difficulty level
	int user_color; //the user color
} Gameboard;

typedef enum chess_board_message_t {
	CHESS_BOARD_INVALID_MOVE_NO_PIECE,
	CHESS_BOARD_INVALID_MOVE_RULES_VIOLATION,
	CHESS_BOARD_INVALID_ARGUMENT,
	CHESS_BOARD_NO_HISTORY,
	CHESS_BOARD_SUCCESS,
	CHESS_BOARD_PROMOTION
} CHESS_BOARD_MESSAGE;

//-------------------------------Game Board General functions-------------------------------
/*create board*/
Gameboard *create_board(int game_mode, int difficulty, int user_color);

/*helping function for create_board (adding piece to the board)*/
void add_piece(Gameboard* gameboard, Piece_type type, int colur, int row, int col, int indexat);

/*destroy board*/
void destroy_board(Gameboard *gameboard);

/* copy board */
Gameboard *copy_board(Gameboard* old);
Gameboard *copy_board_minimax(Gameboard* old);
/* reset board with same parameters like *gameboard */
void reset_board(Gameboard** gameboard);

//----------------------------------------Set Step----------------------------------------
/*make a step
 * gets source and destination coordinates as 0-7 numbers
 * returns CHESS_BOARD_SUCCESS if legal move
 * returns CHESS_BOARD_INVALID_MOVE_<error-type> if move is NOT legal
 * */
CHESS_BOARD_MESSAGE set_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol);
CHESS_BOARD_MESSAGE set_step_minimax(Gameboard *gameboard, int srow, int scol, int drow, int dcol);
//make promotion for piece in board[row][col]
void make_promotion(Gameboard *gameboard, int row, int col, Piece_type new_type);

//helping function for set_step: setting step when castling
void set_castling_move(Gameboard *gameboard, int row, int scol, int dcol);

/* checks if a step is valid
 * returns CHESS_BOARD_SUCCESS if legal move
 * returns CHESS_BOARD_INVALID_MOVE_<error-type> if move is NOT legal
 * gets source and destination coordinates as 0-7 numbers
 * */
CHESS_BOARD_MESSAGE is_valid_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol);

//-----------------------Is (the other player) threatening piece-----------------------

/* is (Piece *threatened) threatened by the other player?
 * when the "other player" is the one with the other color of (Piece *threatened)*/
bool is_threatening_piece(Gameboard* gameboard, Piece *threatened);

/*helping function for is_threatening_piece: check per vector*/
bool is_threatening_piece_per_vector(Gameboard* gameboard, Piece *threatened, Piece* attacking, Vector* v_attacking);

/* is current player threatening the opponent's king?*/
bool is_check_curr_player(Gameboard *gameboard);

/*is current player under check?*/
bool is_under_check(Gameboard * gameboard);

/*is the player with color "colur" threatening the other player's king?*/
bool is_check(Gameboard *gameboard, int colur);

//-----------------------MINIMAX-----------------------

/* get all valid steps of piece in gameboard,
 * and update amount_steps to be the amount of steps of the piece
 * step ** is malloced inside the function
 * */
Step **get_all_valid_steps_of_piece_minimax(Gameboard *gameboard, Piece *piece, int *amount_steps);
void add_steps_per_vector_minimax(Gameboard *gameboard, Piece *piece, Vector *v, Step** all_steps, int *amount_steps);

/*free all_valid_steps*/
void free_all_valid_steps_minimax(Step** all_steps, Piece_type type);

/*assuming that the step is valid!!*/
CHESS_BOARD_MESSAGE set_step_minimax(Gameboard *gameboard, int srow, int scol, int drow, int dcol);

Gameboard *copy_board_minimax(Gameboard* old);

CHESS_BOARD_MESSAGE undo_step_minimax(Gameboard *gameboard);

//---------------------------------Set all Valid Steps---------------------------------

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

//set all castling setps:
void set_castling_steps(Gameboard * gameboard);

//helping function for set_castling_steps
bool is_castling_valid_per_rock(Gameboard * gameboard, Piece* king, Piece* rock);

//-------------------------------------Undo-------------------------------------

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

//----------------------------------Is Game Over----------------------------------

/* return the winner's color
 * if tie return 2
 * if not game over return -1*/
int is_game_over(Gameboard *gameboard);

//----------------------------------Print Board----------------------------------

/*print board for in console format*/
void print_board(Gameboard *gameboard);

/*for debugging - print board and details*/
void print_details_game(Gameboard *gameboard);

#endif /* GAMEBOARD_H_ */
