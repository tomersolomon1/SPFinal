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
	printf("line 1:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line2[] = "quit";
	int offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Quit;
	get_non_arg_command(comm, line2, offset, "quit", Quit, Both);
	printf("line 2:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line2, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);
}


void int_commands() {
	char line[] = "user_color 1";
	Command *comm = parser(line);
	printf("line 1:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line2[] = "user_color 1";
	int offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line2, offset, "user_color", 0, 1, Set_UserColor, Both);
	printf("line 2:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line2, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line3[] = "user_color 2";
	offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line3, offset, "user_color", 0, 1, Set_UserColor, Both);
	printf("line 3:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line3, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line4[] = "difficulty 5";
	offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line4, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 4:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line4, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line5[] = "difficulty 11";
	offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line5, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 5:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line5, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line6[] = "difficulty A";
	offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line6, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 6:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line6, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line7[] = "difficulty 2A";
	offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line7, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 7:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line7, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line8[] = "difficulty                     ";
	offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line8, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 8:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line8, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line9[] = "difficulty";
	offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line9, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 9:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line9, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line10[] = "difficulty ,";
	offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line10, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 10:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line10, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line11[] = "difficul,";
	offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line11, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 11:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line11, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line12[] = " difficulty 4";
	offset = get_non_whitespace_offset(line);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line12, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 12:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line12, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);
}

void check_parser() {
	printf("check_parser\n");
	//no_arg_commands();
	int_commands();
	fflush(stdout);
}
