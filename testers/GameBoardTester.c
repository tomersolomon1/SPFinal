/*
 * GameBoardTester.c
 *
 *  Created on: 12 баев„ 2017
 *      Author: User
 */

#include "../GameBoard.h"
#include "../Files.h"


/********************************** FOR DEBUGGING ******************************
void write_to_log_file(const char *event){
	FILE *f_log = fopen("log.txt","a");
	fprintf(f_log, event);
	fclose(f_log);
	//printf(event); fflush(stdout);
}

void print_board_to_log_file(Gameboard *gameboard) {
	char* a = malloc(2);
	a[1] = '\0';
	write_to_log_file("===========================\n");
	for(int i = (BOARD_SIZE - 1); i >= 0; i--){
		for(int j = 0; j < BOARD_SIZE; j++){
			a[0] =gameboard->board[i][j]->sign;
			write_to_log_file(a);
		}
		write_to_log_file("\n");
	}
	free(a);
}
*******************************/


void check_GameBoard() {
//	FILE *f = fopen("C:\\Users\\sapir\\Desktop\\save0.xml", "r");
//	assert(f != NULL);
//	Gameboard * game = load_game(f);
//	fclose(f);
//	print_details_game(game);
//hazraha:
/*	set_step(game, 1,1,2,1);
	set_step(game, 7,1,5,0);


	set_step(game, 0,2,2,0);
	set_step(game, 5,0,7,1);

	set_step(game, 0,1,2,2);
	set_step(game, 7,1,5,2);

	set_step(game, 1,4,3,4);
	set_step(game, 5,2,3,3);

	set_step(game, 0,3,3,6);
	set_step(game, 3,3,1,4);
*/
//check:

	Gameboard *game = create_board(1,1,1);
	print_details_game(game);
	set_step(game, 1,3,2,3,false);
	print_details_game(game);
	set_step(game, 6,2,5,2,false);
	print_details_game(game);
	set_step(game, 1,7,2,7,false);
	print_details_game(game);
	set_step(game,7,3,4,0,false);
	print_details_game(game);
//	print_details_game(game);
}
