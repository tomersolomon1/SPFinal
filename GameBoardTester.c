/*
 * GameBoardTester.c
 *
 *  Created on: 12 баев„ 2017
 *      Author: User
 */

#include "GameBoard.h"
#include "Files.h"

void check_GameBoard() {
	Gameboard * game = create_board(1, -1, black);

	set_step(game, 1,1,2,1);
	set_step(game, 7,1,5,0);

	set_step(game, 0,2,2,0);
	set_step(game, 5,0,7,1);

	set_step(game, 0,1,2,2);
	set_step(game, 7,1,5,0);

	set_step(game, 1,4,3,4);
	set_step(game, 5,0,7,1);

	set_step(game, 0,3,3,6);
	set_step(game, 7,1,5,0);

	set_step(game, 0,4,0,2);
	print_details_game(game);

	undo_step(game);



	print_details_game(game);


	//upload_Xml(game);
}
