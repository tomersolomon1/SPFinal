/*
 * ParserTester.c
 *
 *  Created on: 12 баев„ 2017
 *      Author: User
 */

#include "Parser.h"
#include <string.h>
#include <stdio.h>

char *commands_es[] = {"Set_GameMode", "Set_Difficulty", "Set_UserColor", "Load", "Restore_Default", "Print_Settings", "Start",\
			"Make_Move", "Save", "Undo_Move", "Reset",\
			"Quit", "Ivalid_command"}; /* string representation of the enums */

char *commands_s[] = {"game_mode", "difficulty", "user_color", "load", "default", "print_setting", "start", \
		"move", "save", "undo", "reset", "quit" };

// checks: bool valid_tail(Command *comm, const char *line, int offset)
void check_valid_tail(Command *comm) {
	comm->comm_e = Restore_Default;
	char line1[] = "aaa a";
	char *valid_line = valid_tail(comm, line1, 3) ? "true" : "false";

	printf("comm->comm_e = %s, valid_line = %s\n", commands_es[comm->comm_e], valid_line);

	comm->comm_e = Restore_Default;
	char line2[] = "a b";
	valid_line = valid_tail(comm, line2, 1) ? "true" : "false";
	printf("comm->comm_e = %s, valid_line = %s\n", commands_es[comm->comm_e], valid_line);

	comm->comm_e = Restore_Default;
	char line3[] = "a ";
	valid_line = valid_tail(comm, line3, 1) ? "true" : "false";
	printf("comm->comm_e = %s, valid_line = %s\n", commands_es[comm->comm_e], valid_line);
}

// checks: bool verify_command(Command *comm, const char *line, int offset, const char *comm_s, int comm_len, bool need_param)
void check_verify_command(Command *comm) {
	/* checking offset, should return true */
	comm->comm_e = Set_GameMode;
	char line1[] = "game_mode        ";
	char *valid_command = verify_command(comm, line1, 0, "game_mode", strlen("game_mode"), true) ? "true" : "false";
	printf("comm->comm_e = %s, valid-command = %s\n", commands_es[comm->comm_e], valid_command);

	/* checking offset, should return true */
	comm->comm_e = Set_GameMode;
	char line2[] = " game_mode        ";
    valid_command = verify_command(comm, line2, 1, "game_mode", strlen("game_mode"), true) ? "true" : "false";
    printf("comm->comm_e = %s, valid-command = %s\n", commands_es[comm->comm_e], valid_command);

    /* checking offset, should return false */
    comm->comm_e = Set_GameMode;
    char line3[] = " game_mode        ";
    valid_command = verify_command(comm, line3, 0, "game_mode", strlen("game_mode"), true) ? "true" : "false";
    printf("comm->comm_e = %s, valid-command = %s\n", commands_es[comm->comm_e], valid_command);

    /* checking the need-param arg, should return false */
    comm->comm_e = Set_GameMode;
    char line4[] = "game_mode";
    valid_command = verify_command(comm, line4, 0, "game_mode", strlen("game_mode"), true) ? "true" : "false";
    printf("comm->comm_e = %s, valid-command = %s\n", commands_es[comm->comm_e], valid_command);

    /* checking the need-param arg, should return true */
	comm->comm_e = Print_Settings;
	char line5[] = "print_setting";
	valid_command = verify_command(comm, line5, 0, "print_setting", strlen("print_setting"), false) ? "true" : "false";
	printf("comm->comm_e = %s, valid-command = %s\n", commands_es[comm->comm_e], valid_command);


	/* checking extra spaces, should return true */
	comm->comm_e = Print_Settings;
	char line6[] = "     print_setting                                              555";
	valid_command = verify_command(comm, line6, 5, "print_setting", strlen("print_setting"), false) ? "true" : "false";
	printf("comm->comm_e = %s, valid-command = %s\n", commands_es[comm->comm_e], valid_command);

	/* checking comparison, should return false */
	comm->comm_e = Print_Settings;
	char line7[] = "     print_seTTing                                              555";
	valid_command = verify_command(comm, line7, 5, "print_setting", strlen("print_setting"), false) ? "true" : "false";
	printf("comm->comm_e = %s, valid-command = %s\n", commands_es[comm->comm_e], valid_command);

	/* extra char in the end of the line-command, should return false */
	comm->comm_e = Print_Settings;
	char line8[] = "print_settingg                                              ";
	valid_command = verify_command(comm, line8, 0, "print_setting", strlen("print_setting"), false) ? "true" : "false";
	printf("comm->comm_e = %s, valid-command = %s\n", commands_es[comm->comm_e], valid_command);

}

void check_parser() {
	Command comm;
	comm.comm_e = Restore_Default; /* just for default value  */
	//check_valid_tail(&comm);
	check_verify_command(&comm);
}
