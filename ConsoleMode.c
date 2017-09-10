/*
 * ConsoleMode.c
 *
 *  Created on: 16 ����� 2017
 *      Author: User
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ConsoleMode.h"
#include "Parser.h"
#include "DataDefinitions.h"
#include "GameBoard.h"
#include "MiniMax.h"
#include "Files.h"

void begin_game(Gameboard *gameboard) {
	char *colors[] = {"black", "white"};
	if (gameboard->game_mode == 1 && gameboard->user_color == 0) { /* performs a computer move only in mode 1, and if the user plays as black */
		Gameboard *copy = copy_board(gameboard);
		Step *step = find_best_step(copy, copy->difficulty);
		destroy_board(copy);
		set_step(gameboard, step->srow, step->scol, step->drow, step->dcol, false);
		destroy_step(step);
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
	char *colors_upper[] = {"BLACK", "WHITE"};
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
int make_single_move(Gameboard *gameboard, int srow, int scol, int drow, int dcol, bool user_turn) {
	char *colors[] = {"black", "white"};
	CHESS_BOARD_MESSAGE move_message = set_step(gameboard, srow, scol, drow, dcol, false);
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
			if (user_turn) {
				printf("The game is tied\n");
			} else { /* it was the computer's turn */
				printf("The game ends in a tie\n");
			}
			return 1;
		} else { /* the game is still on! */
			if(is_under_check(gameboard)) {
				if (user_turn) {
					printf("Check: %s King is threatened!\n\n", colors[gameboard->turn]);
				} else { /* it was the computer's turn */
					printf("Check!\n");
				}
			}
		}
	}
	return 2; /* the game is still on */
}

