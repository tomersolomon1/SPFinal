/*
 * PerserTester.c
 *
 *  Created on: 7 בספט׳ 2017
 *      Author: User
 */

#include "Parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *commands_es[] = {"Set_GameMode", "Set_Difficulty", "Set_UserColor", "Load", "Restore_Default", "Print_Settings", "Start",\
			"Make_Move", "Save", "Undo_Move", "Reset",\
			"Quit", "Ivalid_command"}; /* string representation of the enums */

void no_arg_commands() {
	char line[] = "print_setting";
	Command *comm = parser(line);
	printf("line 1:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line2[] = "quit";
	int offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Quit;
	get_non_arg_command(comm, line2, offset, "quit", Quit, Both);
	printf("line 2:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line2, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);
}

void check_parser() {
	printf("check_parser\n");
	no_arg_commands();
	fflush(stdout);
}
