/*
 * ConsoleMode.h
 *
 *  Created on: 16 баев„ 2017
 *      Author: User
 */

#ifndef CONSOLEMODE_H_
#define CONSOLEMODE_H_
#include "ConsoleDataDefinitions.h"
#include "GameBoard.h"
#include "Parser.h"
#define in_range(x) (((x) > -1) && ((x) < (8)))

typedef int (*Op)(Gameboard *gameboard, Command *comm);

extern char *colors[];

/* print "enter your move" message*/
void ask_move(Gameboard *gameboard, bool to_print);

/* print the computer's move */
void present_computer_move(Step *step, Piece *moving_piece, Piece_type promote_to);

/* performs a computer move only in mode 1,
 * and when we start the game it's not the user's turn
 * return false if the game is over (otherwise return true) */
bool console_begin_game(Gameboard *gameboard);

/* set game mode */
void set_game_mode(Gameboard *gameboard, Command *comm);

/* set game difficulty */
void set_difficulty(Gameboard *gameboard, Command *comm);

/* set user color */
void set_color(Gameboard *gameboard, Command *comm);

/* return true if succeeded in loading the file, otherwise return false */
bool load_file(Gameboard **gameboard_p, Command *comm);

/* set default parameters values to gameboard*/
void resore_default_values(Gameboard *gameboard);

/* print the gameboard settings */
void print_settings(Gameboard *gameboard);

/* handle promotion in console mode */
void handle_user_promotion(Gameboard *gameboard, int drow, int dcol);

/* helping function for make_move: make a single move
 * return 0 if it's illegal move, 1 if the game is over, and 2 if the game is not over */
int make_single_move(Gameboard *gameboard, int srow, int scol, int drow, int dcol,
		bool user_turn, Piece_type computer_promotion);

/* make a move
 * return -1 if illegal move, 0 if the game is over, and 1 otherwise */
int make_move(Gameboard *gameboard, Command *comm);

/* make castling move
 * return -1 if illegal move, 0 if the game is over, and 1 otherwise */
int castling_move(Gameboard *gameboard, Command *comm);

bool op_handler(Gameboard *gameboard, Command *comm, Op op);

/* used for sorting the steps by qsort function */
int steps_comperator(const void *p, const void *q);

/* present all the moves */
void present_all_moves(Gameboard *gameboard, Piece *piece);

/*  */
int get_moves(Gameboard *gameboard, Command *comm);

/* save game in console mode */
void save_game(Gameboard *gameboard, Command *comm);

/* undo move
 * return -1 if couldn't undo the move, otherwise return 0 */
int undo_move(Gameboard *gameboard);

/* gameboard is a pointer to a Gameboard object, with default values */
void manage_console(Gameboard *gameboard);

#endif /* CONSOLEMODE_H_ */
