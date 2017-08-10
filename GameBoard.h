/*
 * Board.h
 *
 *  Created on: 9 баев„ 2017
 *      Author: User
 */

#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_
#include <string.h>
#include <stdbool.h>
#include "Pieces.h"
#include "ArrayList.h"

typedef struct board_t {
	Piece *board[8][8];
	int turn;
	Piece *all_pieces[2][16];
	ArrayList history;
} GameBoard;

typedef struct move_t {
	int sr;
	int sc;
	int dr;
	int dc;
	Piece *prevPiece;
} Move;

typedef enum chess_board_message_t {
	CHESS_BOARD_INVALID_MOVE,
	CHESS_BOARD_INVALID_ARGUMENT,
	CHESS_BOARD_NO_HISTORY,
	CHESS_BOARD_SUCCESS,
} CHESS_BOARD_MESSAGE;

#endif /* GAMEBOARD_H_ */
