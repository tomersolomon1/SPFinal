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

#define PAWN_VALUE 1
#define KNIGHT_VALUE 3
#define BISHOP_VALUE 3
#define ROOK_VALUE 5
#define QUEEN_VALUE 9
#define KING_VALUE 100

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
	int value;				//value of the step
	Piece_type promote_to;  //in case of promotion: promote piece type
} StepValue;


/* sum-up all the pieces of the specified color */
int sumup_pieces(Gameboard *board, int color);

/* scoring function for the mini-max algorithm
 * eval_pespective is the color of the root of the mini-max tree */
int eval(Gameboard *board, int eval_perspective);

/* return true if found a better option, and update alpha or beta if needed */
bool update_ab(int *alpha, int *beta, int step_value, NodeType node_type, bool first_move);

Move find_best_move(Gameboard *board, int search_depth); /* old version of the min-max algo*/

/* implementation of the MiniMax algorithm, using alpha-beta pruning
 * assuming we can alter the board as we will, and that the game is not over
 * node-types:
 * 		0 - min-node
 * 		1 - max-node */
StepValue *MiniMaxAlgo(Gameboard *board, int alpha, int beta, int search_depth, NodeType node_type, int eval_perspective, bool first_option);

/* we assume the game is not over
 * a wrapper function for the mini-max algo */
StepValue *find_best_step(Gameboard *board, int search_depth);

/* destroys a StepValue */
void destroy_step_value(StepValue *best_move);

#endif /* MINIMAX_H_ */

