/*
 * ConsoleMode.c
 *
 *  Created on: 16 баев„ 2017
 *      Author: User
 */

#include <stdbool.h>
#include <string.h>
#include "Parser.h"
#include "DataDefinitions.h"

void console_mode() {
	int mode = 1; /* 1 is settings mode, 0 is game mode */
	bool keep_on = true;
	char *line = NULL;
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
	while (keep_on) {
		line = (char *) malloc(sizeof(char)*(SP_MAX_LINE_LENGTH+1)); /* tentative, until moav says what should we do */
		scanf("SP_MAX_LINE_LENGTH%s", line); /* safe reading from line? asked in the forum */
		Command comm = parser(line);
		if ()
	}
}
