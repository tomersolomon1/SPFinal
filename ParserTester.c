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

	/* doen't work for some crazy reason */
	comm->comm_e = Save;
	char line9[] = "save ";
	valid_command = verify_command(comm, line9, 0, "save", strlen("save"), false) ? "true" : "false";
	printf("comm->comm_e = %s, valid-command = %s\n", commands_es[comm->comm_e], valid_command);

}

// checks: int get_non_whitespace_offset(const char *str)
void check_get_non_whitespace_offset() {
	int offset1 = get_non_whitespace_offset("game_mode"); // offset = 0
	printf("offset = %d\n", offset1);

	int offset2 = get_non_whitespace_offset("     	game_mode"); // offset = 6
	printf("offset = %d\n", offset2);

	int offset3 = get_non_whitespace_offset(""); // offset = -1
	printf("offset = %d\n", offset3);

	int offset4 = get_non_whitespace_offset("           "); // offset = -1
	printf("offset = %d\n", offset4);

	int offset5 = get_non_whitespace_offset("save"); // offset = 0
	printf("offset = %d\n", offset5);
}

void check_save_anommality(Command *comm) {
	comm->comm_e = Save;
	char line9[] = "save ";
	char *valid_command = verify_command(comm, line9, 0, "save", strlen("save"), false) ? "true" : "false";
	printf("comm->comm_e = %s, valid-command = %s\n", commands_es[comm->comm_e], valid_command);

	char line2[] = "save "; // checks recognizing command, without offset
	comm->comm_e = Save;
	int offset = get_non_whitespace_offset(line2);
	get_non_arg_command(comm, line2, offset, "save");
	printf("line:%s END\noffset = %d, comm->comm_e = %s\n\n", line2, offset, commands_es[comm->comm_e]); // offset = 0, command-comm_e = Save
}

// checks: void get_non_arg_command(Command *comm, const char *line, int offset, const char *comm_s)
void check_get_non_arg_command(Command *comm) {
	char line0[] = "save                           "; // checks recognizing command, with additional white-spaces and without offset
	comm->comm_e = Save;
	int offset = get_non_whitespace_offset(line0);
	get_non_arg_command(comm, line0, offset, "save");
	printf("line:%s END\noffset = %d, comm->comm_e = %s\n\n", line0, offset, commands_es[comm->comm_e]); // offset = 0, command-comm_e = Save

	char line1[] = "          save                  "; // checks recognizing command, with offset
	comm->comm_e = Save;
	offset = get_non_whitespace_offset(line1);
	get_non_arg_command(comm, line1, offset, "save");
	printf("line:%s END\noffset = %d, comm->comm_e = %s\n\n", line1, offset, commands_es[comm->comm_e]); // offset = 10, command-comm_e = Save

	char line2[] = "save "; // checks recognizing command, without offset
	comm->comm_e = Save;
	offset = get_non_whitespace_offset(line2);
	get_non_arg_command(comm, line2, offset, "save");
	printf("line:%s END\noffset = %d, comm->comm_e = %s\n\n", line2, offset, commands_es[comm->comm_e]); // offset = 0, command-comm_e = Save

	char line3[] = "save                                5"; // checks redundant parameter, without offset
	comm->comm_e = Save;
	offset = get_non_whitespace_offset(line3);
	get_non_arg_command(comm, line3, offset, "save");
	printf("line:%s END\noffset = %d, comm->comm_e = %s\n\n", line3, offset, commands_es[comm->comm_e]); // offset = 0, command-comm_e = Ivalid_command

	char line4[] = "  save     6"; // checks redundant parameter, with offset
	comm->comm_e = Save;
	offset = get_non_whitespace_offset(line4);
	get_non_arg_command(comm, line4, offset, "save");
	printf("line:%s END\noffset = %d, comm->comm_e = %s\n\n", line4, offset, commands_es[comm->comm_e]); // offset = 5, command-comm_e = Ivalid_command

	char line5[] = "  save5"; // checks linked parameter
	comm->comm_e = Save;
	offset = get_non_whitespace_offset(line5);
	get_non_arg_command(comm, line5, offset, "save");
	printf("line:%s END\noffset = %d, comm->comm_e = %s\n\n", line5, offset, commands_es[comm->comm_e]); // offset = 6, command-comm_e = Ivalid_command

	char line6[] = "sAvE"; // checks spelling-checker
	comm->comm_e = Save;
	offset = get_non_whitespace_offset(line6);
	get_non_arg_command(comm, line6, offset, "save");
	printf("line:%s END\noffset = %d, comm->comm_e = %s\n\n", line6, offset, commands_es[comm->comm_e]); // offset = 0, command-comm_e = Ivalid_command
}

