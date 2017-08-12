/*
 * Test.c
 *
 *  Created on: Aug 11, 2017
 *      Author: sapir
 */

#include "GameBoard.h"

int main(){
	Gameboard * game = create_board();
	print_details_game(game);
	set_step(game, 1,1,2,1);
	print_details_game(game);
	set_step(game, 6,1,5,1);
	print_details_game(game);
	set_step(game, 0,1,2,2);
	print_details_game(game);
	set_step(game, 6,2,4,2);
	print_details_game(game);

	undo_step(game);
	print_details_game(game);
	set_step(game, 6,3,4,3);
	print_details_game(game);
	set_step(game, 2,2,4,3);
	print_details_game(game);
	set_step(game, 7,3,4,3);
	print_details_game(game);
	set_step(game, 1,0,2,0);
	print_details_game(game);
	set_step(game, 4,3,1,3);
	print_details_game(game);
	set_step(game, 0,5,1,3);
	print_details_game(game);
}
