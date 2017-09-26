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

Window_type gui_begin_game(Window *window, Gameboard *gameboard) {
	if (CHECK_COMPUTER_START(gameboard)) { /* checks if it's the computer's turn */
		Gameboard *copy = copy_board(gameboard);
		StepValue *best_move = find_best_step(copy, copy->difficulty);
		Step *step = best_move->step;
		destroy_board(copy);
		int move_consequences = graphical_handle_single_move(window, step->srow, step->scol, step->drow, step->dcol,
		false, best_move->promote_to);
		destroy_step_value(best_move);
		if (move_consequences == 1) { /* the game is over */
			return ExitGame;
		} else if (move_consequences == 0) { /* the game isn't over */
			return Game;
		} else { /* move_consequences == -1, which means we had some problem with SDL along the way */
			return SDLErrorWindow;
		}
	}
	return Game;
}

Piece_type choose_promotion() {
	const SDL_MessageBoxButtonData buttons[] = {
			{ 0, 0, "Pawn" },
			{ 0, 1, "Knight" },
			{ 0, 2, "Bishop" },
			{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 3, "Rook" },
			{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 4, "Queen" },
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
		"Promotion!", /* .title */
		"Which piece do you want your pawn to be promoted to?", /* .message */
		SDL_arraysize(buttons), /* .numbuttons */
		buttons, /* .buttons */
		&colorScheme /* .colorScheme */
	};
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
		SDL_Log("error displaying message box");
		return Empty; /* signaling error */
	}
	return buttonid; /* we leave without saving */
}

/*
 * assuming the move is legal
 * set the step, and show a SimpleMessageBox if the game is over
 * return:
 * 		1  	- the game is over
 * 		0 	- the game isn't over
 * 		-1 	- if some error occurred
 */
int graphical_handle_single_move(Window *window, int srow, int scol, int drow, int dcol,
		bool is_user_move, Piece_type computer_promotion) {
	char *colors[] = {"black", "white"};
	Gameboard *board = window->data->board_widget->board;
	CHESS_BOARD_MESSAGE msg = commit_move(board, srow, scol, drow, dcol, false, computer_promotion);
	if (msg == CHESS_BOARD_PROMOTION && is_user_move) {
		Piece_type piece_type = choose_promotion();
		if (piece_type != Empty)  {
			make_promotion(board, drow, dcol, piece_type);
		} else { /* piece_type == empty, means SDL error occurred */
			return -1;
		}
	}
	drawWindow(window, NULL);
	int game_over = is_game_over(board);
	char mssg[50];
	if (game_over == 0 || game_over == 1 || game_over == 2) { /* the game is over */
		if (game_over == 2) { /* it's a tie */
			strcpy(mssg, "It's a tie!");
		} else { /* somebody won the game */
			sprintf(mssg, "Checkmate! %s player wins the game", colors[game_over]);
		}
		int success = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", mssg, NULL);
		int res = (success == 0 ? 1 : -1);
		return res;
	} else if (is_under_check(board)) {
		sprintf(mssg, "Check: %s King is threatened!", colors[board->turn]);
		return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Check!", mssg, NULL);
	}
	return 0;
}

/* return 0 if the game is still on, 1 if the game is over, and -1 if error occurred */
int graphical_handle_move(Window *window, int srow, int scol, int drow, int dcol) {
	int move_result = graphical_handle_single_move(window, srow, scol, drow, dcol, true, Empty);
	if (move_result == 1) { /* the game is over */
		return 1;
	} else if (move_result == -1) { /* some error occurred */
		return -1;
	} else if (window->data->board_widget->board->game_mode == 1) { /* the game is not over, and we need to play the computer's turn */
		Gameboard *copy = copy_board(window->data->board_widget->board);
		StepValue *best_move = find_best_step(copy, copy->difficulty);
		Step *best_step = best_move->step;
		int game_over = graphical_handle_single_move(window, best_step->srow, best_step->scol, best_step->drow, best_step->dcol, false, best_move->promote_to);
		destroy_step_value(best_move);
		destroy_board(copy);
		return game_over;
	}
	return 0; /* the game is not over yet */
}

