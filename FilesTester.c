/*
 * FilesTester.c
 *
 *  Created on: Aug 16, 2017
 *      Author: sapir
 */

#include "FilesTester.h"
void check_Files(){
	Gameboard *game = create_board(0,0,0);

	set_step(game, 1,1,2,1);
	set_step(game, 6,1,5,1);
	set_step(game, 0,1,2,2);

	set_step(game, 6,3,4,3);
    set_step(game, 2,2,4,3);
	set_step(game, 7,3,4,3);
	set_step(game, 1,0,2,0);
	set_step(game,7,4,7,3);
	set_step(game,0,2,1,1);
	set_step(game,7,3,7,4);
	set_step(game,1,4,3,4);
	set_step(game,7,4,7,3);
	set_step(game,0,3,0,1);
	set_step(game,7,3,7,4);


	print_details_game(game);
	//write:
	FILE *fw = fopen(my_path, "w");
	save_xml(fw,game);
	fclose(fw);
	destroy_board(game);
	//read:
	Gameboard * game2 = NULL;
	FILE *fr = fopen(my_path, "r");

	game2 = load_game(fr);
	fclose(fr);
	print_details_game(game2);

}
