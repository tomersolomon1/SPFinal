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

#include "ConsoleDataDefinitions.h"
#include "GameBasicBuildingBlocks.h"

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

/* get promotion type */
Piece_type get_promotion_type_parser(const char *line);

/* destroy command */
void free_command(Command *comm);

/* line[offset] is the first char to be checked */
void valid_commands_tail(Command *comm, const char *line, int offset);

/* get promotion type */
void valid_tail(Command *comm, const char *line, int offset);

/* make sure this is the right command
 * return true if and only if line starts with the comm_s */
bool verify_command(Command *comm, const char *line, int offset, const char *comm_s, int comm_len);

int get_non_whitespace_offset(const char *str);

void get_non_arg_command(Command *comm, const char *line, int offset, const char *comm_s, SP_commands type, Mode mode);

void get_command_with_file_name(Command *comm, const char *line, int offset, const char *comm_s, SP_commands type, Mode mode);

void get_int_arg(Command *comm, const char *line, int offset, const char *comm_s,
		int lower_bound, int upper_bound, SP_commands type, Mode mode);

/* line[offset] is assumed to be non-whitespace char
 * read line[*offset], store it inside arg, and check it's within the proper bounds.
 * advances offset by one after reading the number (offset = offset + 1)
 * checks that the following char is either a whitespace, '\0', or a "valid following char"
 * return true if and only if the number is within the proper bounds and has proper following char
 * according to Moav, we can ignore pathological cases such as -1, 00001 etc */
bool get_number(const char *line, int *offset, int *arg, char range_offset, int lower_bound,
		int upper_bound, char valid_following, bool end_command);

/* get XY coordinates in the form of <X,Y>, where X is in range 1-8, and Y is in range A-H
 * return true if and only if managed to read the coordinates
 * after calling the function, line[offset] is the first char after the coordinate (after <X,Y>) */
bool getXY(Command *comm, const char *line, int *offset, int *row, int *col, int needed_space, bool first_coordinate);

void get_move_arg(Command *comm, const char *line, int offset);

Command *parser(const char *line);

#endif /* PARSER_H_ */
