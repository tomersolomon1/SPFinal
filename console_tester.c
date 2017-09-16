/*
 * console_tester.c
 *
 *  Created on: 17 баев„ 2017
 *      Author: User
 */

#include <stdio.h>

#include "ConsoleMode.h"
#include "GameBoard.h"
#include "Parser.h"
#include "Files.h"

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

void run_console() {
	Gameboard *gameboard = create_board(1, 2, 1);
	manage_console(gameboard);
}

void check_get_moves_online() {
	Gameboard *gameboard = create_board(1, 2, 1);
	char *commands_str[] = {
			"load checking_games/check_check1.xml",
			"start",
			"get_moves <1,E>",
	};
	for (int i = 0; i < 3; i++) {
		char *line = (char *) malloc(sizeof(char)*(SP_MAX_LINE_LENGTH+1));
		assert(line != NULL);
		strcpy(line, commands_str[i]);
		Command *comm = parser(line); /* parsing the line */
		printf("line[%d]:||%s||END\n", i, line);
		switch (comm->comm_e) {
			case Load: load_file(&gameboard, comm);	break;
			case Start: console_begin_game(gameboard); break;
			case Get_Moves: get_moves(gameboard, comm);	break;
			default:
				printf("invalid - something went wrong\n");
				break;
		}
	}
}

void console_tester() {
	run_console();
	//check_get_moves_online();
}

void emulate_problem() {
	Gameboard *gameboard = create_board(1, 2, 1);
	Gameboard *copy = copy_board(gameboard);
	destroy_board(copy);
}
