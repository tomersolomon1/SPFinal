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
#include "ConsoleMode.h"

#include "ConsoleDataDefinitions.h"
#include "Parser.h"
#include "GameBoard.h"
#include "MiniMax.h"
#include "Files.h"

void ask_move(Gameboard *gameboard, bool to_print) {
	char *colors[] = {"black", "white"};
	if (to_print) {
		print_board(gameboard);
	}
	printf("%s player - enter your move:\n", colors[gameboard->user_color]);
	fflush(stdout);
}

/* presenting the computer's move */
void present_computer_move(Step *step, Piece *moving_piece, Piece_type promote_to) {
	char ABC[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	char *pieces_str[] = {"pawn", "knight", "bishop", "rook", "queen", "king"};
	if (IS_CASTLING_STEP(moving_piece, step)) {
		int rook_col = (step->dcol == KING_SIDE_CASTLING_COL) ? BOARD_SIZE-1 : 0;
		printf("Computer: castle King at <%d,%c> and Rook at <%d,%c>\n",
				1+step->srow, ABC[step->scol],  1+step->srow, ABC[rook_col]);
	} else if (step->src_previous_state == Was_promoted) {
		printf("Computer: move pawn <%d,%c> to <%d,%c> and promote to %s\n",
				1+step->srow, ABC[step->scol],  1+step->drow, ABC[step->dcol], pieces_str[promote_to]);
	} else {
		printf("Computer: move %s at <%d,%c> to <%d,%c>\n",
				pieces_str[moving_piece->type], 1+step->srow, ABC[step->scol],  1+step->drow, ABC[step->dcol]);
	}
}

/* performs a computer move only in mode 1, and when we start the game it's not the user's turn
 * return false if the game is over (otherwise return true)
 */

bool console_begin_game(Gameboard *gameboard) {
	bool game_stil_on = true; /* let's be optimistic */
	if (CHECK_COMPUTER_START(gameboard)) { /* checks if it's the computer's turn */
		Gameboard *copy = copy_board(gameboard);
		StepValue *best_move = find_best_step(copy, copy->difficulty);
		Step *step = best_move->step;
		destroy_board(copy);
		Piece *moving_piece = gameboard->board[step->srow][step->scol];
		present_computer_move(step, moving_piece, best_move->promote_to);
		int over = make_single_move(gameboard, step->srow, step->scol, step->drow, step->dcol, false, best_move->promote_to);
		game_stil_on = (over == 1) ? false : true; /* make_single_move returns either 0, 1 or 2, only 1 is end-of-game*/
		destroy_step_value(best_move);
	}
	if (game_stil_on) {
		ask_move(gameboard, true);
	}
	return game_stil_on;
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
		change_game_difficulty(gameboard, comm->arg1);
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
	//print_board(*gameboard_p);
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

void handle_user_promotion(Gameboard *gameboard, int drow, int dcol) {
	Piece_type promote_to = Empty;
	while (promote_to == Empty) {
		printf("Pawn promotion- please replace the pawn by queen, rook, knight, bishop or pawn:\n");
		fflush(stdout);
		char *line = (char *) malloc(sizeof(char)*(SP_MAX_LINE_LENGTH+1));
		assert(line != NULL);
		fgets(line, SP_MAX_LINE_LENGTH, stdin);
		promote_to = get_promotion_type_parser(line);
		free(line);
		if (promote_to == Empty) {
			printf("Invalid Type\n");
		}
	}
	make_promotion(gameboard, drow, dcol, promote_to);
}

/* return 0 if it's illegal move, 1 if the game is over, and 2 if the game is not over */
int make_single_move(Gameboard *gameboard, int srow, int scol, int drow, int dcol,
		bool user_turn, Piece_type computer_promotion) {
	char *colors[] = {"black", "white"};
	//CHESS_BOARD_MESSAGE move_message = set_step(gameboard, srow, scol, drow, dcol, false);
	CHESS_BOARD_MESSAGE move_message = commit_move(gameboard, srow, scol, drow, dcol, false, computer_promotion);
	if (move_message != CHESS_BOARD_SUCCESS && move_message != CHESS_BOARD_PROMOTION) {
		if (move_message == CHESS_BOARD_INVALID_MOVE_NO_PIECE) {
			printf("The specified position does not contain your piece\n");
		} else {
			printf("Illegal move\n");
		}
		return 0;
	} else if (move_message == CHESS_BOARD_PROMOTION && user_turn) {
		handle_user_promotion(gameboard, drow, dcol);
	}
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
	return 2; /* the game is still on */
}

/* return -1 if illegal move, 0 if the game is over, and 1 otherwise */
int make_move(Gameboard *gameboard, Command *comm) {
	if (comm->args_in_range) { /* the move coordinates represent a valid squares on the board */
		int move_consequences = make_single_move(gameboard, comm->arg1, comm->arg2, comm->arg3, comm->arg4, true, Empty);
		if (move_consequences == 1) { /* the game is over */
			return 0;
		} else if (move_consequences == 2) { /* legal move, and the game is still on */
			if (gameboard->game_mode == 2) { /* there are two players */
				ask_move(gameboard, true);
				return 1;
			} else { /* it's now the computer turn */
				Gameboard *copy = copy_board(gameboard);
				StepValue *best_move = find_best_step(copy, copy->difficulty);
				Step *step = best_move->step;
				destroy_board(copy); /* we don't need it anymore */
				Piece *moving_piece = gameboard->board[step->srow][step->scol];
				present_computer_move(step, moving_piece, best_move->promote_to);
				move_consequences = make_single_move(gameboard, step->srow, step->scol, step->drow, step->dcol, false, best_move->promote_to);
				destroy_step_value(best_move);
				if (move_consequences == 1) { /* the game is over */
					return 0;
				} else { /* the game is still on */
					ask_move(gameboard, true);
					return 1;
				}
			}
		} else { /* illegal move, the user should try again */
			return -1; /* the game is still on */
		}
	} else { /* invalid position on the board */
		printf("Invalid position on the board\n");
		return -1; /* the game is still on */
	}
}

/* return -1 if illegal move, 0 if the game is over, and 1 otherwise */
int castling_move(Gameboard *gameboard, Command *comm) {
	if (!comm->args_in_range) {
		printf("Invalid position on the board\n");
		return -1;
	} else if (gameboard->board[comm->arg1][comm->arg2]->colur != gameboard->turn) {
		printf("The specified position does not contain your piece\n");
		return -1;
	} else if (gameboard->board[comm->arg1][comm->arg2]->type != Rook) {
		printf("Wrong position for a rook\n");
		return -1;
	} else if (gameboard->board[comm->arg1][comm->arg2]->has_moved
			|| gameboard->all_pieces[gameboard->turn][KING_INDEX]->has_moved) {
		printf("Illegal castling move\n"); /* we can't have castling if the rook or the king has moved */
		return -1;
	} else {
		// the rook is fine, we to check the king is in the right place
		int base_row = gameboard->turn ? WHITE_ROW : BLACK_ROW;
		int dcol = (comm->arg2 == 7) ? 6 : 2; /* the king should move to <row,G> or <row,C> */
		comm->arg1 = base_row;
		comm->arg2 = KING_STARTING_COL;
		comm->arg3 = base_row;
		comm->arg4 = dcol;
		if (!is_valid_step(gameboard, comm->arg1, comm->arg2, comm->arg3, comm->arg4)) {
			printf("Illegal castling move\n"); /* we can't have castling if */
			return -1;
		}
		return make_move(gameboard, comm);
	}
}

bool op_handler(Gameboard *gameboard, Command *comm, Op op) {
	int succeded = (*op)(gameboard, comm);
	if (succeded == -1) { /* had some problem along the way */
		ask_move(gameboard, false); /* this type of op's don't print the board in case of error */
		return true;
	} else {
		bool res = (succeded == 1) ? true : false;
		return res;
	}
}

/* used for sorting the steps by qsort function */
int steps_comperator(const void *p, const void *q) {
	Step **step1 = (Step **) p;
	Step **step2 = (Step **) q;
	if ((*step1)->src_previous_state == Castling_Move && (*step2)->src_previous_state == Castling_Move) {
		return (*step1)->dcol - (*step2)->dcol; /* dcol */
	} else if ((*step1)->src_previous_state == Castling_Move && (*step2)->src_previous_state != Castling_Move) {
		return 1;
	} else if ((*step1)->src_previous_state != Castling_Move && (*step2)->src_previous_state == Castling_Move) {
		return -1;
	} else { /* neither of the steps is castling move */
		if ((*step1)->drow == (*step2)->drow) {
			return (*step1)->dcol - (*step2)->dcol;
		} else {
			return (*step1)->drow - (*step2)->drow;
		}
	}
}

void present_all_moves(Gameboard *gameboard, Piece *piece) {
	char ABC[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	char decimal_numbers[] = { '1', '2', '3', '4', '5', '6', '7', '8'};
	int opponent_color = 1 - piece->colur;
	if (piece->amount_steps > 0) {
		int step_size = sizeof(piece->steps[0]); /* there should be at least one step, since piece->amount_steps > 0 */
		qsort((void*) piece->steps, piece->amount_steps, step_size, steps_comperator);
		for (int i = 0; i < piece->amount_steps; i++) {
			Step *step = piece->steps[i];
			if (step->src_previous_state == Castling_Move) {
				int rook_col = (step->dcol == KING_SIDE_CASTLING_COL) ? BOARD_SIZE-1 : 0;
				printf("castle <%c,%c>\n", decimal_numbers[step->srow], ABC[rook_col]);
			} else { /* no castling, so can't be threatened, and can't capture any piece */
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
}

int get_moves(Gameboard *gameboard, Command *comm) {
	if (gameboard->game_mode != 1 || (gameboard->difficulty != 1 && gameboard->difficulty != 2)) {
		printf("ERROR: invalid command\n"); /* get_moves is only supported in game-mode 1, and difficulty levels 1 and 2 */
		return -1;
	}
	if (!comm->args_in_range) {
		printf("Invalid position on the board\n");
		return -1;
	}
	if (gameboard->board[comm->arg1][comm->arg2]->colur != gameboard->user_color) {
		char *colors[] = {"black", "white"};
		printf("The specified position does not contain %s player piece\n", colors[gameboard->user_color]);
		return -1;
	}
	present_all_moves(gameboard, gameboard->board[comm->arg1][comm->arg2]);
	return 0;
}

void save_game(Gameboard *gameboard, Command *comm) {
	FILE *output_file = fopen(comm->file_name, "w");
	if (output_file == NULL) {
		printf("File cannot be created or modified\n");
		ask_move(gameboard, false);
	} else {
		save_xml(output_file, gameboard);
	}
	fclose(output_file);
	free(comm->file_name);
}

/* return -1 if couldn't undo the move, otherwise return 0 */
int undo_move(Gameboard *gameboard) {
	char ABC[]     = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
	char *colors[] = {"black", "white"};
	if (gameboard->game_mode == 2) {
		printf("Undo command not available in 2 players mode\n");
		return -1;
	} else if (ArrayListSize(gameboard->history) < 2) { /* no history */
		printf("Empty history, move cannot be undone\n");
		return -1;
	} else {
		Step *step = ArrayListGetFirst(gameboard->history);
		printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
				colors[1-gameboard->turn], step->drow+1, ABC[step->scol], step->srow+1, ABC[step->dcol]);
		undo_step(gameboard, false);
		step = ArrayListGetFirst(gameboard->history);
		printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
				colors[1-gameboard->turn], step->drow+1, ABC[step->scol], step->srow+1, ABC[step->dcol]);
		undo_step(gameboard, false);
		ask_move(gameboard, true);
	}
	return 0;
}

/* gameboard is a pointer to a Gameboard object, with default values */
void manage_console(Gameboard *gameboard) {
	bool keep_on = true;
	Mode console_mode = SettingsMode;
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
	fflush(stdout);
	while (keep_on) { /* keep on get new command, until the game is over, or received a 'quit' command */
		char *line = (char *) malloc(sizeof(char)*(SP_MAX_LINE_LENGTH+1));
		assert(line != NULL);
		fgets(line, SP_MAX_LINE_LENGTH, stdin); /* copying the input to line */
		Command *comm = parser(line); /* parsing the line */
		free(line); /* we don't need the line as a string anymore */
		if (comm->comm_e == Invalid_command  || (comm->mode != console_mode && comm->mode != Both)) {
			printf("ERROR: invalid command\n");
			if (console_mode == GameMode) {
				ask_move(gameboard, false); /* ask for user's move again in case of illegal command */
			}
		} else { /* appropriate command  */
			switch (comm->comm_e) {
				case Start:
					console_mode = GameMode;
					keep_on = console_begin_game(gameboard);
					break;
				case Set_GameMode:    set_game_mode(gameboard, comm); break;
				case Set_Difficulty:  set_difficulty(gameboard, comm); break;
				case Set_UserColor:   set_color(gameboard, comm); break;
				case Load:            load_file(&gameboard, comm);	break;
				case Restore_Default: resore_default_values(gameboard);	break;
				case Print_Settings:  print_settings(gameboard);	break;
				case Make_Move:
					keep_on = op_handler(gameboard, comm, make_move); /* so we know if the game is over */
					break;
				case Castle:
					keep_on = op_handler(gameboard, comm, castling_move); /* so we know if the game is over */
					break;
				case Get_Moves: op_handler(gameboard, comm, get_moves); break;
				case Save: save_game(gameboard, comm); break;
				case Undo_Move:
					if (undo_move(gameboard) < 0) {
						ask_move(gameboard, false);
					}
						break;
				case Reset:
					console_mode = SettingsMode;
					reset_board(&gameboard);
					printf("Restarting...\n");
					printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
					break;
				case Quit:
					printf("Exiting...\n");
					keep_on = false;
					break;
				case Invalid_command: /* the compiler rises warning without this case */
					break;
			}
		}
		fflush(stdout);
		free(comm);
	}
	destroy_board(gameboard);
}
