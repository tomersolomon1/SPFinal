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
	fprintf(f, "<current_turn>%d</current_turn>\n", game->turn);
	fprintf(f, "<game_mode>%d</game_mode>\n", game->game_mode);
	fprintf(f, "<difficulty>%d</difficulty>\n", game->difficulty);
	fprintf(f, "<user_color>%d</user_color>\n", game->user_color);
	fprintf(f, "<board>\n");
	for(int i = (BOARD_SIZE - 1); i >= 0; i--){
		fprintf(f, "<row_%d>", i + 1);
		for(int j = 0; j < BOARD_SIZE; j++){
			fprintf(f, "%c", game->board[i][j]->sign);
		}
		fprintf(f, "</row_%d>\n", i + 1);
	}
	fprintf(f, "</board>\n");
	fprintf(f, "</game>\n");
	fprintf(f, "<general>\n");
	//data for castling:
	fprintf(f, "<castling_data>\n");
	fprintf(f, "<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", white, 15, game->all_pieces[white][15]->has_moved, white, 15); //15 is the place of king in all_pieces
	fprintf(f, "<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", white, 12, game->all_pieces[white][12]->has_moved, white, 12); //12 is the place of 1st rook in all_pieces
	fprintf(f, "<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", white, 13, game->all_pieces[white][13]->has_moved, white, 13); //13 is the place of 2nd rook in all_pieces
	fprintf(f, "<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", black, 15, game->all_pieces[black][15]->has_moved, black, 15); //15 is the place of king in all_pieces
	fprintf(f, "<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", black, 12, game->all_pieces[black][12]->has_moved, black, 12); //12 is the place of 1st rook in all_pieces
	fprintf(f, "<piece_%d_%d_moved>%d</piece_%d_%d_moved>\n", black, 13, game->all_pieces[black][13]->has_moved, black, 13); //13 is the place of 2nd rook in all_pieces
	fprintf(f, "</castling_data>\n");
	fprintf(f, "</general>");
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
	char data[MAX_DATA_LENGTH];
	int data_int;
	int row_number;
	int piece_number;
	int piece_color;
	bool was_castling_data = false;
	while(fgets(line, MAX_LEN_ROW, f) != NULL){ //go over lines
		if(sscanf(line, "%*[^<]<current_turn>%d", &data_int) == 1 || sscanf(line, "<current_turn>%d", &data_int) == 1)
			game->turn = data_int;
		else if(sscanf(line, "%*[^<]<game_mode>%d", &data_int) == 1 || sscanf(line, "<game_mode>%d", &data_int) == 1)
			game->game_mode = data_int;
		else if(sscanf(line, "%*[^<]<difficulty>%d", &data_int) == 1 || sscanf(line, "<difficulty>%d", &data_int) == 1)
			change_game_difficulty(game, data_int);
		else if(sscanf(line, "%*[^<]<user_color>%d", &data_int) == 1 || sscanf(line, "<user_color>%d", &data_int) == 1)
			game->user_color = data_int;
		else if(sscanf(line, "%*[^<]<row_%d>%8[^<]", &row_number, data) == 2 || sscanf(line, "<row_%d>%8[^<]", &row_number, data) == 2)
			set_row(game, row_number - 1, data);
		else if(sscanf(line,"%*[^<]<piece_%d_%d_moved>%d", &piece_color, &piece_number, &data_int) == 3 || sscanf(line,"<piece_%d_%d_moved>%d", &piece_color, &piece_number, &data_int) == 3){
			was_castling_data = true;
			game->all_pieces[piece_color][piece_number]->has_moved = data_int;
		}
	}
	if(!was_castling_data){ //assume that if the king and the rooks are in their first position, then they were'nt moved
		set_has_moved_by_position(game);
	}
	set_all_valid_steps(game);
	return game;
}

void set_has_moved_by_position(Gameboard* game){
	//kings:
	if(game->board[7][4]->type == King && game->board[7][4]->colur == black) //7,4 is the init position of black king
		game->board[7][4]->has_moved = false;
	if(game->board[0][4]->type == King && game->board[0][4]->colur == white) //0,4 is the init position of white king
		game->board[0][4]->has_moved = false;
	//black rooks:
	if(game->board[7][0]->type == Rook && game->board[7][0]->colur == black) //7,0 is the init position of black rook
		game->board[7][0]->has_moved = false;
	if(game->board[7][7]->type == Rook && game->board[7][7]->colur == black) //7,7 is the init position of black rook
		game->board[7][7]->has_moved = false;
	//white rooks:
	if(game->board[0][0]->type == Rook && game->board[0][0]->colur == white) //0,0 is the init position of white rook
		game->board[0][0]->has_moved = false;
	if(game->board[0][7]->type == Rook && game->board[0][7]->colur == white) //0,7 is the init position of white rook
		game->board[0][7]->has_moved = false;
}

void set_position_of_piece(Gameboard *game, Piece *p, int row, int col){
	p->alive = true;
	p->row = row;
	p->col = col;
	game->board[row][col] = p;
	if(p->type == Pawn){ //set vector size of pawn by its place on board:
		if(p->colur == white)
			p->vectors[0]->vector_size = (row == 1? 2: 1);
		else
			p->vectors[0]->vector_size = (row == 6? 2: 1);
	}
}

void set_row(Gameboard* game, int row_number, char* str){
	char sign;
	int color;
	Piece *p;
	bool piece_setted;
	for(int col = 0; col < BOARD_SIZE; col++){
		sign = str[col];
		if(sign == SIGN_EMPTY)
			continue;
		piece_setted = false;
		color = (('a' <= sign && sign <= 'z') ? white: black);
		for(int j = 0; j < AMOUNT_PIECES_PER_COLOR; j++){
			p = game->all_pieces[color][j];
			if(p->sign == sign && !p->alive){
				piece_setted = true;
				set_position_of_piece(game, p, row_number, col);
				break;
			}
		}
		if(!piece_setted){ //there was promotion so there could be more pieces of the same type
			for(int j = 0; j < AMOUNT_PIECES_PER_COLOR; j++){
				p = game->all_pieces[color][j];
				if(p->type == Pawn && !p->alive){
					change_piece_type(p, get_piece_type_by_sign(sign));
					set_position_of_piece(game, p, row_number, col);
					break;
				}
			}
		}
	}
}



