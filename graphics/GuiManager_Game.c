/*
 * GuiManager_Game.c
 *
 *  Created on: 3 בספט׳ 2017
 *      Author: User
 */

#include "GameWindow.h"
#include "SPCommon.h"
#include "WindowDataStruct.h"
#include "GuiManager_Game.h"
#include "Windows.h"
#include "../ConsoleMode.h"
#include "../Files.h"

/*
 * assuming the move is legal
 * set the step, and show a SimpleMessageBox if the game is over
 * return:
 * 		true  - the game is over
 * 		false - the game isn't over
 */

bool graphical_handle_single_move(Window *window, int srow, int scol, int drow, int dcol) {
	Gameboard *board = window->data->board_widget->board;
	set_step(board, srow, scol, drow, dcol);
	//draw_board(window->data, window->windowRenderer, NULL); /* check the clear-renderer */
	drawWindow(window, NULL);
	int game_over = is_game_over(board);
	char mssg[50];
	if (game_over == 0 || game_over == 1 || game_over == 2) { /* the game is over */
		if (game_over == 2) { /* it's a tie */
			strcpy(mssg, "It's a tie!");
		} else { /* somebody won the game */
			sprintf(mssg, "Checkmate! %s player wins the game", colors[game_over]);
		}
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", mssg, NULL);
		return true;
	} else if (is_under_check(board)) {
		sprintf(mssg, "Check: %s King is threatend!", colors[board->turn]);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Check!", mssg, NULL);
	}
	return false;
}

bool graphical_handle_move(Window *window, int srow, int scol, int drow, int dcol) {
	bool game_over = graphical_handle_single_move(window, srow, scol, drow, dcol);
	if (game_over) {
		return true;
	} else if (window->data->board_widget->board->game_mode == 1) { /* the game is not over, and we need to play the computer's turn */
		Gameboard *copy = copy_board(window->data->board_widget->board);
		Move move = find_best_move(copy, copy->difficulty);
		destroy_board(copy);
		return graphical_handle_single_move(window, move.srow, move.scol, move.drow, move.dcol);
	}
	return false; /* the game is not over yet */
}

/* taking care of the first move of the game.
 * in case we are in game-mode 1, and the user plays the black pieces, then playing the computer move
 */
void gui_first_move(Gameboard *board) {
	if (board->game_mode == 1 && board->user_color == 0) {
		Gameboard *copy = copy_board(board);
		Move move = find_best_move(copy, copy->difficulty);
		destroy_board(copy);
		set_step(board, move.srow, move.scol, move.drow, move.dcol);
	}
}

void save_game_from_gui(Gameboard *game) {
	promote_saves();
	FILE *file = fopen(saved_files[0], "w");
	assert(file != NULL);
	save_xml(file, game);
	fclose(file);
}

