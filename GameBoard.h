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



Gameboard *create_board();
void add_piece(Gameboard* gameboard, Piece_type type, int colur, int row, int col, int indexat, char sign);
void destroy_board(Gameboard *gameboard);
Gameboard *copy_board(Gameboard* old);
CHESS_BOARD_MESSAGE set_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol);
bool is_valid_step(Gameboard *gameboard, int srow, int scol, int drow, int dcol);
bool is_check_curr_player(Gameboard *gameboard);
//is the player with color colur threating the other player?
bool is_check(Gameboard *gameboard, int colur);
bool is_check_per_vector(Gameboard *gameboard, Piece *piece, Vector *v);
//---

void set_all_valid_steps(Gameboard *gameboard);
void set_all_valid_steps_per_piece(Gameboard *gameboard, Piece *piece);
void add_steps_per_vector(Gameboard *gameboard, Piece *piece, Vector *v, int *amount_steps);
bool is_step_causes_check(Gameboard* gameboard, Piece* piece, Step *step);
//---

Piece *get_piece_in_place(Gameboard *gameboard, int row, int col);
CHESS_BOARD_MESSAGE undo_step(Gameboard *gameboard);
// return the winner's color
// if tie return 2
// if not game over return -1
int is_game_over(Gameboard *gameboard);
void print_board(Gameboard *gameboard);
void print_details_game(Gameboard *gameboard);

#endif /* GAMEBOARD_H_ */
