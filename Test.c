/*
 * Test.c
 *
 *  Created on: Aug 11, 2017
 *      Author: sapir
 */

#include "GameBoard.h"

int main(){
	Gameboard * game = create_board();
	print_board(game);
	set_step(game, 1,1,2,1);
	print_board(game);
	set_step(game, 6,1,5,1);
	print_board(game);
	set_step(game, 0,1,2,2);
	print_board(game);
	set_step(game, 6,2,4,2);
	print_board(game);
}
