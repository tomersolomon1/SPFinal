
#include "ConsoleMode.h"
#include "graphics/GuiManager.h"

int main(int argc, char *argv[]){
	if(argc == 1) //The user didn't specify the game mode (gui or console)
		run_console();
	else if(strlen(argv[1] > 1) && argv[1][0] == '-'){ //The user used a flag
		if(argv[1][1] == 'g') //
			run_gui();
		else if(argv[1][1] == 'c')
			run_console();
	}
	return 0;
}

