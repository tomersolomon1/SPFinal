/*
 * Board.h
 *
 *  Created on: 9 баев„ 2017
 *      Author: User
 */

#ifndef BOARD_H_
#define BOARD_H_

typedef struct board_t {
	Piece *board[8][8];
	int turn;
	Piece *pieces[2][16];
} Board;

typedef struct move_t {
	int sr;
	int sc;
	int dr;
	int dc;
	Piece *prepre;
} Move;

typedef enum sp_chess_board_message_t {
	SP_CHESS_BOARD_INVALID_MOVE,
	SP_CHESS_BOARD_INVALID_ARGUMENT,
	SP_CHESS_BOARD_NO_HISTORY,
	SP_CHESS_BOARD_SUCCESS,
} SP_CHESS_BOARD_MESSAGE;

#endif /* BOARD_H_ */
