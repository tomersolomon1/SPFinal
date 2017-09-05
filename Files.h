/*
 * Files.h
 *
 *  Created on: Aug 12, 2017
 *      Author: sapir
 */

#ifndef FILES_H_
#define FILES_H_

#include <stdio.h>
#include "GameBoard.h"
#define MAX_LEN_ROW 50
#define MAX_DATA_LENGTH 8

/*load game from *f */
Gameboard *load_game(FILE* f);

/*save game in xml format to *f */
void save_xml(FILE *f, Gameboard* game);

/*helping function for load_game - set the row by data from the xml data*/
void set_row(Gameboard* game, int row_number, char* str);

#endif /* FILES_H_ */
