/*
 * ConsoleMode.c
 *
 *  Created on: 16 баев„ 2017
 *      Author: User
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Parser.h"
#include "DataDefinitions.h"
#include "GameBoard.h"
#include "MiniMax.h"
#include "Files.h"

# define DEBUGGING_STATE 0 /* for debugging purposes */
# define ROUNDS			 100 /* for debugging purposes */

#define in_range(x) (((x) > -1) && ((x) < (8)))

char *commands_s[] = {"game_mode", "difficulty", "user_color", "load", "default", "print_setting", "start", \
		"move", "save", "undo", "reset", "quit" };

char *colors[] = {"black", "white"};
char *colors_upper[] = {"BLACK", "WHITE"};
char ABC[]     = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

// "load s1.txt\n"
char *debug_comm[] = {"start", "move <1,B> to <3,C>\n", "move <2,C> to <4,C>\n", "move <2,F> to <4,F>\n"};

void begin_game(Gameboard *gameboard) {
	if (gameboard->game_mode == 1 && gameboard->user_color == 0) { /* performs a computer move only in mode 1, and if the user plays as black */
		Gameboard *copy = copy_board(gameboard);
		Move move = find_best_move(copy, gameboard->difficulty);
		destroy_board(copy);
		set_step(gameboard, move.srow, move.scol, move.drow, move.dcol);
	}
	print_board(gameboard);
	printf("%s player - enter your move:\n", colors[gameboard->user_color]);
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
	if (comm->arg1 == 5) { /* we are not supporting expert level */
		printf("Expert level not supported, please choose a value between 1 to 4:\n"); /* the ':' in the end of the string appears in the instructions PDF */
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
		printf("ERROR: no such color. color values are either 1 or 0.\n");
	} else {
		gameboard->user_color = comm->arg1;
	}
}

/* return true if succeeded in loading the file, otherwise return false */
bool load_file(Gameboard **gameboard_p, Command *comm) {
	FILE *input_file = fopen(comm->file_name, "r");
	if (input_file == NULL) {
		printf("File cannot be created or modified\n");
		return false;
	} else {
		destroy_board(*gameboard_p);
		*gameboard_p = load_game(input_file);
	}
	fclose(input_file);
	free(comm->file_name);
	return true;
}

void resore_default_values(Gameboard *gameboard) {
	gameboard->game_mode  = 1;
	gameboard->difficulty = 2;
	gameboard->user_color = 1;
}

void print_settings(Gameboard *gameboard) {
	printf("SETTINGS:\n");
	if (gameboard->game_mode == 1) {
		printf("GAME_MODE: 1\n");
		printf("DIFFICULTY_LVL: %d\n", gameboard->difficulty);
		printf("USER_CLR: %s\n", colors_upper[gameboard->user_color]);
	} else { /* game_mode = 2 */
		printf("GAME_MODE: 2\n");
	}
}


/* return 0 if it's illegal move, 1 if the game is over, and 2 if the game is not over */
int make_single_move(Gameboard *gameboard, int srow, int scol, int drow, int dcol) {
	CHESS_BOARD_MESSAGE move_message = set_step(gameboard, srow, scol, drow, dcol);
	if (move_message != CHESS_BOARD_SUCCESS) {
		if (move_message == CHESS_BOARD_INVALID_MOVE_NO_PIECE) {
			printf("The specified position does not contain your piece\n");
		} else {
			printf("Illegal move\n");
		}
		return 0;
	} else { /* legal move */
		int game_over = is_game_over(gameboard);
		if (game_over == 0 || game_over == 1) {
			printf("Checkmate! %s player wins the game\n", colors[game_over]);
			return 1;
		} else if (game_over == 2) {
			printf("The game is tied\n");
			return 1;
		} else { /* the game is still on! */
			if(is_under_check(gameboard)) {
				printf("Check: %s King is threatend!\n", colors[gameboard->turn]); /* "threatend" appears in the instructions PDF */
			}
		}
	}
	return 2; /* the game is still on */
}

/* return false if the game is over, otherwise return true */
bool make_move(Gameboard *gameboard, Command *comm) {
	//printf("make_move - move: <%d, %d> to <%d, %d>\n", comm->arg1, comm->arg2, comm->arg3, comm->arg4); /* for debugging */
	if (in_range(comm->arg1) && in_range(comm->arg2) && in_range(comm->arg3) && in_range(comm->arg4)) { /* the move coordinates represent a valid squares */
		int move_consequences = make_single_move(gameboard, comm->arg1, comm->arg2, comm->arg3, comm->arg4);
		if (move_consequences == 1) { /* the game is over */
			return false;
		} else if (move_consequences == 2) { /* legal move, and the game is still on */
			if (gameboard->game_mode == 2) { /* there are two different players */
				print_board(gameboard);
				printf("%s player - enter your move:\n", colors[gameboard->turn]);
				return true;
			} else { /* it's now the computer turn */
				Gameboard *copy = copy_board(gameboard);
				Move move = find_best_move(copy, gameboard->difficulty);
				destroy_board(copy);
				move_consequences = make_single_move(gameboard, move.srow, move.scol, move.drow, move.dcol);
				if (move_consequences == 1) { /* the game is over */
					return false;
				} else { /* the game is still on */
					print_board(gameboard);
					printf("%s player - enter your move:\n", colors[gameboard->turn]);
					return true;
				}
			}
		} else { /* illegal move, the user should try again */
			return true; /* the game is still on */
		}
	} else { /* invalid position on the board */
		printf("Invalid position on the board\n");
		return true; /* the game is still on */
	}
}

