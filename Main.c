
#include "ConsoleMode.h"
#include "graphics/GuiManager.h"
#include "GameBoard.h"


int main(int argc, char *argv[]){
	//the game will be destroyed inside manage_console and run_gui
	Gameboard *gameboard = create_board(GAME_DEFAULT_MODE, GAME_DEFAULT_DIFFICULTY, GAME_DEFAULT_COLOR);

	if(argc == 1) //The user didn't specify the game mode, so we run it in console mode by default
		manage_console(gameboard);
	else if(argv[1][0] == '-' && argv[1][1] == 'g') //The user used a flag -g (=gui)
		run_gui(gameboard);
	else if(argv[1][0] == '-' && argv[1][1] == 'c') //The user used a flag -c (=console)
		manage_console(gameboard);
	else
		destroy_board(gameboard); //Something with the flags is wrong
	return 0;
}