// void get_command_with_file_name(Command *comm, const char *line, int offset, const char *comm_s)
void check_get_command_with_file_name(Command *comm) {
	char line0[] = "save /dir/dir/file.txt          "; // checks recognizing command, without additional white-spaces and without offset
	comm->comm_e = Save;
	int offset = get_non_whitespace_offset(line0);
	get_command_with_file_name(comm, line0, offset, "save");
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s\nfile-name: %s\n\n",\
			line0, offset, commands_es[comm->comm_e], comm->file_name); // offset = 0, command-comm_e = Save

	char line1[] = "save /dir/dir/file.txt faffggdhdghh"; // checks extra parameter
	comm->comm_e = Save;
	offset = get_non_whitespace_offset(line1);
	get_command_with_file_name(comm, line1, offset, "save");
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s\nfile-name: %s\n\n",\
			line1, offset, commands_es[comm->comm_e], comm->file_name); // offset = 0, command-comm_e = Ivalid_command

	char line2[] = "save  "; // absence of parameter
	comm->comm_e = Save;
	offset = get_non_whitespace_offset(line2);
	get_command_with_file_name(comm, line2, offset, "save");
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s\nfile-name: %s\n\n",\
			line2, offset, commands_es[comm->comm_e], comm->file_name); // offset = 0, command-comm_e = Ivalid_command

	char line3[] = "Load  /adfadsfadsfa.txt"; // wrong command
	comm->comm_e = Load;
	offset = get_non_whitespace_offset(line3);
	get_command_with_file_name(comm, line3, offset, "load");
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s\nfile-name: %s\n\n",\
			line3, offset, commands_es[comm->comm_e], comm->file_name); // offset = 0, command-comm_e = Ivalid_command
}

// void get_int_arg(Command *comm, const char *line, int offset, const char *comm_s, int lower_bound, int upper_bound)
void check_get_int_arg(Command *comm) {
	char line0[] = "user_color         "; // checks recognizing command, without offset and parameter
	comm->comm_e = Set_UserColor;
	int offset = get_non_whitespace_offset(line0);
	get_int_arg(comm, line0, offset, "user_color", 0, 1);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d, comm->arg1: %d\n\n",\
			line0, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1);
	// offset = 0, command-comm_e = Set_UserColor, arg is invalid (no arg)


	char line1[] = "user_color    1     "; // checks recognizing command, with good parameter
	comm->comm_e = Set_UserColor;
	offset = get_non_whitespace_offset(line1);
	get_int_arg(comm, line1, offset, "user_color", 0, 1);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d, comm->arg1: %d\n\n",\
			line1, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1);
	// offset = 0, command-comm_e = Set_UserColor, arg is valid and in range


	char line2[] = "difficulty    10     "; // checks recognizing command, with bad parameter
	comm->comm_e = Set_Difficulty;
	offset = get_non_whitespace_offset(line2);
	get_int_arg(comm, line2, offset, "difficulty", 1, 5);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d, comm->arg1: %d\n\n",\
			line2, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1);
	// offset = 0, command-comm_e = Set_Difficulty, arg is valid, not in range


	char line3[] = "difficulty    10     10"; // checks recognizing command, too much parameters
	comm->comm_e = Set_Difficulty;
	offset = get_non_whitespace_offset(line3);
	get_int_arg(comm, line3, offset, "difficulty", 1, 5);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d, comm->arg1: %d\n\n",\
			line3, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1);
	// offset = 0, command-comm_e = invalid, arg is valid (????), not in range


	char line4[] = "difficulty    00000000000004"; // checks recognizing command, too much parameters
	comm->comm_e = Set_Difficulty;
	offset = get_non_whitespace_offset(line4);
	get_int_arg(comm, line4, offset, "difficulty", 1, 5);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d, comm->arg1: %d\n\n",\
			line4, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1);
	// offset = 0, command-comm_e = Set_Difficulty, arg is valid, not in range

}

// void getXY(Command *comm, const char *line, int *offset, int *row, int *col)
void check_getXY(Command *comm) {
	comm->comm_e = Make_Move;
	char line1[] = "<1,A>";
	int offset = get_non_whitespace_offset(line1);
	getXY(comm, line1, &offset, &comm->arg1, &comm->arg2);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d, comm->arg1: %d, comm->arg2: %d\n\n",\
			line1, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2);
	// everything is in good shape

	comm->comm_e = Make_Move;
	char line2[] = "    <1,A>";
	offset = get_non_whitespace_offset(line2);
	getXY(comm, line2, &offset, &comm->arg1, &comm->arg2);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d, comm->arg1: %d, comm->arg2: %d\n\n",\
			line2, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2);
	// everything is in good shape

	comm->comm_e = Make_Move;
	char line3[] = "    <00000001,0000000A>";
	offset = get_non_whitespace_offset(line3);
	getXY(comm, line3, &offset, &comm->arg1, &comm->arg2);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d, comm->arg1: %d, comm->arg2: %d\n\n",\
			line3, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2);
	// everything is in good shape


	comm->comm_e = Make_Move;
	char line4[] = "    <11,A>";
	offset = get_non_whitespace_offset(line4);
	getXY(comm, line4, &offset, &comm->arg1, &comm->arg2);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d, comm->arg1: %d, comm->arg2: %d\n\n",\
			line4, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2);
	// arguments not in range
}

