/*
 * MiniMax.c
 *
 *  Created on: 14 баев„ 2017
 *      Author: User
 */

#include <limits.h>
#include "GameBoard.h"
#include "MiniMax.h"

void destroy_step_value(StepValue *sv) {
	if (sv != NULL) {
		destroy_step(sv->step);
		free(sv);
	}
}

int sumup_pieces(Gameboard *board, int color) {
	int pieces_value[] = {1, 3, 3, 5, 9, 100 };
	int sumup = 0;
	for (int i = 0; i < 16; i++) {
		if (board->all_pieces[color][i]->alive) {
			sumup += pieces_value[board->all_pieces[color][i]->type];
		}
	}
	return sumup;
}

/*
 * eval_pespective is the color of the root of the mini-max tree
 */
int eval(Gameboard *board, int eval_perspective) {
	int white_pieces = sumup_pieces(board, 1);
	int black_pieces = sumup_pieces(board, 0);
	int result = eval_perspective ? white_pieces - black_pieces : black_pieces - white_pieces;
	return result;
}

/* return true if found a better option */
bool update_ab(int *alpha, int *beta, int step_value, NodeType node_type, bool first_move) {
	if (node_type == MinNode && (*beta > step_value || first_move)) { /* it's a min-node */
		//*beta = (*beta < step_value) ? *beta : step_value;
		*beta = step_value;
		return true; /* found a better option */
	} else if (node_type == MaxNode && (*alpha < step_value || first_move)) { /* it's a max-node */
		//*alpha = (*alpha > step_value) ? *alpha : step_value;
		*alpha = step_value;
		return true; /* found a better option */
	}
	else {
		return false;
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
					set_step(board, step->srow, step->scol, step->drow, step->dcol);
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
					undo_step(board);
				}
			}
			piece_index++;
		}
		best_sv->value = node_type ? alpha : beta; /* to be removed? */
		return best_sv;
	}
}

/* assuming we can alter the board as we will
 * we assume the game is not over
 */
Step *find_best_step(Gameboard *board, int search_depth) {
	printf("\nstart-of-move\n");
	int alpha = INT_MIN;
	int beta  = INT_MAX;
	int eval_perspective = board->turn;
	StepValue *best_sv = MiniMaxAlgo2(board, alpha, beta, search_depth, MaxNode, eval_perspective, true);
	Step *best_step = copy_step(best_sv->step);
	destroy_step_value(best_sv);
	printf("end-of-move\n");
	return best_step;
}

////// -----------------------  old code -----------------

void prune(int *alpha, int *beta, int step_value, int node_type) {
	if (node_type == 0) { /* it's a min-node */
		*beta = (*beta < step_value) ? *beta : step_value;
	} else { /* it's a max-node */
		*alpha = (*alpha > step_value) ? *alpha : step_value;
	}
}

/* the root of the mini-max algorithm, it's a max node */
Move find_best_move(Gameboard *board, int search_depth) {
	int alpha = INT_MIN;
	int beta  = INT_MAX;
	Move best_move;
	int piece_index = 0;
	bool first_option = true;
	int eval_perspective = board->turn;
	while ((alpha < beta) && (piece_index < 16)) { /* each player has 16 pieces */
		Piece *current_piece = board->all_pieces[board->turn][piece_index];
		// printf("piece_index: %d\n", piece_index); /* for debugging */
		// fflush(stdout); /* for debugging */
		if (current_piece->alive) {
			for (int step_index = 0; (step_index < current_piece->amount_steps) && (alpha < beta); step_index++){
				Step *step = current_piece->steps[step_index];
				set_step(board, step->srow, step->scol, step->drow, step->dcol);
				int step_value = MiniMaxAlgo(board, alpha, beta, search_depth-1, 0, eval_perspective);
				if ((step_value > alpha) || first_option) { /* we have found a better move or it's the first move to be considered */
					first_option = false;
					alpha = step_value;
					Step *good_step = ArrayListGetFirst(board->history); /* we recover the step the game-history */
					best_move.srow = good_step->srow;
					best_move.scol = good_step->scol;
					best_move.drow = good_step->drow;
					best_move.dcol = good_step->dcol;
				}
				undo_step(board);
			}
		}
		piece_index++;
	}
	return best_move;
}

/*
 * node-type: either 1 (max-node), or 0 (min-node)
 */
int MiniMaxAlgo(Gameboard *board, int alpha, int beta, int search_depth, int node_type, int eval_perspective) {
	int game_over = is_game_over(board);
	if (game_over == 1 || game_over == 0) { /* it's a checkmate */
		int result = node_type ? INT_MIN : INT_MAX;
		return result;
	} else if (game_over == 2 || search_depth == 0) { /* it's a tie or a leaf in the mini-max tree */
		return eval(board, eval_perspective);
	} else { /* the game is still on */
		int piece_index = 0;
		while ((alpha < beta) && (piece_index < 16)) { /* each player has 16 pieces */
			Piece *current_piece = board->all_pieces[board->turn][piece_index];
			if (current_piece->alive) {
				for (int step_index = 0; (step_index < current_piece->amount_steps) && (alpha < beta); step_index++){
					Step *step = current_piece->steps[step_index];
					set_step(board, step->srow, step->scol, step->drow, step->dcol);
					int step_value = MiniMaxAlgo(board, alpha, beta, search_depth-1, 1-node_type, eval_perspective);
					prune(&alpha, &beta, step_value, node_type);
					undo_step(board);
				}
			}
			piece_index++;
		}
		int result = node_type ? alpha : beta;
		return result;
	}
}
