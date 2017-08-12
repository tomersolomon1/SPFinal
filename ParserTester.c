/*
 * ParserTester.c
 *
 *  Created on: 12 баев„ 2017
 *      Author: User
 */

#include "Parser.h"
#include <string.h>

void check_valid_tail(Command *comm) {
	char *commands_s[] = {"Set_GameMode", "Set_Difficulty", "Set_UserColor", "Load", "Restore_Default", "Print_Settings", "Start",\
			"Make_Move", "Save", "Undo_Move", "Reset",\
			"Quit", "Ivalid_command"};
	comm->comm_e = Restore_Default;
	char line1[] = "aaa a";
	char *valid_line = valid_tail(comm, line1, 3) ? "true" : "false";

	printf("comm->comm_e = %s, valid_line = %s\n", commands_s[comm->comm_e], valid_line);

	comm->comm_e = Restore_Default;
	char line2[] = "a b";
	valid_line = valid_tail(comm, line2, 1) ? "true" : "false";
	printf("comm->comm_e = %s, valid_line = %s\n", commands_s[comm->comm_e], valid_line);

	comm->comm_e = Restore_Default;
	char line3[] = "a ";
	valid_line = valid_tail(comm, line3, 1) ? "true" : "false";
	printf("comm->comm_e = %s, valid_line = %s\n", commands_s[comm->comm_e], valid_line);
}

void check_parser() {
	Command comm;
	comm.comm_e = Restore_Default; /* just for default value  */
	check_valid_tail(&comm);
}
