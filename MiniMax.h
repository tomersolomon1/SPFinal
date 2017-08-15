/*
 * MiniMax.h
 *
 *  Created on: 14 баев„ 2017
 *      Author: User
 */

#include "Pieces.h"

#ifndef MINIMAX_H_
#define MINIMAX_H_

typedef struct Move {
	int srow; // source row
	int scol; // source col
	int drow; // dest row
	int dcol; // dest col
} Move;

int sumup_pieces(Gameboard *board, int color);
int eval(Gameboard *board, int eval_perspective);
void prune(int *alpha, int *beta, int step_value, int node_type);
Move find_best_move(Gameboard *board, int search_depth);
int MiniMaxAlgo(Gameboard *board, int alpha, int beta, int search_depth, int node_type, int eval_perspective);



#endif /* MINIMAX_H_ */

