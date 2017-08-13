/*
 * Parser.h
 *
 *  Created on: 10 баев„ 2017
 *      Author: User
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stdbool.h>

#define SP_MAX_LINE_LENGTH 1024
#define MAX_FILE_NAME 1024

typedef enum {
	/* Settings-Mode commands */
	Set_GameMode,
	Set_Difficulty,
	Set_UserColor,
	Load,
	Restore_Default,
	Print_Settings,
	Start,

	/* Game-Mode commands */
	Make_Move,
	Save,
	Undo_Move,
	Reset,

	Quit, /* this command is for both modes */
	Ivalid_command /* for invalid commands */
} SP_commands;

typedef struct command_t {
	SP_commands comm_e;
	bool need_arg;
	bool valid_arg;
	bool args_in_range;
	int arg1, arg2, arg3, arg4;
	char *file_name; /* useful for load & save commands */
} Command;

bool valid_tail(Command *comm, const char *line, int offset);
bool verify_command(Command *comm, const char *line, int offset, const char *comm_s, int comm_len, bool need_param);
int get_non_whitespace_offset(const char *str);
void get_non_arg_command(Command *comm, const char *line, int offset, const char *comm_s);
void get_command_with_file_name(Command *comm, const char *line, int offset, const char *comm_s);
void get_int_arg(Command *comm, const char *line, int offset, const char *comm_s, int lower_bound, int upper_bound);
bool get_number(const char *line, int *offset, int *arg, char range_offset, int lower_bound, int upper_bound);
#endif /* PARSER_H_ */
