/*
 * console_tester.c
 *
 *  Created on: 17 баев„ 2017
 *      Author: User
 */

#include <stdio.h>

#include "../ConsoleMode.h"
#include "../GameBoard.h"
#include "../Parser.h"
#include "../Files.h"
#include "../MiniMax.h"
#define LEN 100
#define LEN_s "100"

 #define STR1(x)  #x
 #define STR(x)  STR1(x)

void test_strings() {
	char line[LEN+1];
	//sscanf(stdin, "%"LEN_s"s", line);
	scanf("%" LEN_s "s", line);
	printf("line1:||%s\n", line);
}

void run_console() {
	Gameboard *gameboard = create_board(1, 2, 1);
	manage_console(gameboard);
}

void check_get_moves_online() {
	Gameboard *gameboard = create_board(1, 2, 1);
	char *commands_str[] = {
			"start",
			"get_moves <1,G>",
			"reset",
			"load graphics/saved_games/save4.xml",
			"start",
			"get_moves <1,G>",
			"reset",
			"difficulty 1",
			"start",
			"get_moves <1,G>",
	};
	for (int i = 0; i < 10; i++) {
		char *line = (char *) malloc(sizeof(char)*(SP_MAX_LINE_LENGTH+1));
		assert(line != NULL);
		strcpy(line, commands_str[i]);
		Command *comm = parser(line); /* parsing the line */
		printf("line[%d]:||%s||END\n", i, line);
		switch (comm->comm_e) {
			case Load: load_file(&gameboard, comm);	break;
			case Set_Difficulty:  set_difficulty(gameboard, comm); break;
			case Start: console_begin_game(gameboard); break;
			case Get_Moves: get_moves(gameboard, comm);	break;
			case Reset: reset_board(&gameboard); break;
			default:
				printf("invalid - something went wrong\n");
				break;
		}
	}
}

void console_tester() {
	run_console();
	//check_get_moves_online();
}

void emulate_problem() {
	Gameboard *gameboard = create_board(1, 2, 1);
	Gameboard *copy = copy_board(gameboard);
	destroy_board(copy);
}





////// --------------------------  old code ---------------------------

void prune(int *alpha, int *beta, int step_value, int node_type) {
	if (node_type == 0) { /* it's a min-node */
		*beta = (*beta < step_value) ? *beta : step_value;
	} else { /* it's a max-node */
		*alpha = (*alpha > step_value) ? *alpha : step_value;
	}
}

/* assuming we can alter the board as we will
 * we assume the game is not over
 * return the best move according to the minimax-algo, using alpha-beta prunning
 * node-types:
 * 		0 - min-node
 * 		1 - max-node
 */
StepValue *MiniMaxAlgo2(Gameboard *board, int alpha, int beta, int search_depth, NodeType node_type, int eval_perspective, bool first_option) {
	StepValue *best_sv = (StepValue *) malloc(sizeof(StepValue));
	assert(best_sv != NULL);
	best_sv->step = NULL; /* defalut */
	int game_over = is_game_over(board);
	if (game_over == 1 || game_over == 0) { /* it's a checkmate */
		best_sv->value = node_type ? INT_MIN : INT_MAX;
		return best_sv;
	} else if (game_over == 2 || search_depth == 0) { /* it's a tie or a leaf in the mini-max tree */
		best_sv->value = eval(board, eval_perspective);
		return best_sv;
	} else { /* the game is still on */
		int piece_index = 0;
		while ((alpha < beta) && (piece_index < 16)) { /* each player has 16 pieces */
			Piece *current_piece = board->all_pieces[board->turn][piece_index];
			if (current_piece->alive) {
				for (int step_index = 0; (step_index < current_piece->amount_steps) && (alpha < beta); step_index++){
					Step *step = current_piece->steps[step_index];
					set_step(board, step->srow, step->scol, step->drow, step->dcol, true);
					StepValue *sv = MiniMaxAlgo2(board, alpha, beta, search_depth-1, 1-node_type, eval_perspective, first_option);
					if (update_ab(&alpha, &beta, sv->value, node_type, first_option)) {
						//printf("history-size: %d\n", ArrayListSize(board->history));
						//fflush(stdout);
						Step *good_step = ArrayListGetFirst(board->history);
						destroy_step(best_sv->step);
						best_sv->step = copy_step(good_step);
					}
					first_option = false;
					destroy_step_value(sv);
					undo_step(board, true);
				}
			}
			piece_index++;
		}
		best_sv->value = node_type ? alpha : beta; /* to be removed? */
		return best_sv;
	}
}

