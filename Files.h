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
#define MAX_LEN_ROW 200
#define MAX_TAG_LEN 200
#define MAX_DATA_LENGTH 8

//load game from *f
Gameboard *load_game(FILE* f);

//save game in xml format to *f
void save_xml(FILE *f, Gameboard* game);

//helping function
bool is_str1_begins_with_str2(const char* str1, const char* str2);

//helping function for load_game - set the row by data in xml
void set_row(Gameboard* game, int row_number, char* str);

#endif /* FILES_H_ */
