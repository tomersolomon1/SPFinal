/*
 * Files.c
 *
 *  Created on: Aug 12, 2017
 *      Author: sapir
 */

#include "Files.h"


void void save_xml(FILE *f){
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


bool is_str1_begins_with_str2(const char* str1, const char* str2){
	if(strlen(str1) < strlen(str2)){
		return false;
	}
	int a = strncmp(str1, str2, strlen(str2));
	if(a == 0){
		return true;
	}
	return false;
}

Gameboard *load_game(FILE* f){
	Gameboard *game = create_board(0, 0, 0);
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 16; j++){
			game->all_pieces[i][j]->alive = false;
			game->all_pieces[i][j]->has_moved = true;
		}
	}
	FILE *f = NULL;
	char line[MAX_LEN_ROW];
	char tag[MAX_TAG_LEN];
	char data[MAX_DATA_LENGTH];
	int data_int;
	while(fgets(line, MAX_LEN_ROW, f) != NULL){
		if(sscanf(line, "<%s>%d</%s>\n", tag, data_int) == 2){
			if(is_str1_begins_with_str2(line, "current_turn")){
				game->turn = data_int;
			}
			if(is_str1_begins_with_str2(line, "game_mode")){
				game->game_mode = data_int;
			}
			if(is_str1_begins_with_str2(line, "difficulty")){
				game->difficulty = data_int;
			}
			if(is_str1_begins_with_str2(line, "user_color")){
				game->user_color = data_int;
			}
		}
		else if(sscanf(line, "<%s>%s</%s>\n", tag, data) == 2){
			if(is_str1_begins_with_str2(line, "row")){

			}
		}
	}

	fclose(f);
	return NULL;
}
