/*
 * ConsoleMode.h
 *
 *  Created on: 16 баев„ 2017
 *      Author: User
 */

#ifndef CONSOLEMODE_H_
#define CONSOLEMODE_H_
#include "GameBoard.h"
#include "DataDefinitions.h"
#include "Parser.h"
#define in_range(x) (((x) > -1) && ((x) < (8)))

extern char *colors[];
bool load_file(Gameboard **gameboard_p, Command *comm);
void set_difficulty(Gameboard *gameboard, Command *comm);
void reset_game(Gameboard **gameboard);
void manage_console(Gameboard *gameboard);
int begin_game(Gameboard *gameboard, Program_Mode mode);
bool console_begin_game(Gameboard *gameboard);
int make_single_move(Gameboard *gameboard, int srow, int scol, int drow, int dcol,
		bool user_turn, Piece_type computer_promotion);
void get_moves(Gameboard *gameboard, Command *comm);
#endif /* CONSOLEMODE_H_ */
