/*
 * Files.c
 *
 *  Created on: Aug 12, 2017
 *      Author: sapir
 */

#include "Files.h"


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

char *get_data_by_tag(FILE *f, char *tag, int tag_len, int data_len){

	return NULL;
}

Gameboard *upload_game(char* path){
	int current_turn, game_mode, difficulty, user_color;
	FILE *f = NULL;
	char line[MAX_LEN_ROW];
	char tag[MAX_TAG_LEN];
	char data[MAX_DATA_LENGTH];
	int i;
	while(fgets(line, MAX_LEN_ROW, f) != NULL){
		if(sscanf(line, "<%s>%d</%s>\n", tag, i) == 2){
			if(strcmp(tag, "current_turn") ){

			}
		}
		else if(sscanf(line, "<%s>%s</%s>\n", tag, data) == 2){

		}
	}

	fclose(f);
	return NULL;
}
