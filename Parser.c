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

const SP_commands comms_e[] = { Set_GameMode, Set_Difficulty, Set_UserColor, Load, Restore_Default, Print_Settings, Start, \
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
	assert(comm != NULL);

	comm->comm_e =  Ivalid_command;
	comm->valid_arg = false;
	int offset = get_non_whitespace_offset(line);
	if (offset == -1) { return comm; } /* invalid command */

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

		case 'u': /* either 'user_color' or 'undo' */
			switch (line[offset+1]) {
				case 's': /* 'user_color */
					comm->comm_e = Set_UserColor;
					get_user_colur(comm, line, offset);
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
			get_non_arg_command(comm, line, "print_setting");
			break;

		case 's': /* either 'save' or 'start' */
			switch (line[offset+1]) {
				case 'a': /* 'save' */
					comm->comm_e = Save;
					get_file_name(comm, line, offset, "save");
					break;
				case 't': /* 'start */
					comm->comm_e = Start;
					get_non_arg_command(comm, line, "start");
					break;
			};
			break;

		case 'm': /* 'move' */
			comm->comm_e = Make_Move;
			get_move_arg(comm, line, offset);
			break;

		case 'r': /* 'reset' */
			comm->comm_e = Reset;
			get_non_arg_command(comm, line, "reset");
			break;

		case 'q': /* 'quit' */
			comm->comm_e = Quit;
			get_non_arg_command(comm, line, "quit");
			break;

		default: /* unrecognized command */
			comm->comm_e = Ivalid_command;
	}
	return comm;
}

