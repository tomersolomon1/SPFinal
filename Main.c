
#include "ConsoleMode.h"
#include "graphics/GuiManager.h"
#include "GameBoard.h"

int main(int argc, char *argv[]){
	Gameboard *gameboard = create_board(1, 2, 1);
	if(argc == 1) //The user didn't specify the game mode (gui or console)
		manage_console(gameboard);
	else if(strlen(argv[1]) > 1 && argv[1][0] == '-'){ //The user used a flag
		if(argv[1][1] == 'g') //
			run_gui();
		else if(argv[1][1] == 'c')
			manage_console(gameboard);
	}
	destroy_board(gameboard);
	return 0;
}

