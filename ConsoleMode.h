/*
 * ConsoleMode.h
 *
 *  Created on: 16 баев„ 2017
 *      Author: User
 */

#ifndef CONSOLEMODE_H_
#define CONSOLEMODE_H_
#include "GameBoard.h"

#define in_range(x) (((x) > -1) && ((x) < (8)))

extern char *colors[];

void manage_console(Gameboard *gameboard);
void begin_game(Gameboard *gameboard);
#endif /* CONSOLEMODE_H_ */
