/*
 * Files.c
 *
 *  Created on: Aug 12, 2017
 *      Author: sapir
 */

#include "Files.h"


void save_xml(FILE *f, Gameboard* game){
	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<game>\n");
	fprintf(f, "\t<current_turn>%d</current_turn>\n", game->turn);
	fprintf(f, "\t<game_mode>%d</game_mode>\n", game->game_mode);
	fprintf(f, "\t<user_color>%d</user_color>\n", game->user_color);
	fprintf(f, "\t<board>\n");
	for(int i = 7; i >= 0; i--){
		fprintf(f, "\t\t<row_%d>", i + 1);
		for(int j = 0; j < 8; j++){
			fprintf(f, "%c", game->board[i][j]->sign);
		}
		fprintf(f, "</row_%d>\n", i + 1);
	}
	fprintf(f, "\t</board>\n");
	fprintf(f, "</game>");
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
	for(int i = 0; i < 8; i++){
		for(int j = 0 ; j < 8; j ++){
			game->board[i][j] = game->empty;
		}
	}
	char line[MAX_LEN_ROW];
	char tag[MAX_TAG_LEN];
	char data[MAX_DATA_LENGTH];
	int data_int[1];
	int row_number[1];
	while(fgets(line, MAX_LEN_ROW, f) != NULL){
		printf("%d",sscanf(line, "<%20s>%d</", tag, data_int));
		fflush(stdout);
		if(sscanf(line, "<%20s>%d</", tag, data_int) == 2){
			if(is_str1_begins_with_str2(line, "current_turn")){
				game->turn = data_int[0];
			}
			if(is_str1_begins_with_str2(line, "game_mode")){
				game->game_mode = data_int[0];
			}
			if(is_str1_begins_with_str2(line, "difficulty")){
				game->difficulty = data_int[0];
			}
			if(is_str1_begins_with_str2(line, "user_color")){
				game->user_color = data_int[0];
			}
		}
		else if(sscanf(line, "<%20s_%d>%20s</", tag, row_number, data) == 3){
			if(is_str1_begins_with_str2(line, "row")){
				set_row(game, row_number[0] - 1, data);
			}
		}
	}
	set_all_valid_steps(game);
	return game;
}

void set_row(Gameboard* game, int row_number, char* str){
	char sign;
	int color;
	Piece * p;
	for(int col = 0; col < 8; col++){
		sign = str[col];
		color = (('a' <= sign && sign <= 'z') ? white: black);
		for(int j = 0; j < 16; j++){
			p = game->all_pieces[color][j];
			if(p->sign == sign && !p->alive){
				p->alive = true;
				p->row = row_number;
				p->col = col;
				game->board[row_number][col] = p;
				if(p->type == Pawn){
					if(color == white){
						p->vectors[0]->vector_size = (row_number == 1? 2: 1);
					}
					else{
						p->vectors[0]->vector_size = (row_number == 6? 2: 1);
					}

				}
			}
		}
	}
}