/* ask the user if he wants to save the game before leaving   */
StayOrLeave suggest_save(Gameboard *game) {
	const SDL_MessageBoxButtonData buttons[] = {
	        { 0, 0, "no" },
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
	        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" },
	    };
	const SDL_MessageBoxColorScheme colorScheme = {
		{ /* .colors (.r, .g, .b) */
			/* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
			{ 255,   0,   0 },
			/* [SDL_MESSAGEBOX_COLOR_TEXT] */
			{   0, 255,   0 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
			{ 255, 255,   0 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
			{   0,   0, 255 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
			{ 255,   0, 255 }
		}
	};
	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION, /* .flags */
		NULL, /* .window */
		"Save The Game?", /* .title */
		"do you want to save the game before leaving?", /* .message */
		SDL_arraysize(buttons), /* .numbuttons */
		buttons, /* .buttons */
		&colorScheme /* .colorScheme */
	};
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
		SDL_Log("error displaying message box");
		return Error; /* error */
	}
	if (buttonid == -1 || buttonid == 2) { /* i'll treat 'no selection' as cancel */
		return Stay;
	} else if (buttonid == 1) { /* 'yes' option, so we need to save */
		save_game_from_gui(game); /* notice we don't need to update window->data->saved_game, since we are leaving the game-window anyway */
	}
	return Leave; /* we leave without saving */
}

Window_type handle_game_buttons(Window *window, Button* clicked_button, Gameboard **game) {
	StayOrLeave op;
	switch(clicked_button->type) {
		case RestartButton:
			reset_board(game);
			window->data->board_widget->board = *game;
			return Game;
		case SaveButton:
			save_game_from_gui(*game);
			window->data->saved_game = true;
			return Game;
		case LoadButton:
			return LoadGame;
		case UndoButton:
			if (clicked_button->active) {
				double_undo(window->data->board_widget->board);
				if (ArrayListSize(window->data->board_widget->board->history) < 2) {
					window->buttons[UndoButtonIndex]->active = false;
				}
			}
			return Game;
		case MenuButton:
			op = suggest_save(*game);
			if (op == Error) {
				/* what should we do?????????????????????????????? */
			} else if (op == Stay) {
				return Game;
			} else {
				destroy_board(*game);
				*game = create_board(1,1,1); /* reseting the settings */
				return Enterance;
			}
			break;
		case ExitButton:
			op = suggest_save(*game);
			if (op == Error) {
				/* what should we do????????????????????????? */
			} else if (op == Stay) {
				return Game;
			} else {
				return ExitGame;
			}
			break;
		default: /* we need to have default, otherwise it arises warnings */
			return ExitGame;
	}
	return Game;
}

Window_type handle_game_events(Window *window, SDL_Event *event,  Gameboard **game) {
	//printf("color1: %d, color2: %d\n", (*game)->user_color, window->data->board_widget->board->user_color);
	fflush(stdout);
	if (event == NULL || window == NULL ) {
		return ExitGame;
	}
	switch(event->type) {
		case SDL_MOUSEBUTTONDOWN:
			if (mouse_in_rec(event->button.x, event->button.y, window->data->board_widget->location)) {
				int relative_x = event->button.x - window->data->board_widget->location->x;
				int relative_y = event->button.y - window->data->board_widget->location->y;
				int x_board = (8*relative_x / window->data->board_widget->location->w);
				int y_board = 7 - (8*relative_y / window->data->board_widget->location->h);
				Piece *piece = window->data->board_widget->board->board[y_board][x_board];
				if (piece->type != Empty && piece->colur == window->data->board_widget->board->user_color) { /* the user clicked on one of his pieces */
					window->data->selected_piece_color = piece->colur;
					window->data->selected_piece_index = piece->indexat;
					if (event->button.button == SDL_BUTTON_LEFT) { /* selected the piece for moving */
						window->data->picked_piece  = true;
					} else if (event->button.button == SDL_BUTTON_RIGHT && ((*game)->game_mode == 1)
							&& ((*game)->difficulty == 1 || (*game)->difficulty == 2)) {
						window->data->highlight_moves = true;
					}

				}
			}
			else if (event->button.button == SDL_BUTTON_LEFT) { /* the click wasn't inside the board, and it's a left click  */
				Button* clicked_button = get_button_clicked(event, window->buttons, window->num_buttons);
				if (clicked_button != NULL) { /* some button was clicked */
					return handle_game_buttons(window, clicked_button, game);
				}
			}
			return Game; /* if no case was applied */
		case SDL_MOUSEBUTTONUP:
			if (event->button.button == SDL_BUTTON_LEFT && window->data->picked_piece) { /* the selected piece was dropped */
				window->data->picked_piece  = false;
				if (mouse_in_rec(event->button.x, event->button.y, window->data->board_widget->location)) {
					int relative_x = event->button.x - window->data->board_widget->location->x;
					int relative_y = event->button.y - window->data->board_widget->location->y;
					int x_board = (8*relative_x / window->data->board_widget->location->w);
					int y_board = 7 - (8*relative_y / window->data->board_widget->location->h);
					Piece *piece = window->data->board_widget->board->all_pieces[window->data->selected_piece_color][window->data->selected_piece_index];
					CHESS_BOARD_MESSAGE mssg = is_valid_step(window->data->board_widget->board, piece->row, piece->col, y_board, x_board);
					if (mssg == CHESS_BOARD_SUCCESS) {
						graphical_handle_move(window, piece->row, piece->col, y_board, x_board);
						window->data->saved_game = false;
					}
				}
			}
			if (ArrayListSize(window->data->board_widget->board->history) > 1 && window->data->board_widget->board->game_mode == 1) {
				window->buttons[UndoButtonIndex]->active = true;
			}
			return Game;;
		case SDL_MOUSEMOTION:
			if(!window->data->picked_piece || !mouse_in_rec(event->motion.x, event->motion.y, window->data->board_widget->location)) {
				window->data->picked_piece = false;
			}
			return Game;
		default:
			return Game;
	}
	return Game;
}
