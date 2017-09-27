/*
 * FilesTester.c
 *
 *  Created on: Aug 16, 2017
 *      Author: sapir
 */

#include "FilesTester.h"
void check_Files(){
	Gameboard *game = create_board(0,0,0);

	set_step(game, 1,1,2,1,false);
	set_step(game, 6,1,5,1,false);
	set_step(game, 0,1,2,2,false);

	set_step(game, 6,3,4,3,false);
    set_step(game, 2,2,4,3,false);
	set_step(game, 7,3,4,3,false);
	set_step(game, 1,0,2,0,false);
	set_step(game,7,4,7,3,false);
	set_step(game,0,2,1,1,false);
	set_step(game,7,3,7,4,false);
	set_step(game,1,4,3,4,false);
	set_step(game,7,4,7,3,false);
	set_step(game,0,3,0,1,false);
	set_step(game,7,3,7,4,false);


	print_details_game(game);
	//write:
	FILE *fw = fopen("", "w");
	save_xml(fw,game);
	fclose(fw);
	destroy_board(game);
	//read:
	Gameboard * game2 = NULL;
	FILE *fr = fopen("", "r");

	game2 = load_game(fr);
	fclose(fr);
	print_details_game(game2);

}
