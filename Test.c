/*
 * Test.c
 *
 *  Created on: Aug 11, 2017
 *      Author: sapir
 */

#include "GameBoard.h"

#include "Parser.h"



void check_GameBoard() {
	Gameboard * game = create_board(1, -1, black);
	print_details_game(game);
	set_step(game, 1,1,2,1);
	print_details_game(game);
	set_step(game, 6,1,5,1);
	print_details_game(game);
	set_step(game, 0,1,2,2);
	print_details_game(game);
	set_step(game, 6,2,4,2);
	print_details_game(game);

	undo_step(game);
	print_details_game(game);
	set_step(game, 6,3,4,3);
	print_details_game(game);
	set_step(game, 2,2,4,3);
	print_details_game(game);
	set_step(game, 7,3,4,3);
	print_details_game(game);
	set_step(game, 1,0,2,0);
	print_details_game(game);
	set_step(game, 4,3,1,3);
	print_details_game(game);
	set_step(game, 0,5,1,3);
	print_details_game(game);
}

void check_valid_tail(Command *comm) {
	char *commands_s[] = {"Set_GameMode", "Set_Difficulty", "Set_UserColor", "Load",	"Restore_Default", "Print_Settings", "Start",\
			"Make_Move", "Save", "Undo_Move", "Reset",\
			"Quit", "Ivalid_command"};
	comm->comm_e = Restore_Default;
	char line1[] = "aaa                   a";
	char *valid_line = valid_tail(comm, line1, 3) ? "true" : "false";

	printf("comm->comm_e = %s, valid_line = %s\n", commands_s[comm->comm_e], valid_line);

	comm->comm_e = Restore_Default;
	char line2[] = "a                     b";
	valid_line = valid_tail(comm, line2, 1) ? "true" : "false";
	printf("comm->comm_e = %s, valid_line = %s\n", commands_s[comm->comm_e], valid_line);

	comm->comm_e = Restore_Default;
	char line3[] = "a                      ";
	valid_line = valid_tail(comm, line3, 2) ? "true" : "false";
	printf("comm->comm_e = %s, valid_line = %s\n", commands_s[comm->comm_e], valid_line);
}

void check_parser() {
	Command comm;
	comm.comm_e = Restore_Default; /* just for default value  */
	check_valid_tail(&comm);
}

int main(){
	check_GameBoard();
}
