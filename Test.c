/*
 * Test.c
 *
 *  Created on: Aug 11, 2017
 *      Author: sapir
 */

#include <string.h>
#include "GameBoardTester.h"
#include "ParserTester.h"
#include "FilesTester.h"
#include "ConsoleTester.h"
#include "graphics/Game_Window_Tester.h"
#include "graphics/GuiManager.h"

//FILE *f_log;
int main(int argc, char *argv[]){
	if (argc == 2) {
		if (strcmp(argv[1], "0") == 0) { /* console test  */
			console_tester();
		} else if (strcmp(argv[1], "1") == 0) { /* Sapir's tests */
//			console_tester();
			run_gui();
		} else if (strcmp(argv[1], "g") == 0) { /* game window (gui) test  */
			check_game_window();
		} else if (strcmp(argv[1], "p") == 0) { /* parser test */
			check_parser();
		}
	}
	return 0;
}
