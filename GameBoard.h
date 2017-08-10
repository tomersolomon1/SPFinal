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
#define black 0
#define white 1

typedef struct gameboard_t {
	Piece *board[8][8];
	int turn;
	Piece *all_pieces[2][16];
	ArrayList history;
} Gameboard;

typedef struct step_t {
	int srow;
	int scol;
	int drow;
	int dcol;
	Piece *prevPiece;
} Step;

typedef enum chess_board_message_t {
	CHESS_BOARD_INVALID_MOVE,
	CHESS_BOARD_INVALID_ARGUMENT,
	CHESS_BOARD_NO_HISTORY,
	CHESS_BOARD_SUCCESS,
} CHESS_BOARD_MESSAGE;

#endif /* GAMEBOARD_H_ */
