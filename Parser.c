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

/* line[offset] is the first char to be checked */
bool valid_tail(Command *comm, const char *line, int offset) {
	while ((offset < SP_MAX_LINE_LENGTH) && (line[offset] != '\0')) {
		if (!isspace(line[offset])) {
			comm->comm_e = Ivalid_command; /* more non-whitespace chars than should be, therefore illegal command */
			return false;
		}
		offset++;
	}
	return true;
}

/* make sure this is the right command */
bool verify_command(Command *comm, const char *line, int offset, const char *comm_s, int comm_len, bool need_param) {
	int actual_len = 0;
	for (; (actual_len < comm_len && offset < SP_MAX_LINE_LENGTH); actual_len++, offset++) {
		if (comm_s[actual_len] != line[offset]) {
			comm->comm_e = Ivalid_command; /* it's not the command we thought it was */
			return false;
		}
	}
	if ((actual_len != comm_len - 1) || (offset == SP_MAX_LINE_LENGTH && need_param) ||
			(!isspace(line[offset]) && line[offset] != '\0')) {
		comm->comm_e = Ivalid_command;
		return false;
	}
	return true;
}

int get_non_whitespace_offset(const char *str) {
	int i = 0;
	int practical_max_legnth = SP_MAX_LINE_LENGTH - 4; /* if it's a valid command it should be at least 4-char length */
	while ((i < practical_max_legnth - 4) && (isspace(*str))) {
			i++;
			str++;
	}
	if (i < practical_max_legnth) { return i; }
	else { return -1; }
}

/* should transform it into a macro */
void get_non_arg_command(Command *comm, const char *line, int offset, const char *comm_s) {
	int len = strlen(comm_s);
	verify_command(comm, line, offset, comm_s, len, false);
	valid_tail(comm, line, offset+len);
}

void get_file_name(Command *comm, const char *line, int offset, const char *comm_s) {
	int len = strlen(comm_s);
	if (verify_command(comm, line, offset, comm_s, len, true)) {
		int file_name_offset = get_non_whitespace_offset(line + offset + len) + offset + len;
		comm->file_name = (char *) malloc(sizeof(char) * MAX_FILE_NAME);
		int j = 0;
		while ((file_name_offset < SP_MAX_LINE_LENGTH) && (j < MAX_FILE_NAME - 1)
				&& (!isspace(line[file_name_offset]))) {
			comm->file_name[j] = line[file_name_offset];
			file_name_offset++;
			j++;
		}
		comm->file_name[j] = '\0'; /* terminating the file-name */
		comm->valid_arg = true;
		valid_tail(comm, line, file_name_offset);
	}
}

/* gets a pointer to arg, fill it, and check it's within the proper bounds */
bool get_arg(char char_arg, int *arg, char range_offset, int lower_bound, int upper_bound) {
	*arg = char_arg - range_offset;
	if ((*arg < lower_bound) || (*arg > upper_bound)) {
		return false;
	}
	return true;
}

void get_int_arg(Command *comm, const char *line, int offset, const char *comm_s, int lower_bound, int upper_bound) {
	int len = strlen(comm_s);
	if (verify_command(comm, line, offset, comm_s, len, true)) {
		int arg_offset = get_non_whitespace_offset(line + offset + len) + offset + len;
		comm->valid_arg = get_arg(line[arg_offset], &comm->arg1, '0', lower_bound, upper_bound);
		valid_tail(comm, line, arg_offset + 1); /* should check the 1 constant */
	}
}


void getXY(Command *comm, const char *line, int offset, int *row, int *col) {
	if (offset >= SP_MAX_LINE_LENGTH - 6) { /* should check at home if it's 5 or 6 */
		comm->comm_e = Ivalid_command; /* no room for parameters, treat the command as illegal */
	} else {
		if (line[offset] == '<') {
			comm->valid_arg = get_arg(line[offset+1], row, '0', 1, 7);
			//comm->valid_arg = get_number(line, offset+1, row, '0');
			comm->valid_arg = get_arg(line[offset+1], row, 'A', 1, 7) && comm->valid_arg;
			//comm->valid_arg = get_number(line, offset+3, col, 'A') && comm->valid_arg;
			comm->comm_e = (line[offset+2] == ',') && (line[offset+4] == '>') ? Make_Move : Ivalid_command;
		}
	}
}

void get_move_arg(Command *comm, const char *line, int offset) {
	int len = strlen("move");
	if (!verify_command(comm, line, offset, "move", len, true)) { /* not move after all... */
		//int barcket_offset = get_non_whitespace_offset(line + len + offset);
		return; /* no need for further evaluation */
	}
	offset = get_non_whitespace_offset(line+offset) + offset + len;
	getXY(comm, line, offset, &(comm->arg1), &(comm->arg2)); /* getting the first coordinate */
	if (!comm->valid_arg || comm->comm_e == Ivalid_command) { return; } /* no need for further parsing */

	offset += 5; /* 5? or 6? */

	offset += get_non_whitespace_offset(line + offset);
	if ((offset < SP_MAX_LINE_LENGTH - 2) && (line[offset] == 't') && (line[offset+1] == 'o')) { /* first check there is enough space for 'to' */
		offset += 2 + get_non_whitespace_offset(line + offset + 2);
		getXY(comm, line, offset, &(comm->arg3), &(comm->arg4)); /* getting the second coordinate */
		offset += 6; /* should it be 6? or 5? */
		valid_tail(comm, line, offset);
	}
}

Command *parser(const char *line) {
	Command *comm = (Command *) malloc(sizeof(Command));
	assert(comm != NULL);

	comm->comm_e =  Ivalid_command;
	comm->valid_arg = false;
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
			get_file_name(comm, line, offset, "load");
			break;

		case 'p': /* 'print_settings */
			comm->comm_e = Print_Settings;
			get_non_arg_command(comm, line, offset, "print_setting");
			break;

		case 's': /* either 'save' or 'start' */
			switch (line[offset+1]) {
				case 'a': /* 'save' */
					comm->comm_e = Save;
					get_file_name(comm, line, offset, "save");
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

