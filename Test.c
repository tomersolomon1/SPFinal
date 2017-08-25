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

int main(int argc, char *argv[]){
	if (argc == 2) {
		if (strcmp(argv[1], "0") == 0) { /* Tomer's tests  */
			//check_parser();
			console_tester();
		} else if (strcmp(argv[1], "1") == 0) { /* Sapir's tests */
			check_Files();
		}
	}
}
