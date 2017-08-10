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
	bool valid_arg;
	int arg1, arg2, arg3, arg4;
	char *file_name; /* useful for load & save commands */
} Command;

void get_non_arg_command(Command *comm, const char *line, int offset, const char *comm_s);
void get_int_arg(Command *comm, const char * line, int offset, const char *comm_s, int lower_bound, int upper_bound);
#endif /* PARSER_H_ */