// void get_move_arg(Command *comm, const char *line, int offset)
void check_get_move(Command *comm) {
	// everything is in place, all parameters are in range
	comm->comm_e = Make_Move;
	char line1[] = "move <1,A> to <2,B>";
	int offset = get_non_whitespace_offset(line1);
	get_move_arg(comm, line1, offset);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line1, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);

	// everything is in place, parameters are *not* in range
	comm->comm_e = Make_Move;
	char line2[] = "move <1,AA> to <2,B>";
	offset = get_non_whitespace_offset(line2);
	get_move_arg(comm, line2, offset);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line2, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);

	// too many commas in the first argument
	comm->comm_e = Make_Move;
	char line3[] = "move <1,,A> to <2,B>";
	offset = get_non_whitespace_offset(line3);
	get_move_arg(comm, line3, offset);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line3, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);

	// one extra '>' bracket
	comm->comm_e = Make_Move;
	char line4[] = "move <1,A>> to <2,B>";
	offset = get_non_whitespace_offset(line4);
	get_move_arg(comm, line4, offset);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line4, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);

	// one extra '<' bracket - should ask Moav how to treat it (invalid command, or take the ASCII value of '<')
	comm->comm_e = Make_Move;
	char line5[] = "move <1,A> to <<2,B>";
	offset = get_non_whitespace_offset(line5);
	get_move_arg(comm, line5, offset);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line5, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);

	// one extra '<' bracket - should ask Moav how to treat it (invalid command OR invalid input)
	comm->comm_e = Make_Move;
	char line6[] = "move <1,A> to <2,B>>";
	offset = get_non_whitespace_offset(line6);
	get_move_arg(comm, line6, offset);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line6, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);

	// one extra '<' bracket - should ask Moav how to treat it (invalid command OR invalid input)
	comm->comm_e = Make_Move;
	char line7[] = "move <1,A> to <2,B> adsfasdfdf";
	offset = get_non_whitespace_offset(line6);
	get_move_arg(comm, line7, offset);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line7, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);

}

void no_parameter_commands() {
	// no parameters, should work
	char line1[] = "start      ";
	Command *comm = parser(line1);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line1, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// checking offset
	char line2[] = "     default      ";
	comm = parser(line2);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line2, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// too many parameters
	char line3[] = "     print_setting    5555   ";
	comm = parser(line3);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line3, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);


	// reset command
	char line4[] = "     reset   ";
	comm = parser(line4);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line4, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// quit command
	char line5[] = "     quit   ";
	comm = parser(line5);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line5, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// undo command
	char line6[] = "     undo   ";
	comm = parser(line6);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line6, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);
}

void int_parameter_commands() {
	// all parameters, everything should work fine
	char line1[] = "game_mode 1";
	Command *comm = parser(line1);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line1, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// missing parameter
	char line2[] = "game_mode       ";
	comm = parser(line2);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line2, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// too many parameters
	char line3[] = "user_color 1 1       ";
	comm = parser(line3);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line3, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// parameter not in range
	char line4[] = "user_color 4       ";
	comm = parser(line4);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line4, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// check offset
	char line5[] = "    difficulty     4 ";
	comm = parser(line5);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line5, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);
}

void file_commands() {
	// exact parameters
	char line1[] = "save f1.txt";
	Command *comm = parser(line1);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\ncomm->file_name: %s\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line1, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range,comm->file_name, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// too many parameters
	char line2[] = "save f1.txt f2.txt";
	comm = parser(line2);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\ncomm->file_name: %s\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line2, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range,comm->file_name, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// no parameters
	char line3[] = "load ";
	comm = parser(line3);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\ncomm->file_name: %s\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line3, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range,comm->file_name, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);

	// exact parameters, with offset
	char line4[] = " 	 	load f1.txt";
	comm = parser(line4);
	printf("line:%s|END|\ncomm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d\ncomm->file_name: %s\n"
			"comm->arg1: %d, comm->arg2: %d, comm->arg3: %d, comm->arg4: %d\n\n",
			line4, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range,comm->file_name, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
	free_command(comm);
}

void check_parser_itself() {
	//int_parameter_commands();
	//no_parameter_commands();
	file_commands();
}

void check_parser() {
	//ommand comm;
	//comm.comm_e = Restore_Default; /* just for default value  */
	//check_valid_tail(&comm);
	//check_verify_command(&comm);
	//check_get_non_whitespace_offset();
	//check_get_non_arg_command(&comm);
	//check_get_command_with_file_name(&comm);
	//check_get_int_arg(&comm);
	//check_getXY(&comm);
	//check_get_move(&comm);
	check_parser_itself();
}
