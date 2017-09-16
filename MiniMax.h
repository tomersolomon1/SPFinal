/*
 * MiniMax.h
 *
 *  Created on: 14 баев„ 2017
 *      Author: User
 */


#ifndef MINIMAX_H_
#define MINIMAX_H_

#include "GameBasicBuildingBlocks.h"
#include "GameBoard.h"

typedef enum {
	MinNode,
	MaxNode
} NodeType;

typedef struct Move {
	int srow; // source row
	int scol; // source col
	int drow; // dest row
	int dcol; // dest col
} Move;

typedef struct step_value {
	Step *step;
	int value;
	Piece_type promote_to;
} StepValue;

int sumup_pieces(Gameboard *board, int color);
int eval(Gameboard *board, int eval_perspective);
void prune(int *alpha, int *beta, int step_value, int node_type);
Move find_best_move(Gameboard *board, int search_depth); /* old version of the min-max algo*/
StepValue *MiniMaxAlgo(Gameboard *board, int alpha, int beta, int search_depth,
NodeType node_type, int eval_perspective, bool first_option);
StepValue *find_best_step(Gameboard *board, int search_depth); /* new version of the min-max algo*/
void destroy_step_value(StepValue *best_move);

#endif /* MINIMAX_H_ */

