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

/* line[offset] is the first char to be checked */
bool valid_tail(Command *comm, const char *line, int offset) {
	while ((offset < SP_MAX_LINE_LENGTH) && (line[offset] != '\0')) {
		if (!isspace(line[offset])) {
			//comm->comm_e = Ivalid_command; /* more non-whitespace chars than should be, therefore illegal command */
			comm->valid_arg = false;
			return false;
		}
		offset++;
	}
	return true;
}

/* make sure this is the right command
 * return true if and only if line starts with the command in comm_s
 */
bool verify_command(Command *comm, const char *line, int offset, const char *comm_s, int comm_len, bool need_param) {
	int actual_len = 0;
	//printf("offset = %d\n", offset);
	for (; (actual_len < comm_len && offset < SP_MAX_LINE_LENGTH); actual_len++, offset++) {
		if (comm_s[actual_len] != line[offset]) {
			//printf("diff: comm_s[%d] = %c, line[%d] = %c\n", actual_len, comm_s[actual_len], offset, line[offset]);
			comm->comm_e = Ivalid_command; /* it's not the command we thought it was */
			return false;
		}
	}
	if ((actual_len != comm_len) || ((offset == SP_MAX_LINE_LENGTH || line[offset] == '\0') && need_param) ||
			(!isspace(line[offset]) && line[offset] != '\0')) {
		comm->comm_e = Ivalid_command;
		return false;
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
	else { return -1; } /* no legal command is stored in srt */
}

/* should transform it into a macro?? */
void get_non_arg_command(Command *comm, const char *line, int offset, const char *comm_s) {
	int len = strlen(comm_s);
	comm->need_arg = false;
	verify_command(comm, line, offset, comm_s, len, false);
	valid_tail(comm, line, offset+len);
}

void get_command_with_file_name(Command *comm, const char *line, int offset, const char *comm_s) {
	int len = strlen(comm_s);
	comm->need_arg  = true;
	if (verify_command(comm, line, offset, comm_s, len, true)) {
		int addi_offset = get_non_whitespace_offset(line + offset + len);
		if (addi_offset == -1) {
			comm->valid_arg = false;
			comm->comm_e = Ivalid_command; /* maybe shouldn't change command-type? */
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
			comm->valid_arg = true;
			valid_tail(comm, line, file_name_offset);
		}
	}
}

/* ignoring for the moment negative numbers
 * gets a pointer to arg, fill it, and check it's within the proper bounds
 * return true if and only if the lower_bound <= number <= upper_bound
 * in the end of this function, offset is the index of the first char after the argument
 */
bool get_number(const char *line, int *offset, int *arg, char range_offset, int lower_bound, int upper_bound) {
	int n = 0;
	bool number_validity = true; /* we are optimistic */
	while (*offset < SP_MAX_LINE_LENGTH && line[*offset] == '0') {
		(*offset)++;
	} /* we consume all the zeroes */

	while (*offset < SP_MAX_LINE_LENGTH && !isspace(line[*offset]) && line[*offset] != '\0' && line[*offset] != ',' && line[*offset] != '>') {
		int m = line[*offset] - range_offset; /* typical values: range_offset = '@' or '1' or '0' ('@' is the char before 'A' in the ascii table  */
		//printf("m = %d, line[%d], = %c\n", m, *offset, line[*offset]);
		(*offset)++;
		n *= 10;
		n += m;
		if (m < 0) {
			number_validity = false; /* there is some problem with the input */
		}
	}
	number_validity = (n < lower_bound || n> upper_bound) ? false : number_validity;
	*arg = n;
	return number_validity;
}

void get_int_arg(Command *comm, const char *line, int offset, const char *comm_s, int lower_bound, int upper_bound) {
	comm->need_arg = true;
	int len = strlen(comm_s);
	if (verify_command(comm, line, offset, comm_s, len, true)) {
		int addi = get_non_whitespace_offset(line + offset + len);
		if (addi == -1) { /* there isn't any parameter */
			comm->valid_arg = false;
			comm->comm_e = Ivalid_command; /* maybe shouldn't change command-type? */
		} else {
			comm->valid_arg = true;
			int arg_offset = addi + offset + len;
			comm->args_in_range = get_number(line, &arg_offset, &comm->arg1, '0', lower_bound, upper_bound);
			valid_tail(comm, line, arg_offset); /* should check the 1 constant */
		}
	}
}

/* the form of input should be <XXXXXX,YYYYYY>, where XXXXX is in range 1-8, and YYYYYY is in range A-H */
void getXY(Command *comm, const char *line, int *offset, int *row, int *col) {
	if (*offset > SP_MAX_LINE_LENGTH - 6) { /* at least 6 char: <x,y> and '\0' */
		comm->comm_e = Ivalid_command; /* no room for parameters, treat the command as illegal */
	} else {
		if (line[*offset] == '<') {
			(*offset)++;
			comm->args_in_range = get_number(line, offset, row, '0', 1, 8) && comm->args_in_range;
			if (line[*offset] != ',') { comm->valid_arg = false; }
			(*offset)++;
			comm->args_in_range = get_number(line, offset, col, '@', 1, 8) && comm->args_in_range;
			comm->valid_arg = (line[*offset] == '>') ? true : false;
		} else {
			 comm->valid_arg = false;
		}
	}
}

void get_move_arg(Command *comm, const char *line, int offset) {
	comm->need_arg      = true;
	comm->valid_arg     = true; /* let's be optimistic */
	comm->args_in_range = true; /* let's be optimistic */
	int len = strlen("move");
	if (!verify_command(comm, line, offset, "move", len, true)) { /* not "move" after all... */
		return; /* no need for further evaluation */
	}
	offset = get_non_whitespace_offset(line + offset + len) + offset + len;
	getXY(comm, line, &offset, &(comm->arg1), &(comm->arg2)); /* getting the first coordinate */
	if (!comm->valid_arg) { return; } /* no need for further parsing */
	offset += 1; /* so now offset holds the index of the first char after the closing bracket in the first parameter */
	offset += get_non_whitespace_offset(line + offset);
	if ((offset < SP_MAX_LINE_LENGTH - 2) && (line[offset] == 't') && (line[offset+1] == 'o')) { /* checking for the "to" (first checking we're within the boundaries) */
		offset += 2 + get_non_whitespace_offset(line + offset + 2);
		getXY(comm, line, &offset, &(comm->arg3), &(comm->arg4)); /* getting the second coordinate */
		offset += 1; /* should it be 6? or 5? */
		valid_tail(comm, line, offset);
	} else { /* the "to" is not in the right place */
		comm->valid_arg = false;
	}
}

Command *parser(const char *line) {
	Command *comm = (Command *) malloc(sizeof(Command));
	assert(comm != NULL);

	comm->comm_e =  Ivalid_command;
	comm->file_name = NULL;
	comm->valid_arg = true; /* optimistic */
	int offset = get_non_whitespace_offset(line);
	if (offset == -1) { return comm; } /* invalid command */

	switch (line[offset]) {
		case 'g': /* 'game_mode' */
			comm->comm_e = Set_GameMode;
			get_int_arg(comm, line, offset, "game_mode", 1, 2);
			break;

		case 'd': /* either 'difficulty' or 'default' */
			switch (line[offset+1]) {
				case 'i': /* 'difficulty' */
					comm->comm_e = Set_Difficulty;
					get_int_arg(comm, line, offset, "difficulty", 1, 5);
					break;
				case 'e': /* 'default' */
					comm->comm_e = Restore_Default;
					get_non_arg_command(comm, line, offset, "default");
					break;
			};
			break;

		case 'u': /* either 'user_color' or 'undo' */
			switch (line[offset+1]) {
				case 's': /* 'user_color */
					comm->comm_e = Set_UserColor;
					get_int_arg(comm, line, offset, "user_color", 0, 1);
					break;
				case 'n': /* 'undo' */
					comm->comm_e = Undo_Move;
					break;
			};
			break;

		case 'l': /* 'load' */
			comm->comm_e = Load;
			get_command_with_file_name(comm, line, offset, "load");
			break;

		case 'p': /* 'print_settings */
			comm->comm_e = Print_Settings;
			get_non_arg_command(comm, line, offset, "print_setting");
			break;

		case 's': /* either 'save' or 'start' */
			switch (line[offset+1]) {
				case 'a': /* 'save' */
					comm->comm_e = Save;
					get_command_with_file_name(comm, line, offset, "save");
					break;
				case 't': /* 'start */
					comm->comm_e = Start;
					get_non_arg_command(comm, line, offset, "start");
					break;
			};
			break;

		case 'm': /* 'move' */
			comm->comm_e = Make_Move;
			get_move_arg(comm, line, offset); /* !!!!! */
			break;

		case 'r': /* 'reset' */
			comm->comm_e = Reset;
			get_non_arg_command(comm, line, offset, "reset");
			break;

		case 'q': /* 'quit' */
			comm->comm_e = Quit;
			get_non_arg_command(comm, line, offset, "quit");
			break;

		default: /* unrecognized command */
			return comm;
	}
	return comm;
}

