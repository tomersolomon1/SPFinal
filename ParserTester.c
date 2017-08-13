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
	offset = get_non_whitespace_offset(line2);
	get_int_arg(comm, line3, offset, "difficulty", 1, 5);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d, comm->arg1: %d\n\n",\
			line3, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1);
	// offset = 0, command-comm_e = invalid, arg is valid (????), not in range


	char line4[] = "difficulty    00000000000004"; // checks recognizing command, too much parameters
	comm->comm_e = Set_Difficulty;
	offset = get_non_whitespace_offset(line2);
	get_int_arg(comm, line4, offset, "difficulty", 1, 5);
	printf("line:%s|END|\noffset = %d, comm->comm_e = %s, comm->valid_arg: %d\ncomm->arg_in_range: %d, comm->arg1: %d\n\n",\
			line4, offset, commands_es[comm->comm_e], comm->valid_arg, comm->args_in_range, comm->arg1);
	// offset = 0, command-comm_e = Set_Difficulty, arg is valid, not in range

}

void check_parser() {
	Command comm;
	comm.comm_e = Restore_Default; /* just for default value  */
	//check_valid_tail(&comm);
	//check_verify_command(&comm);
	//check_get_non_whitespace_offset();
	//check_get_non_arg_command(&comm);
	//check_get_command_with_file_name(&comm);
	check_get_int_arg(&comm);
}
