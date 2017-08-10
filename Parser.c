/*
 * Parser.c
 *
 *  Created on: 10 баев„ 2017
 *      Author: User
 */

#include <string.h>
#include <stdlib.h>
#include "Parser.h"

bool valid_tail(Command *comm, const char *line, int offset) {
	while (offset < SP_MAX_LINE_LENGTH) {
		if (!isspace(line(offset))) { /* what about the null-char? ask moav */
			comm->comm_e = Ivalid_command; /* more chars than should be, therefore illegal */
			return false;
		}
	}
	return true;
}

/* make sure this is the right command */
bool verify_command(Command *comm, const char *line, int offset, const char *comm_s, int comm_len,
		bool need_param) {
	int actual_len = 0;
	for (; (actual_len < comm_len && offset < SP_MAX_LINE_LENGTH); actual_len++, offset++) {
		if (comm_s[offset] != line[offset]) {
			comm->comm_e = Ivalid_command; /* it's not the command we thought it was */
			return false;
		}
	}
	if ((actual_len != comm_len - 1) || (offset == SP_MAX_LINE_LENGTH && need_param) ||
			(need_param && !isspace(line[offset]))) {
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
				&& (!isspace(line(file_name_offset)))) {
			comm->file_name[j] = line[file_name_offset];
			file_name_offset++;
			j++;
		}
		comm->file_name[j] = '\0'; /* terminating the file-name */
		valid_tail(comm, line, file_name_offset);
	}
}

get_user_color(comm, line, offset) {

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
			get_game_mode(comm, line, offset); /* !!!!! */
			break;

		case 'd': /* either 'difficulty' or 'default' */
			switch (line[offset+1]) {
				case 'i': /* 'difficulty' */
					comm->comm_e = Set_Difficulty;
					get_difficulty(comm, line, offset); /* !!!!! */
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
					get_user_color(comm, line, offset); /* !!!!! */
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