void save_game(Gameboard *gameboard, Command *comm) {
	FILE *output_file = fopen(comm->file_name, "w");
	if (output_file == NULL) {
		printf("File cannot be created or modified\n");
	} else {
		save_xml(output_file, gameboard);
	}
	fclose(output_file);
	free(comm->file_name);
}

void undo_move(Gameboard *gameboard) {
	if (gameboard->game_mode == 2) {
		printf("Undo command not avaialbe in 2 players mode\n"); /* "avaialbe" appears in the Instructions PDF */
	} else if (ArrayListSize(gameboard->history) < 2) { /* no history */
		printf("Empty history, move cannot be undone\n");
	} else {
		Step *step = ArrayListGetFirst(gameboard->history);
		printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
				colors[1-gameboard->turn], step->drow+1, ABC[step->scol], step->srow+1, ABC[step->dcol]);
		undo_step(gameboard);
		step = ArrayListGetFirst(gameboard->history);
		printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
				colors[1-gameboard->turn], step->drow+1, ABC[step->scol], step->srow+1, ABC[step->dcol]);
		undo_step(gameboard);
		print_board(gameboard);
		printf("%s player - enter your move:\n", colors[gameboard->turn]);
	}
}

void reset_game(Gameboard **gameboard) {
	int old_game_mode = (*gameboard)->game_mode;
	int old_user_color= (*gameboard)->user_color;
	int old_difficulty = (*gameboard)->difficulty;
	destroy_board(*gameboard);
	*gameboard = create_board(old_game_mode, old_difficulty, old_user_color);
}

/* return 1 if we should enter game-mode, and 0 if should quit */
int manage_console(Gameboard *gameboard) {
	bool keep_on = true;
	char *line = NULL;
	int res = 0;
	Mode console_mode = SettingsMode;
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
	fflush(stdout);
	int counter = 0; /* for debugging!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
	while (keep_on && counter < ROUNDS) {
		Command *comm;
		line = (char *) malloc(sizeof(char)*(SP_MAX_LINE_LENGTH+1)); /* tentative, until moav says what should we do */
		assert(line != NULL);
		if(DEBUGGING_STATE) {
			strcpy(line, debug_comm[counter]);
			comm = parser(line);
		} else {
			fgets(line, SP_MAX_LINE_LENGTH, stdin);
			printf("line||%s\n", line);
			comm = parser(line);
			free(line);
		}
		if (comm->comm_e == Ivalid_command) {
			printf("ERROR: invalid command111\n"); /* should ask in the forum? */
		} else if ((comm->mode != console_mode) && (comm->mode != Both)) { /* the command is not appropriate in the current mode */
			if (comm->comm_e == Undo_Move) {
				printf("Undo command not avaialbe in 2 players mode\n"); /* "avaialbe" appears in the instructions PDF */
			} else {
				char *mode_s = (console_mode == SettingsMode) ? "Settings" : "Game";
				printf("ERROR: command not appropriate to %s Mode\n", mode_s);
			}

		} else { /* appropriate command and enough args  */
			if (!comm->need_arg && comm->extra_param) {
				printf("ERROR: command '%s' shouldn't get any parameters", commands_s[comm->comm_e]);
			} else if (comm->need_arg && comm->extra_param) {
				printf("ERROR: command '%s' got too many parameters", commands_s[comm->comm_e]);
			} else if(comm->need_arg && !comm->valid_arg) {
				printf("ERROR: invalid parameter\n");
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
						if(load_file(&gameboard, comm)) { /* changes game-mode if loading succeeded */
							console_mode = GameMode;
						}
						break;
					case Restore_Default:
						resore_default_values(gameboard);
						break;
					case Print_Settings:
						print_settings(gameboard);
						break;
					case Make_Move:
						keep_on = make_move(gameboard, comm);
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
						printf("Exiting...\n");
						keep_on = false;
						break;
					case Ivalid_command: /* the compiler rises warning without this case */
						break;
				}
			}
		}
		counter += 1; /* for debugging */
		fflush(stdout);
		free(comm);
	}
	return res;
}
