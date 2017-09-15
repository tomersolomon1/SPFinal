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
#define in_range(x) (((x) > -1) && ((x) < (8)))

extern char *colors[];

void manage_console(Gameboard *gameboard);
int begin_game(Gameboard *gameboard, Program_Mode mode);
int make_single_move(Gameboard *gameboard, int srow, int scol, int drow, int dcol,
		bool user_turn, Piece_type computer_promotion);
#endif /* CONSOLEMODE_H_ */
