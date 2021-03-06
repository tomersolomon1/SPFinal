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



/* save game in xml format to *f */
void save_xml(FILE *f, Gameboard* game);

/* load game from *f
 * we assume that f is a valid game-saved file
 * NOTICE:
 * we assume that if the file doesn't contain castling data,
 * then Kings and Rooks in their initial positions on board
 * are defined to be not-moved by default
 * (so castling might be valid during the game)*/
Gameboard *load_game(FILE* f);

/* helping function for load_game
 * set true or false to piece->has_moved by their first position
 * for the pieces involved in castling:
 * we assume that if there isn't castling information in the xml,
 * then the kings and rooks who are in their beginning positions were not moved
 * so castling is valid */
void set_has_moved_for_castling_by_init_position(Gameboard* game);

/* helping function for load_game
 * set the row by data from the xml data*/
void set_row(Gameboard* game, int row_number, char* str);

/* helping function for set_row
 * set piece p to be in position [row,col] on game*/
void set_position_of_piece(Gameboard *game, Piece *p, int row, int col);

#endif /* FILES_H_ */
