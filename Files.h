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
#define file_path "C:\\Users\\try.txt"
#define MAX_LEN_ROW 200
#define MAX_TAG_LEN 40
#define MAX_DATA_LENGTH 8

//
Gameboard *load_game(FILE* f);
void save_xml(FILE *f);
bool is_str1_begins_with_str2(const char* str1, const char* str2);
#endif /* FILES_H_ */
