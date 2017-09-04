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
	fprintf(f, "\t<difficulty>%d</difficulty>\n", game->difficulty);
	fprintf(f, "\t<user_color>%d</user_color>\n", game->user_color);
	fprintf(f, "\t<board>\n");
	for(int i = (BOARD_SIZE - 1); i >= 0; i--){
		fprintf(f, "\t\t<row_%d>", i + 1);
		for(int j = 0; j < BOARD_SIZE; j++){
			fprintf(f, "%c", game->board[i][j]->sign);
		}
		fprintf(f, "</row_%d>\n", i + 1);
	}
	fprintf(f, "\t</board>\n");
	fprintf(f, "</game>\n");
	fprintf(f,"<general>\n");
	//data for castling:
	fprintf(f, "\t<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", white, 15, game->all_pieces[white][15]->has_moved, white, 15); //15 is the place of king in all_pieces
	fprintf(f, "\t<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", white, 12, game->all_pieces[white][12]->has_moved, white, 12); //12 is the place of 1st rock in all_pieces
	fprintf(f, "\t<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", white, 13, game->all_pieces[white][13]->has_moved, white, 13); //13 is the place of 2nd rock in all_pieces
	fprintf(f, "\t<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", black, 15, game->all_pieces[black][15]->has_moved, black, 15); //15 is the place of king in all_pieces
	fprintf(f, "\t<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", black, 12, game->all_pieces[black][12]->has_moved, black, 12); //12 is the place of 1st rock in all_pieces
	fprintf(f, "\t<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", black, 13, game->all_pieces[black][13]->has_moved, black, 13); //13 is the place of 2nd rock in all_pieces
	fprintf(f, "</general>");
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
	for(int i = 0; i < 2; i++){ //"clean" game
		for(int j = 0; j < AMOUNT_PIECES_PER_COLOR; j++){
			game->all_pieces[i][j]->alive = false;
			game->all_pieces[i][j]->has_moved = true;
		}
	}
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0 ; j < BOARD_SIZE; j ++)
			game->board[i][j] = game->empty;
	}
	char line[MAX_LEN_ROW];
	char tag[MAX_TAG_LEN];
	char data[MAX_DATA_LENGTH];
	int data_int;
	int row_number;
	int piece_number;
	int piece_color;
	while(fgets(line, MAX_LEN_ROW, f) != NULL){ //go over lines
		if(sscanf(line, "%*[ \t]<%13[^>]>%d", tag, &data_int) == 2){
			if(is_str1_begins_with_str2(tag, "current_turn"))
				game->turn = data_int;
			else if(is_str1_begins_with_str2(tag, "game_mode"))
				game->game_mode = data_int;
			else if(is_str1_begins_with_str2(tag, "difficulty"))
				game->difficulty = data_int;
			else if(is_str1_begins_with_str2(tag, "user_color"))
				game->user_color = data_int;
		}
		else if(sscanf(line, "%*[ \t]<%13[^_]_%d>%8[^<]", tag, &row_number, data) == 3){
			if(is_str1_begins_with_str2(tag, "row"))
				set_row(game, row_number - 1, data);
		}
		else if(sscanf(line,"%*[ \t]<piece_%d_%d_moved>%d", &piece_color, &piece_number, &data_int) == 3)
			game->all_pieces[piece_color][piece_number]->has_moved = data_int;
	}
	set_all_valid_steps(game);
	return game;
}

void set_row(Gameboard* game, int row_number, char* str){
	char sign;
	int color;
	Piece *p;
	for(int col = 0; col < BOARD_SIZE; col++){
		sign = str[col];
		if(sign == SIGN_EMPTY)
			continue;
		color = (('a' <= sign && sign <= 'z') ? white: black);
		for(int j = 0; j < AMOUNT_PIECES_PER_COLOR; j++){
			p = game->all_pieces[color][j];
			if(p->sign == sign && !p->alive){
				p->alive = true;
				p->row = row_number;
				p->col = col;
				game->board[row_number][col] = p;
				if(p->type == Pawn){ //set vector size of pawn by its place on board:
					if(color == white)
						p->vectors[0]->vector_size = (row_number == 1? 2: 1);
					else
						p->vectors[0]->vector_size = (row_number == 6? 2: 1);
				}
				break;
			}
		}
	}
}
