/*
 * ConsoleMode.c
 *
 *  Created on: 16 баев„ 2017
 *      Author: User
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Parser.h"
#include "DataDefinitions.h"

char *commands_s[] = {"game_mode", "difficulty", "user_color", "load", "default", "print_setting", "start", \
		"move", "save", "undo", "reset", "quit" };

void set_game_mode(Gameboard *gameboard) {
	destroy_board(gameboard);
	printf("Exiting...\n");
}

void begin_game(Gameboard *gameboard) {
	if (gameboard->game_mode == 1 && gameboard->user_color == 0) { /* performs a computer move only in mode 1, and if the user plays as black */
		Gameboard *copy = copy_board(gameboard);
		Move move = find_best_move(copy, gameboard->difficulty);
		set_step(gameboard, move.srow, move.scol, move.drow, move.dcol);
		destroy_board(copy);
	}
	print_board(gameboard);
}

void set_game_mode(Gameboard *gameboard, Command *comm) {
	if (comm->arg1 > 2 || comm->arg1 < 1) {
		printf("Wrong game mode\n");
	} else {
		gameboard->game_mode = comm->arg1;
		char *message = comm->arg1 == 1 ? "1 player" : "2 players";
		printf("Game mode is set to %s\n", message);
	}
}

void set_difficulty(Gameboard *gameboard, Command *comm) {
	if (comm->arg1 == 5) {
		printf("Expert level not supported, please choose a value between 1 to 4:\n");
	} else if (comm->arg1 > 5 || comm->arg1 < 1) {
		printf("Wrong difficulty level. The value should be between 1 to 5\n");
	} else { /* arg is in the right range */
		gameboard->difficulty = comm->arg1;
	}
}

void set_color(Gameboard *gameboard, Command *comm) {
	if (gameboard->game_mode == 2) {
		printf("ERROR: invalid command when GAME-MODE = 2\n");
	} else if (comm->arg1 > 1 || comm->arg1 < 0) {
		printf("ERROR: no such color. color values are either 1 or 0\n");
	} else {
		gameboard->user_color = comm->arg1;
	}
}

void load_file(Gameboard **gameboard_p, Command *comm) {
	FILE *input_file = fopen(comm->file_name, 'r');
	assert(input_file != NULL);
	destroy_board(*gameboard_p);
	*gameboard_p = load_game(input_file);
	fclose(input_file);
	free(comm->file_name);
}

void resore_default_values(Gameboard *gameboard) {
	gameboard->game_mode  = 1;
	gameboard->difficulty = 2;
	gameboard->user_color = 1;
}

void save_game(Gameboard *gameboard, Command *comm) {
	FILE *output_file = fopen(comm->file_name, 'w');
	assert(output_file != NULL);
	save_xml(output_file, gameboard);
	fclose(output_file);
	free(comm->file_name);
}

void undo_move(Gameboard *gameboard) {

}

void reset_game(Gameboard **gameboard) {
	int old_game_mode = *gameboard->game_mode;
	int old_user_color= *gameboard->user_color;
	int old_difficulty = *gameboard->difficulty;
	destroy_board(*gameboard);
	*gameboard = create_board(old_game_mode, old_difficulty, old_user_color);
}

void quit_game(Gameboard *gameboard) {
	free(gameboard);
	printf("Exiting...\n");
}

/* return 1 if we should enter game-mode, and 0 if should quit */
int manage_console(Gameboard *gameboard) {
	//bool keep_on = true;
	char *line = NULL;
	int res = 0;
	Mode console_mode = SettingsMode;
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n");

	while (true) {
		line = (char *) malloc(sizeof(char)*(SP_MAX_LINE_LENGTH+1)); /* tentative, until moav says what should we do */
		assert(line != NULL);
		scanf("SP_MAX_LINE_LENGTH%s", line); /* safe reading from line? asked in the forum */
		Command *comm = parser(line);
		free(line);
		if (comm->comm_e == Ivalid_command) {
			printf("ERROR: invalid command\n"); /* should ask in the forum? */
		} else if (comm->mode != GameMode){
			if (comm->mode == Undo_Move) {
				printf("Undo command not avaialbe in 2 players mode\n"); /* "avaialbe" appears in the instructions PDF */
			} else {
				char *mode_s = (console_mode == SettingsMode) ? "Settings" : "Game";
				printf("ERROR: command not appropriate to %s Mode\n", mode_s);
			}

		} else { /* appropriate command and enough args  */
			if (!comm->need_arg && comm->extra_param) {
				printf("ERROR: command '%s' shouldn't get any parameters", commands_s[comm]);
			} else if (comm->need_arg && comm->extra_param) {
				printf("ERROR: command '%s' got too many parameters", commands_s[comm]);
			} else if(comm->need_arg && !comm->valid_arg) {
				printf("ERROR: invalid parameter\n");
			} else if (comm->comm_e == Start || comm->comm_e == Quit) {
				res = (comm->comm_e == Start) ? 1 : 0;
			} else { /* the command is appropriate for the mode, and got enough parameters */
				switch (comm->comm_e) {
					case Start:
						console_mode = GameMode;
						begin_game(gameboard);
						break;
					case Set_GameMode:
						set_game_mode(gameboard, comm);
						break;
					case Set_Difficulty:
						set_difficulty(gameboard, comm);
						break;
					case Set_UserColor:
						set_color(gameboard, comm);
						break;
					case Load:
						load_file(&gameboard, comm);
						break;
					case Restore_Default:
						resore_default_values(gameboard);
						break;
					case Print_Settings:
						print_board(gameboard);
						break;
					case Make_Move:
						make_move(gameboard, comm);
						break;
					case Save:
						save_game(gameboard, comm);
						break;
					case Undo_Move:
						undo_move(gameboard);
						break;
					case Reset:
						console_mode = SettingsMode;
						reset_game(&gameboard);
						break;
					case Quit:
						quit_game(gameboard);
						return res;
				}
			}
		}
		free(comm);
	}
	return res;
}

int game_mode(Gameboard *gameboard) {
	return 1;
}

void console_mode() {
	int mode = 1; /* 1 is settings mode, 0 is game mode */


}
