/*
 * Parser.c
 *
 *  Created on: 10 баев„ 2017
 *      Author: User
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "Parser.h"

void free_command(Command *comm) {
	if (comm->file_name != NULL) {
		free(comm->file_name);
	}
	free(comm);
}

/* make sure this is the right command
 * return true if and only if line starts with the comm_s
 */
bool verify_command(Command *comm, const char *line, int offset, const char *comm_s, int comm_len) {
	int actual_len = 0;
	for (; (actual_len < comm_len && offset < SP_MAX_LINE_LENGTH); actual_len++, offset++) {
		if (comm_s[actual_len] != line[offset]) {
			comm->comm_e = Ivalid_command; /* it's not the command we thought it was */
			return false;
		}
	}
	if ((actual_len != comm_len) || (offset <= SP_MAX_LINE_LENGTH && !isspace(line[offset]) && line[offset] != '\0')) {
		comm->comm_e = Ivalid_command;
	}
	return true;
}

int get_non_whitespace_offset(const char str[]) {
	int i = 0;
	while ((i < SP_MAX_LINE_LENGTH) && (isspace(*str)) && *str != '\0') {
			i++;
			str++;
	}
	if (i < SP_MAX_LINE_LENGTH && *str != '\0') { return i; } /* found a beginning of a word */
	else { return -1; } /* no legal command is stored in str */
}

void get_non_arg_command(Command *comm, const char *line, int offset, const char *comm_s, SP_commands type, Mode mode) {
	comm->comm_e = type;
	comm->mode = mode;
	int len = strlen(comm_s);
	verify_command(comm, line, offset, comm_s, len);
}

void get_command_with_file_name(Command *comm, const char *line, int offset, const char *comm_s, SP_commands type, Mode mode) {
	comm->comm_e = type;
	comm->mode = mode;
	int len = strlen(comm_s);
	if (verify_command(comm, line, offset, comm_s, len)) {
		int addi_offset = get_non_whitespace_offset(line + offset + len);
		if (addi_offset == -1) {
			comm->file_name = NULL;
		} else {
			int file_name_offset =  addi_offset + offset + len;
			comm->file_name = (char *) malloc(sizeof(char) * MAX_FILE_NAME);
			assert(comm->file_name != NULL);
			int j = 0;
			while ((file_name_offset < SP_MAX_LINE_LENGTH) && (j < MAX_FILE_NAME - 1)
					&& (!isspace(line[file_name_offset])) && line[file_name_offset] != '\0') {
				comm->file_name[j] = line[file_name_offset];
				file_name_offset++;
				j++;
			}
			comm->file_name[j] = '\0'; /* terminating the file-name */
		}
	}
}

/* new get-number func!!!!!
 * offset is assumed not a whitespace
 * read line[*offset], store it inside arg, and check it's within the proper bounds.
 * advances offset by one after reading the number
 * checks that the following char is either a whitespace, '\0', or a "valid following char"
 * return true if and only if the number is within the proper bounds and has proper following char
 * according to Moav, we can ignore pathological cases such as -1, 00001 etc
 */
bool get_number(const char *line, int *offset, int *arg, char range_offset, int lower_bound,
		int upper_bound, char valid_following, bool end_command) {
	*arg = line[*offset] - range_offset;
	(*offset)++;
	// checking the parameter is in the right range, and the following char is valid
	if (*arg > upper_bound || *arg < lower_bound) { /* the parameter is not a legal in the right range */
		return false;
	}
	if (end_command) { /* the following char should be '\0' or white space */
		if (*offset < SP_MAX_LINE_LENGTH && !isspace(line[*offset]) && line[*offset] != '\0') {
			return false;
		}
	} else if (*offset >= SP_MAX_LINE_LENGTH || (line[*offset] != valid_following)) { /* the following char is inadequate */
				return false;
	}
	return true;
}

void get_int_arg(Command *comm, const char *line, int offset, const char *comm_s,
		int lower_bound, int upper_bound, SP_commands type, Mode mode) {
	comm->comm_e = type;
	comm->mode = mode;
	int len = strlen(comm_s);
	if (verify_command(comm, line, offset, comm_s, len)) {
		int addi = get_non_whitespace_offset(line + offset + len);
		if (addi == -1) { /* there isn't any parameter */
			comm->args_in_range = false;
		} else {
			int arg_offset = addi + offset + len;
			comm->args_in_range = get_number(line, &arg_offset, &comm->arg1, '0', lower_bound, upper_bound, '\0', true);
		}
	}
}

/* get XY coordinates in the form of <X,Y>, where X is in range 1-8, and Y is in range A-H
 * return true if and only if managed to read the coordinates
 */
bool getXY(Command *comm, const char *line, int *offset, int *row, int *col, int needed_space, bool first_coordinate) {
	if (*offset > SP_MAX_LINE_LENGTH - needed_space) { /* */
		comm->comm_e = Ivalid_command; /* not enough room for parameters, treat the command as illegal */
		return false;
	} else {
		if (line[*offset] == '<') { /* the coordinate should start with '<'*/
			(*offset)++;
			comm->args_in_range = get_number(line, offset, row, '1', 0, 7, ',', false) && comm->args_in_range;
			if ((line[*offset] != ',') || (!comm->args_in_range)){ /* if first argument is faulty, no need for further parsing */
				comm->args_in_range = false;
				return false;
			}
			(*offset)++;
			comm->args_in_range = get_number(line, offset, col, 'A', 0, 7, '>', false) && comm->args_in_range;
			if (line[*offset] != '>') { /* should be at the end of the parameter */
				comm->args_in_range = false;
			}
			(*offset)++;
			if (!isspace(line[(*offset)]) && (first_coordinate || line[*offset] != '\0')) {
				comm->args_in_range = false;
			} /* after the first parameter should be a whitespace, and after the second parameter it's ok to have whitespace or '\0' */

		} else {
			comm->args_in_range = false;
		}
	}
	return comm->args_in_range;
}

