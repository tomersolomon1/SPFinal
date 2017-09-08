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
	int offset = get_non_whitespace_offset(line2);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line2, offset, "user_color", 0, 1, Set_UserColor, Both);
	printf("line 2:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line2, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line3[] = "user_color 2";
	offset = get_non_whitespace_offset(line3);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line3, offset, "user_color", 0, 1, Set_UserColor, Both);
	printf("line 3:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line3, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line4[] = "difficulty 5";
	offset = get_non_whitespace_offset(line4);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line4, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 4:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line4, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line5[] = "difficulty 11";
	offset = get_non_whitespace_offset(line5);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line5, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 5:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line5, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line6[] = "difficulty A";
	offset = get_non_whitespace_offset(line6);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line6, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 6:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line6, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line7[] = "difficulty 2A";
	offset = get_non_whitespace_offset(line7);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line7, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 7:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line7, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line8[] = "difficulty                     ";
	offset = get_non_whitespace_offset(line8);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line8, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 8:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line8, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line9[] = "difficulty";
	offset = get_non_whitespace_offset(line9);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line9, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 9:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line9, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line10[] = "difficulty ,";
	offset = get_non_whitespace_offset(line10);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line10, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 10:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line10, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line11[] = "difficul,";
	offset = get_non_whitespace_offset(line11);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line11, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 11:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line11, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line12[] = "	difficulty 		4";
	offset = get_non_whitespace_offset(line12);
	printf("offset: %d\n", offset);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line12, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 12:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line12, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line13[] = "	difficulty 		.4";
	offset = get_non_whitespace_offset(line13);
	printf("offset: %d\n", offset);
	comm = (Command *) malloc(sizeof(Command));
	comm->comm_e = Ivalid_command;
	get_int_arg(comm, line13, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
	printf("line 12:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line13, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);
}

void check_good_moves() {
	char line0[] = "move <1,A> to <1,B>";
	Command *comm = parser(line0);
	printf("line 0:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line0, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line1[] = "move 	<1,A> 	to 	<1,B>";
	comm = parser(line1);
	printf("line 1:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line1, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line2[] = "		move 	<1,A> 	to 	<1,B>";
	comm = parser(line2);
	printf("line 2:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line2, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line3[] = "		move <2,H> 	to 		<1,B>";
	comm = parser(line3);
	printf("line 3:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line3, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);
}

void check_bad_move() {
	char line1[] = "move <1,AA> to <1,B>";
	Command *comm = parser(line1);
	printf("line 1:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line1, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line2[] = "move <1,A>> to <1,B>";
	comm = parser(line2);
	printf("line 2:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line2, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);


	char line3[] = "move <1,J> to <1,B>";
	comm = parser(line3);
	printf("line 3:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line3, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line4[] = "move <11,A> to <1,B>";
	comm = parser(line4);
	printf("line 4:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line4, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line5[] = "move <A,1> to <1,B>";
	comm = parser(line5);
	printf("line 5:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line5, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);


	char line6[] = "move <1.1,1> to <1,B>";
	comm = parser(line6);
	printf("line 6:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line6, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line7[] = "move <1,,1> to <1,B>";
	comm = parser(line7);
	printf("line 7:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line7, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line8[] = "move <0,A> to <1,B>";
	comm = parser(line8);
	printf("line 8:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line8, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line9[] = "move <1,A> to <A,B>";
	comm = parser(line9);
	printf("line 9:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line9, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line10[] = "move <1,A> to <11,B>";
	comm = parser(line10);
	printf("line 10:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line10, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line11[] = "move <1,A> to <1,1>";
	comm = parser(line11);
	printf("line 11:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line11, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line12[] = "move <1,A> to <1,BB>";
	comm = parser(line12);
	printf("line 12:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line12, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line13[] = "move <1,A> to <1.1,B>";
	comm = parser(line13);
	printf("line 13:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line13, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line14[] = "move <1,A> to <1,,B>";
	comm = parser(line14);
	printf("line 14:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line14, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line15[] = "move <1,A> <1,B>";
	comm = parser(line15);
	printf("line 15:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line15, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line16[] = "move <1,A> to";
	comm = parser(line16);
	printf("line 16:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line16, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line17[] = "move <1,K> to         ";
	comm = parser(line17);
	printf("line 17:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line17, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line18[] = "move  to     <1,A>    ";
	comm = parser(line18);
	printf("line 18:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line18, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line19[] = "move  to        ";
	comm = parser(line19);
	printf("line 19:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line19, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line20[] = "move          ";
	comm = parser(line20);
	printf("line 20:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line20, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line21[] = "movee          ";
	comm = parser(line21);
	printf("line 21:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line21, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	char line22[] = "move<1,A> to <1,B>         ";
	comm = parser(line22);
	printf("line 22:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line22, commands_es[comm->comm_e], comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);
}


void check_get_command_with_file_name(Command *comm) {
	char line0[] = "save /dir/dir/file.txt          "; // checks recognizing command, without additional white-spaces and without offset
	comm->comm_e = Ivalid_command;
	int offset = get_non_whitespace_offset(line0);
	get_command_with_file_name(comm, line0, offset, "save", Save, GameMode);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s\nfile-name: %s\n\n",\
			line0, offset, commands_es[comm->comm_e], comm->file_name); // offset = 0, command-comm_e = Save

	char line1[] = "save /dir/dir/file.txt faffggdhdghh"; // checks extra parameter
	comm->comm_e = Ivalid_command;
	offset = get_non_whitespace_offset(line1);
	get_command_with_file_name(comm, line1, offset, "save", Save, GameMode);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s\nfile-name: %s\n\n",\
			line1, offset, commands_es[comm->comm_e], comm->file_name); // offset = 0, command-comm_e = Ivalid_command

	char line2[] = "save  "; // absence of parameter
	comm->comm_e = Ivalid_command;
	offset = get_non_whitespace_offset(line2);
	get_command_with_file_name(comm, line2, offset, "save", Save, GameMode);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s\nfile-name: %s\n\n",\
			line2, offset, commands_es[comm->comm_e], comm->file_name); // offset = 0, command-comm_e = Ivalid_command

	char line3[] = "load  /adfadsfadsfa.txt"; // wrong command
	comm->comm_e = Ivalid_command;
	offset = get_non_whitespace_offset(line3);
	get_command_with_file_name(comm, line3, offset, "load", Load, GameMode);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s\nfile-name: %s\n\n",\
			line3, offset, commands_es[comm->comm_e], comm->file_name); // offset = 0, command-comm_e = Ivalid_command

	char line4[] = "			load  	/adfadsfadsfa.txt"; // wrong command
	comm->comm_e = Ivalid_command;
	offset = get_non_whitespace_offset(line4);
	get_command_with_file_name(comm, line4, offset, "load", Load, GameMode);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s\nfile-name: %s\n\n",\
			line4, offset, commands_es[comm->comm_e], comm->file_name); // offset = 0, command-comm_e = Ivalid_command
}

void more_file_commands() {
	printf("more_file_commands\n");
	// exact parameters
	char line1[] = "save f1.txt";
	Command *comm = parser(line1);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\ncomm->file_name: %s\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line1, commands_es[comm->comm_e], comm->args_in_range,comm->file_name, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// too many parameters
	char line2[] = "save f1.txt f2.txt";
	comm = parser(line2);
	printf("line:%s|END|\ncomm->comm_e = %s,comm->arg_in_range: %d\ncomm->file_name: %s\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line2, commands_es[comm->comm_e], comm->args_in_range,comm->file_name, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// no parameters
	char line3[] = "load ";
	comm = parser(line3);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\ncomm->file_name: %s\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line3, commands_es[comm->comm_e], comm->args_in_range,comm->file_name, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// exact parameters, with offset
	char line4[] = " 	 	load f1.txt";
	comm = parser(line4);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->arg_in_range: %d\ncomm->file_name: %s\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line4, commands_es[comm->comm_e], comm->args_in_range,comm->file_name, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);
}

void check_save(Command *comm) {
	comm->comm_e = Save;
	char line9[] = "save";
	char *valid_command = verify_command(comm, line9, 0, "save", strlen("save")) ? "true" : "false";
	printf("comm->comm_e = %s, valid-command = %s\n", commands_es[comm->comm_e], valid_command);

	//free_command(comm);
	char line2[] = "	save "; // checks recognizing command, without offset
	comm = parser(line2);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->file_name: %s\n", line2, commands_es[comm->comm_e] ,comm->file_name);
	free_command(comm);


}

void check_file_commands() {
	Command comm;
	//check_get_command_with_file_name(&comm);
	check_save(&comm);
	//more_file_commands();
}

void check_parser() {
	printf("check_parser\n");
	//no_arg_commands();
	//int_commands();
	//check_bad_move();
	//check_good_moves();
	check_file_commands();
	fflush(stdout);
}