void save_game_from_gui(Gameboard *game) {
	promote_saves();
	char game_path[SAVED_GAME_PATH_LENGTH];
	set_i_th_game_path(game_path, 0);
	FILE *file = fopen(game_path, "w");
	assert(file != NULL);
	save_xml(file, game);
	fclose(file);
}

/* ask the user if he wants to save the game before leaving    */
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
	StayOrLeave op = Leave; /* default */
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
			if (!window->data->saved_game) { /* the game wasn't saved */
				op = suggest_save(*game);
			}
			if (op == Error) {
				return SDLErrorWindow; /* we are shutting down in case of error */
			} else if (op == Stay) {
				return Game;
			} else {
				destroy_board(*game);
				*game = create_board(1,1,1); /* reseting the settings */
				return Enterance;
			}
			break;
		case ExitButton:
			if (!window->data->saved_game) { /* the game wasn't saved */
				op = suggest_save(*game);
			}
			if (op == Error) {
				return SDLErrorWindow; /* we are shutting down in case of error */
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
	Window_type next_window = Game; /* default */
	if (event == NULL || window == NULL ) {
		return ExitGame;
	}
	if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
		if (mouse_in_rec(event->button.x, event->button.y, window->data->board_widget->location)) { /* the event was inside the board */
			int relative_x = event->button.x - window->data->board_widget->location->x;
			int relative_y = event->button.y - window->data->board_widget->location->y;
			int x_board = (8*relative_x / window->data->board_widget->location->w); /* calculating the x (column) coordinate on the board */
			int y_board = 7 - (8*relative_y / window->data->board_widget->location->h); /* calculating the y (row) coordinate on the board */
			Piece *piece = window->data->board_widget->board->board[y_board][x_board]; /* getting the piece on (row, col) coordinates */
			if (piece->type != Empty &&  event->type == SDL_MOUSEBUTTONDOWN && /* checking the conditions for selecting a piece */
				((piece->colur == window->data->board_widget->board->user_color && window->data->board_widget->board->game_mode == 1)
				|| (piece->colur == window->data->board_widget->board->turn && window->data->board_widget->board->game_mode == 2))) {
				window->data->selected_piece_color = piece->colur;
				window->data->selected_piece_index = piece->indexat;
				if (event->button.button == SDL_BUTTON_LEFT) { /* selecting the piece for moving */
					window->data->picked_piece  = true;
				} else if (event->button.button == SDL_BUTTON_RIGHT && ((*game)->game_mode == 1) && ((*game)->difficulty == 1 || (*game)->difficulty == 2)) {
					window->data->highlight_moves = true; /* turning on the highlight squares feature */
				}
			} else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && window->data->picked_piece) {
				window->data->picked_piece  = false; /* the piece is no longer selected */
				piece = window->data->board_widget->board->all_pieces[window->data->selected_piece_color][window->data->selected_piece_index];
				if (is_valid_step(window->data->board_widget->board, piece->row, piece->col, y_board, x_board) == CHESS_BOARD_SUCCESS) {
					int move_result = graphical_handle_move(window, piece->row, piece->col, y_board, x_board);
					window->data->saved_game = false;
					if (move_result == 1) {
						next_window = ExitGame; /* the game is over */
					} else if (move_result == 0) {
						next_window = Game; /* the game is still on */
					} else {
						next_window = SDLErrorWindow; /* some SDL related error occurred */
					}
				}
				if (ArrayListSize(window->data->board_widget->board->history) > 1 && window->data->board_widget->board->game_mode == 1) {
					window->buttons[UndoButtonIndex]->active = true; /* updating the undo button, if necessary */
				}
			}
		} else if (event->button.button == SDL_BUTTON_LEFT && event->type == SDL_MOUSEBUTTONUP) { /* the click wasn't inside the board, and it's a left click  */
			Button* clicked_button = get_button_clicked(event, window->buttons, window->num_buttons);
			if (clicked_button != NULL) { /* some button was clicked */
				return handle_game_buttons(window, clicked_button, game);
			}
		}
	} else if (event->type == SDL_MOUSEMOTION && (!window->data->picked_piece /* to be changed?*/
			|| !mouse_in_rec(event->motion.x, event->motion.y, window->data->board_widget->location))) {
		window->data->picked_piece = false;
	}
	return next_window;
}

