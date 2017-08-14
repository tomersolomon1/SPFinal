/*
 * MiniMax.c
 *
 *  Created on: 14 баев„ 2017
 *      Author: User
 */

#include <limits.h>
#include "GameBoard.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

Step find_best_move(Gameboard *board) {
	Step step;
	return step;
}

void prune(int *alpha, int *beta, int step_value, int node_type) {
	if (node_type == 0) { /* it's a min-node */
		MIN(*beta, step_value);
	} else {
		MAX(*beta, step_value);
	}
}

/*
 * node-type: either 1 (max-node), or 0 (min-node)
 */
int MiniMaxAlgo(Gameboard *board, int alpha, int beta, int search_depth, int node_type) {
	int game_over = is_game_over(board);
	if (game_over == 1 || game_over == 0) { /* it's a checkmate */
		int result = node_type ? INT_MIN : INT_MAX;
		return result;
	} else if (game_over == 2 || search_depth == 0) { /* it's a tie or a leaf in the mini-max tree */
		return eval(board, node_type);
	} else { /* the game is still on */
		int piece_index = 0;
		while ((alpha < beta) && (piece_index < 17)) { /* each player has 16 pieces */
			Piece *current_piece = board->all_pieces[board->turn][piece_index];
			if (current_piece->alive) {
				for (int step_index = 0; (step_index < current_piece->amount_steps) && (alpha < beta); step_index++){
					Step *step = current_piece->steps[step_index];
					set_step(board, step->srow, step->scol, step->drow, step->dcol);
					int step_value = MiniMaxAlgo(board, alpha, beta, search_depth-1, 1-node_type);
					prune(&alpha, &beta, step_value, node_type);
					undo_step(board);
				}
			}
		}
		int result = node_type ? alpha : beta;
		return result;
	}
}
