/*
 * MiniMax.c
 *
 *  Created on: 14 баев„ 2017
 *      Author: User
 */

#include <limits.h>
#include "MiniMax.h"
#include "GameBoard.h"

void destroy_step_value(StepValue *sv) {
	if (sv != NULL) {
		destroy_step(sv->step);
		free(sv);
	}
}

int sumup_pieces(Gameboard *board, int color) {
	int pieces_value[] = {PAWN_VALUE, KNIGHT_VALUE, BISHOP_VALUE, ROOK_VALUE, QUEEN_VALUE, KING_VALUE};
	int sumup = 0;
	for (int i = 0; i < AMOUNT_PIECES_PER_COLOR; i++) {
		if (board->all_pieces[color][i]->alive) {
			sumup += pieces_value[board->all_pieces[color][i]->type];
		}
	}
	return sumup;
}

int eval(Gameboard *board, int eval_perspective) {
	int white_pieces = sumup_pieces(board, 1);
	int black_pieces = sumup_pieces(board, 0);
	int result = eval_perspective ? white_pieces - black_pieces : black_pieces - white_pieces;
	return result;
}

bool update_ab(int *alpha, int *beta, int step_value, NodeType node_type, bool first_move) {
	if (node_type == MinNode && (*beta > step_value || first_move)) { /* it's a min-node */
		*beta = step_value;
		return true; /* found a better option */
	} else if (node_type == MaxNode && (*alpha < step_value || first_move)) { /* it's a max-node */
		*alpha = step_value;
		return true; /* found a better option */
	}
	else {
		return false;
	}
}

StepValue *init_step_value() {
	StepValue *sv = (StepValue *) malloc(sizeof(StepValue));
	assert(sv != NULL);
	sv->step = NULL; /* default */
	sv->promote_to = Empty; /* default */
	return sv;
}

StepValue *MiniMaxAlgo(Gameboard *board, int alpha, int beta, int search_depth,
		NodeType node_type, int eval_perspective, bool first_option) {
	StepValue *best_sv = init_step_value();
	int game_over = is_game_over_minimax(board);
	if (game_over == 1 || game_over == 0) { /* it's a checkmate */
		best_sv->value = node_type ? INT_MIN : INT_MAX;
		return best_sv;
	} else if (game_over == 2 || search_depth == 0) { /* it's a tie or a leaf in the mini-max tree */
		best_sv->value = eval(board, eval_perspective);
		return best_sv;
	} else { /* the game is still on */
		int piece_index = 0;
		while ((alpha < beta) && (piece_index < AMOUNT_PIECES_PER_COLOR)) { /* each player has 16 pieces, some of them might not be alive */
			Piece *current_piece = board->all_pieces[board->turn][piece_index];
			if (current_piece->alive) {
				int amount_steps = 0;
				Step **valid_steps = get_all_valid_steps_of_piece_minimax(board, current_piece, &amount_steps);
				int promote_to = Empty; /* default */
				for (int step_index = 0, promotion_option = 0; (step_index < amount_steps) && (alpha < beta); step_index++){
					Step *step = valid_steps[step_index];
					CHESS_BOARD_MESSAGE msg = commit_move(board, step->srow, step->scol,
							step->drow, step->dcol, true, promotion_option);
					if (step->src_previous_state == Was_promoted) { /* a pawn can be promoted to only 5 different pieces */
						promote_to = promotion_option;
						if (promotion_option < 4) {
							promotion_option += 1;
							step_index -= 1; /* we are stalling the enumeration of the steps, so we could check all promotion possibilities */
						} else {
							promotion_option = 0; /* setting promotion_option to be zero again */
						}
					}
					StepValue *sv = MiniMaxAlgo(board, alpha, beta, search_depth-1, 1-node_type, eval_perspective, first_option);
					if (update_ab(&alpha, &beta, sv->value, node_type, first_option)) {
						destroy_step(best_sv->step);
						best_sv->step = copy_step(step);
						if (msg == CHESS_BOARD_PROMOTION) { /* it was a promotion move */
							best_sv->promote_to = promote_to; /* saving the piece-type we are promoting to */
						} else {
							best_sv->promote_to = Empty; /* the new best-move wasn't a promotion */
						}
					}
					first_option = false;
					destroy_step_value(sv);
					undo_step(board, true);
				}
				free_all_valid_steps_minimax(valid_steps, current_piece->type); /* free all steps */
			}
			piece_index++;
		}
		best_sv->value = node_type ? alpha : beta;
		return best_sv;
	}
}

StepValue *find_best_step(Gameboard *board, int search_depth) {
	Gameboard *copy = copy_board(board);
	int alpha = INT_MIN;
	int beta  = INT_MAX;
	int eval_perspective = board->turn;
	StepValue *best_sv = MiniMaxAlgo(copy, alpha, beta, search_depth, MaxNode, eval_perspective, true);
	destroy_board(copy);
	return best_sv;
}