/* return false if the game is over, otherwise return true */
bool make_move(Gameboard *gameboard, Command *comm) {
	char *colors[] = {"black", "white"};
	if (comm->args_in_range) { /* the move coordinates represent a valid squares on the board */
		int move_consequences = make_single_move(gameboard, comm->arg1, comm->arg2, comm->arg3, comm->arg4, true);
		if (move_consequences == 1) { /* the game is over */
			return false;
		} else if (move_consequences == 2) { /* legal move, and the game is still on */
			if (gameboard->game_mode == 2) { /* there are two players */
				print_board(gameboard);
				printf("%s player - enter your move:\n", colors[gameboard->turn]);
				return true;
			} else { /* it's now the computer turn */
				char ABC[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
				Gameboard *copy = copy_board(gameboard);
				Step *step = find_best_step(copy, copy->difficulty);
				destroy_board(copy); /* we don't need it anymore */
				Piece *moving_piece = gameboard->board[step->srow][step->scol];
				if (IS_CASTLING_STEP(moving_piece, step)) {
					int rock_col = (step->dcol == KING_SIDE_CASTLING_COL) ? BOARD_SIZE-1 : 0;
					printf("Computer: castle King at <%d,%c> and Rook at <%d,%c>\n", 1+step->srow, ABC[step->scol],  1+step->srow, ABC[rock_col]);
				} else {
					char *pieces_str[] = {"pawn", "knight", "bishop", "rook", "queen", "king"};
					printf("Computer: move %s at <%d,%c> to <%d,%c>\n", pieces_str[moving_piece->type], 1+step->srow, ABC[step->scol],  1+step->drow, ABC[step->scol]);
				}
				move_consequences = make_single_move(gameboard, step->srow, step->scol, step->drow, step->dcol, false);
				destroy_step(step);
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

bool castling_move(Gameboard *gameboard, Command *comm) {
	if (!comm->args_in_range) {
		printf("Invalid position on the board\n");
		return true;
	} else if (gameboard->board[comm->arg1][comm->arg2]->colur != gameboard->turn) {
		printf("The specified position does not contain your piece\n");
		return true;
	} else if (gameboard->board[comm->arg1][comm->arg2]->type != Rock) {
		printf("Wrong position for a rook\n");
		return true;
	} else if (gameboard->board[comm->arg1][comm->arg2]->has_moved
			|| gameboard->all_pieces[gameboard->turn][KING_INDEX]->has_moved) {
		printf("Illegal castling move\n"); /* we can't have castling if the rock or the king has moved */
		return true;
	} else {
		// the rock is fine, we to check the king is in the right place
		int base_row = gameboard->turn ? WHITE_ROW : BLACK_ROW;
		int dcol = (comm->arg2 == 7) ? 6 : 2; /* the king should move to <row,G> or <row,C> */
		comm->arg1 = base_row;
		comm->arg2 = KING_STARTING_COL;
		comm->arg3 = base_row;
		comm->arg4 = dcol;
		if (!is_valid_step(gameboard, comm->arg1, comm->arg2, comm->arg3, comm->arg4)) {
			printf("Illegal castling move\n"); /* we can't have castling if */
			return true;
		}
		return make_move(gameboard, comm);
	}
}

/* used for sorting the steps by qsort function */
int steps_comperator(const void *p, const void *q) {
	Step **step1 = (Step **) p;
	Step **step2 = (Step **) q;
	if ((*step1)->src_previous_state == Castling_move && (*step2)->src_previous_state == Castling_move) {
		return (*step1)->dcol - (*step2)->dcol; /* dcol */
	} else if ((*step1)->src_previous_state == Castling_move && (*step2)->src_previous_state != Castling_move) {
		return -1;
	} else if ((*step1)->src_previous_state != Castling_move && (*step2)->src_previous_state == Castling_move) {
		return 1;
	}
	if ((*step1)->drow == (*step2)->drow) {
		return (*step1)->dcol - (*step2)->dcol;
	} else {
		return (*step1)->drow - (*step2)->drow;
	}
}

void present_moves(Gameboard *gameboard, Piece *piece) {
	char ABC[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	char decimal_numbers[] = { '1', '2', '3', '4', '5', '6', '7', '8'};
	int opponent_color = 1 - piece->colur;
	if (piece->amount_steps > 0) {
		int step_size = sizeof(piece->steps[0]); /* there should be at least one step, since piece->amount_steps > 0 */
		qsort((void*) piece->steps, piece->amount_steps, step_size, steps_comperator);
		for (int i = 0; i < piece->amount_steps; i++) {
			Step *step = piece->steps[i];
			printf("<%c,%c>", decimal_numbers[step->drow], ABC[step->dcol]);
			if (step->is_threatened) {
				printf("*");
			}
			if (gameboard->board[step->drow][step->dcol]->colur == opponent_color) {
				printf("^");
			}
			printf("\n");
		}
	}
}

void get_moves(Gameboard *gameboard, Command *comm) {
	if (gameboard->game_mode != 1 || (gameboard->difficulty != 1 && gameboard->difficulty != 2)) {
		printf("ERROR: invalid command\n"); /* get_moves is only supported in game-mode 1, and difficulty levels 1 and 2 */
		return;
	}
	if (!comm->args_in_range) {
		printf("Invalid position on the board\n");
		return;
	}
	if (gameboard->board[comm->arg1][comm->arg2]->colur != gameboard->user_color) {
		char *colors[] = {"black", "white"};
		printf("The specified position does not contain %s player piece\n", colors[gameboard->user_color]);
		return;
	}
	present_moves(gameboard, gameboard->board[comm->arg1][comm->arg2]);
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
	char ABC[]     = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
	char *colors[] = {"black", "white"};
	if (gameboard->game_mode == 2) {
		printf("Undo command not available in 2 players mode\n");
	} else if (ArrayListSize(gameboard->history) < 2) { /* no history */
		printf("Empty history, move cannot be undone\n");
	} else {
		Step *step = ArrayListGetFirst(gameboard->history);
		printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
				colors[1-gameboard->turn], step->drow+1, ABC[step->scol], step->srow+1, ABC[step->dcol]);
		undo_step(gameboard, false);
		step = ArrayListGetFirst(gameboard->history);
		printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
				colors[1-gameboard->turn], step->drow+1, ABC[step->scol], step->srow+1, ABC[step->dcol]);
		undo_step(gameboard, false);
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
void manage_console(Gameboard *gameboard) {
	bool keep_on = true;
	Mode console_mode = SettingsMode;
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n"); /* !!!!!! to be printed each time we are in settings mode */
	fflush(stdout);
	while (keep_on) {
		char *line = (char *) malloc(sizeof(char)*(SP_MAX_LINE_LENGTH+1));
		assert(line != NULL);
		fgets(line, SP_MAX_LINE_LENGTH, stdin);
		Command *comm = parser(line);
		free(line);
		if (comm->comm_e == Ivalid_command  || (comm->mode != console_mode && comm->mode != Both)) {
			printf("ERROR: invalid command\n");
		} else { /* appropriate command  */
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
					print_settings(gameboard);
					break;
				case Make_Move:
					keep_on = make_move(gameboard, comm); /* so we know if the game is over */
					break;
				case Castle:
					keep_on = castling_move(gameboard, comm); /* so we know if the game is over */
					break;
				case Get_Moves:
					get_moves(gameboard, comm);
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
					printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
					break;
				case Quit:
					printf("Exiting...\n");
					keep_on = false;
					break;
				case Ivalid_command: /* the compiler rises warning without this case */
					break;
			}
		}
		fflush(stdout);
		free(comm);
	}
	destroy_board(gameboard);
}
