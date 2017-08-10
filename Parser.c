/*
 * Parser.c
 *
 *  Created on: 10 баев„ 2017
 *      Author: User
 */

#include <string.h>
#include "Parser.h"

const char comms_s[] = { "game_mode", "difficulty", "user_color", "load", "default", "print_setting", "start", \
		"move", "save", "undo", "reset", \
		"quit"
};

const SP_commands comms_e[] = { Set_GameMode, Set_Difficulty, Set_UserColur, Load, Restore_Default, Print_Settings, Start, \
								Make_Move, Save, Undo_Move, Reset, \
								Quit, \
								Ivalid_command
};


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

Command *parser(const char *line) {
	Command *comm = (Command *) malloc(sizeof(Command));
	comm->comm_e =  Ivalid_command;
	comm->valid_arg = false;
	int offset = get_non_whitespace_offset(line);
	switch (line[offset]) {
		case 'g': /* 'game_mode' */
			comm->comm_e = Set_GameMode;
			get_game_mode(comm, line, offset);
			break;
		case 'd': /* either 'difficulty' or 'default' */
			switch (line[offset+1]) {
				case 'i': /* 'difficulty' */
					comm->comm_e = Set_Difficulty;
					get_difficulty(comm, line, offset);
					break;
				case 'e': /* 'default' */
					comm->comm_e = Restore_Default;
					get_non_arg_command(comm, line, offset, "default");
					break;
			};
			break;
		case 'u': /* 'user_color' */
			get_user_colur(comm, line, offset);
			comm->comm_e = Set_UserColur;
			break;
		case 'l': /* 'load' */
			get_file_name(comm, line, offset, "load");
			break;
		case 'p': /* 'print_settings */
			comm->comm_e = Print_Settings;
			get_non_arg_command(comm, line, "print_setting");
			break;
		case 's': /* either 'save' or 'start' */
			switch (line[offset+1]) {
				case 'a': /* 'start' */
			}
	}
	return comm;
}