void coordinates_commands(Command *comm, const char *line, int offset, const char *comm_name,
		SP_commands type) {
	comm->comm_e = type;
	comm->mode = GameMode;
	comm->args_in_range = true; /* let's be optimistic */
	bool is_move_commands = (type == Make_Move);
	int len = strlen(comm_name);
	if (!verify_command(comm, line, offset, comm_name, len)) { /* not "move" after all... */
		return; /* no need for further evaluation */
	}
	int additional_offset = get_non_whitespace_offset(line + offset + len);
	if (additional_offset == -1) { /* didn't find the first coordinate */
		comm->comm_e = Ivalid_command;
		return;  /* no need for further parsing */
	}
	offset = additional_offset + offset + len;
	int needed_space = strlen("<x,y> "); /* also need room for the whitespace after the first coordinate, and '\0' or whitespace after the second coordinate */
	bool got_param = getXY(comm, line, &offset, &(comm->arg1), &(comm->arg2), needed_space, is_move_commands); /* getting the first coordinate */
	if ((!got_param) || (!is_move_commands)) {
		return;  /* something is wrong with the first coordinate or it's get_moves or castling command, no need for further parsing */
	}
	additional_offset = get_non_whitespace_offset(line + offset);
	if (additional_offset == -1) { /* didn't find the "to" */
		comm->comm_e = Ivalid_command;
		return;  /* no need for further parsing */
	}
	offset += additional_offset;
	if ((offset >= SP_MAX_LINE_LENGTH - 2) || (line[offset] != 't') || (line[offset+1] != 'o')) { /* something wrong with the "to" */
		comm->comm_e = Ivalid_command;
		return;  /* no need for further parsing */
	}
	additional_offset = get_non_whitespace_offset(line + offset + 2);
	if (additional_offset == -1) { //* didn't find the second coordinate */
		comm->comm_e = Ivalid_command;
		return;  /* no need for further parsing */
	}
	offset += 2 + additional_offset;
	getXY(comm, line, &offset, &(comm->arg3), &(comm->arg4), needed_space, false); /* getting the second coordinate */
}

Command *init_command() {
	Command *comm = (Command *) malloc(sizeof(Command));
	assert(comm != NULL);
	comm->comm_e =  Ivalid_command;
	comm->file_name = NULL;
	return comm;
}

Command *parser(const char *line) {
	Command *comm = init_command();
	int offset = get_non_whitespace_offset(line);
	if ((offset == -1) || (offset+1 >= SP_MAX_LINE_LENGTH)) { return comm; } /* invalid command */
	switch (line[offset]) { /* switching the leading char of the command */
		case 'c': /* 'castle' */
			coordinates_commands(comm, line, offset, "castle", Castle);
			break;
		case 'g': /* 'game_mode' or 'get_moves' */
			switch (line[offset+1]) {
			case 'a': /* 'game_mode' */
				get_int_arg(comm, line, offset, "game_mode", 1, 2, Set_GameMode, SettingsMode);
				break;
			case 'e': /* 'game_mode' or 'get_moves' */
				coordinates_commands(comm, line, offset, "get_moves", Get_Moves);
				break; /* 'get_moves' */
			}
			break;
		case 'd': /* either 'difficulty' or 'default' */
			switch (line[offset+1]) {
				case 'i': /* 'difficulty' */
					get_int_arg(comm, line, offset, "difficulty", 1, 5, Set_Difficulty, SettingsMode);
					break;
				case 'e': /* 'default' */
					get_non_arg_command(comm, line, offset, "default", Restore_Default, SettingsMode);
					break;
			};
			break;
		case 'u': /* either 'user_color' or 'undo' */
			switch (line[offset+1]) {
				case 's': /* 'user_color */
					get_int_arg(comm, line, offset, "user_color", 0, 1, Set_UserColor, SettingsMode);
					break;
				case 'n': /* 'undo' */
					get_non_arg_command(comm, line, offset, "undo", Undo_Move, GameMode);
					break;
			};
			break;
		case 'l': /* 'load' */
			get_command_with_file_name(comm, line, offset, "load", Load, SettingsMode);
			break;
		case 'p': /* 'print_settings */
			get_non_arg_command(comm, line, offset, "print_setting", Print_Settings, SettingsMode);
			break;
		case 's': /* either 'save' or 'start' */
			switch (line[offset+1]) {
				case 'a': /* 'save' */
					get_command_with_file_name(comm, line, offset, "save", Save, GameMode);
					break;
				case 't': /* 'start */
					get_non_arg_command(comm, line, offset, "start", Start, SettingsMode);
					break;
			};
			break;
		case 'm': /* 'move' */
			coordinates_commands(comm, line, offset, "move", Make_Move);
			break;
		case 'r': /* 'reset' */
			get_non_arg_command(comm, line, offset, "reset", Reset, GameMode);
			break;
		case 'q': /* 'quit' */
			get_non_arg_command(comm, line, offset, "quit", Quit, Both);
			break;
		default: /* unrecognized command */
			return comm; /* Ivalid_command by default */
	}
	return comm;
}

