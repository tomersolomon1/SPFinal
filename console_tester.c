/*
 * console_tester.c
 *
 *  Created on: 17 баев„ 2017
 *      Author: User
 */

#include <stdio.h>

#include "ConsoleMode.h"
#include "GameBoard.h"

#define LEN 100
#define LEN_s "100"

 #define STR1(x)  #x
 #define STR(x)  STR1(x)

void test_strings() {
	char line[LEN+1];
	//sscanf(stdin, "%"LEN_s"s", line);
	scanf("%" LEN_s "s", line);
	printf("line1:||%s\n", line);
}

void console_tester() {
	Gameboard *gameboard = create_board(1, 2, 1);
	manage_console(gameboard);
	//destroy_board(gameboard);
}

void emulate_problem() {
	Gameboard *gameboard = create_board(1, 2, 1);
	Gameboard *copy = copy_board(gameboard);
	destroy_board(copy);
}
