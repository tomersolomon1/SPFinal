/*
 * Files.c
 *
 *  Created on: Aug 12, 2017
 *      Author: sapir
 */

#include "Files.h"
#define file_path "C:\\Users\\try.txt"

void upload_Xml(Gameboard* game){
	FILE *file = NULL;
	file = fopen(file_path, "w");
	assert(file != NULL);
	fprintf(file, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(file, "<game>\n");
	fprintf(file, "\t<current_turn>%d</current_turn>\n", game->turn);
	fprintf(file, "\t<game_mode>%d</game_mode>\n", game->game_mode);
	fprintf(file, "\t<user_color>%d</user_color>\n", game->user_color);
	fprintf(file, "\t<board>\n");
	for(int i = 7; i >= 0; i--){
		fprintf(file, "\t\t<row_%d>", i + 1);
		for(int j = 0; j < 8; j++){
			fprintf(file, "%c", game->board[i][j]->sign);
		}
		fprintf(file, "</row_%d>\n", i + 1);
	}
	fprintf(file, "\t</board>\n");
	fprintf(file, "</game>");
	fclose(file);
}
