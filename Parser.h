/*
 * Parser.h
 *
 *  Created on: 10 баев„ 2017
 *      Author: User
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stdbool.h>
#include <string.h>
#include "DataDefinitions.h"

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
	Castle,
	Get_Moves,
	Save,
	Undo_Move,
	Reset,

	Quit, /* this command is for both modes */
	Invalid_command /* for invalid commands */
} SP_commands;

typedef struct command_t {
	SP_commands comm_e;
	Mode mode; /* the mode associated with the command */
	bool args_in_range; /* for determing if the command args are in the right range */
	int arg1, arg2, arg3, arg4;
	char *file_name; /* useful for load & save commands */
} Command;

void free_command(Command *comm);
void valid_tail(Command *comm, const char *line, int offset);
bool verify_command(Command *comm, const char *line, int offset, const char *comm_s, int comm_len);
int get_non_whitespace_offset(const char *str);
void get_non_arg_command(Command *comm, const char *line, int offset, const char *comm_s, SP_commands type, Mode mode);
void get_command_with_file_name(Command *comm, const char *line, int offset, const char *comm_s, SP_commands type, Mode mode);
void get_int_arg(Command *comm, const char *line, int offset, const char *comm_s,
		int lower_bound, int upper_bound, SP_commands type, Mode mode);
bool get_number(const char *line, int *offset, int *arg, char range_offset, int lower_bound,
		int upper_bound, char valid_following, bool end_command);
bool getXY(Command *comm, const char *line, int *offset, int *row, int *col, int needed_space, bool first_coordinate);
void get_move_arg(Command *comm, const char *line, int offset);
Command *parser(const char *line);
#endif /* PARSER_H_ */
