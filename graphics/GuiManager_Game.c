#include "GameWindow.h"
#include "SPCommon.h"
#include "WindowDataStruct.h"
#include "GuiManager_Game.h"
#include "Windows.h"
#include "../Files.h"

Window_type gui_begin_game(Window *window) {
	Gameboard *board = window->data->board_widget->board;
	if (CHECK_COMPUTER_START(board)) { /* checks if it's the computer's turn */
		StepValue *best_move = find_best_step(board, board->difficulty);
		Step *step = best_move->step;
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
		printf("Error: There was a problem with SDL_ShowMessageBox\n");
		return Empty; /* signaling error */
	}
	return buttonid; /* we leave without saving */
}

int graphical_handle_single_move(Window *window, int srow, int scol, int drow, int dcol,
		bool is_user_move, Piece_type computer_promotion) {
	int success;
	char *colors[] = {"black", "white"};
	Gameboard *board = window->data->board_widget->board;
	CHESS_BOARD_MESSAGE msg = commit_move(board, srow, scol, drow, dcol, false, computer_promotion);
	if (msg == CHESS_BOARD_PROMOTION && is_user_move) {
		Piece_type piece_type = choose_promotion();
		if (piece_type != Empty)
			make_promotion(board, drow, dcol, piece_type);
		else /* piece_type == empty, means SDL error occurred */
			return -1;
	}
	success = drawWindow(window, NULL);
	if(success == -1)
		return -1;
	int game_over = is_game_over(board);
	char mssg[MAX_MSG_LENGTH];
	if (game_over == 0 || game_over == 1 || game_over == 2) { /* the game is over */
		if (game_over == 2) /* it's a tie */
			strcpy(mssg, "It's a tie!");
		else /* somebody won the game */
			sprintf(mssg, "Checkmate! %s player wins the game", colors[game_over]);
		success = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", mssg, NULL);
		if(success == -1){
			printf("Error: There was a problem with SDL_ShowSimpleMessageBox\n");
			return -1;
		}
		return 1;
	} else if (is_under_check(board)) {
		sprintf(mssg, "Check: %s King is threatened!", colors[board->turn]);
		success =  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Check!", mssg, NULL);
		if(success == -1){
			printf("Error: There was a problem with SDL_ShowSimpleMessageBox\n");
			return -1;
		}
	}
	return 0;
}

int graphical_handle_move(Window *window, int srow, int scol, int drow, int dcol) {
	int move_result = graphical_handle_single_move(window, srow, scol, drow, dcol, true, Empty);
	if (move_result == 1) { /* the game is over */
		return 1;
	} else if (move_result == -1) { /* some error occurred */
		return -1;
	} else if (window->data->board_widget->board->game_mode == 1) { /* the game is not over, and we need to play the computer's turn */
		StepValue *best_move = find_best_step(window->data->board_widget->board, window->data->board_widget->board->difficulty);
		Step *best_step = best_move->step;
		int game_over = graphical_handle_single_move(window, best_step->srow, best_step->scol, best_step->drow, best_step->dcol, false, best_move->promote_to);
		destroy_step_value(best_move);
		return game_over;
	}
	return 0; /* the game is not over yet */
}

void save_game_from_gui(Gameboard *game) {
	promote_saves();
	char game_path[SAVED_GAME_PATH_LENGTH];
	set_i_th_game_path(game_path, 0);
	FILE *file = fopen(game_path, "w");
	if(file == NULL){
		printf("Error: There was a problem saving the game\n");
		return;
	}
	save_xml(file, game);
	fclose(file);
}

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
		printf("Error: There was a problem with SDL_ShowMessageBox\n");
		return Error; /* error */
	}
	if (buttonid == -1 || buttonid == 2) /* i'll treat 'no selection' as cancel */
		return Stay;
	else if (buttonid == 1) /* 'yes' option, so we need to save */
		save_game_from_gui(game); /* notice we don't need to update window->data->saved_game, since we are leaving the game-window anyway */
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
				if (ArrayListSize(window->data->board_widget->board->history) < 2)
					window->buttons[UndoButtonIndex]->active = false;
			}
			return Game;
		case MenuButton:
			if (!window->data->saved_game) /* the game wasn't saved */
				op = suggest_save(*game);
			if (op == Error)
				return SDLErrorWindow; /* we are shutting down in case of error */
			else if (op == Stay)
				return Game;
			else {
				destroy_board(*game);
				*game = create_board(1,1,1); /* reseting the settings */
				return Enterance;
			}
			break;
		case ExitButton:
			if (!window->data->saved_game)/* the game wasn't saved */
				op = suggest_save(*game);
			if (op == Error)
				return SDLErrorWindow; /* we are shutting down in case of error */
			else if (op == Stay)
				return Game;
			else
				return ExitGame;
			break;
		default:
			return ExitGame;
	}
	return Game;
}

Window_type handle_game_events(Window *window, SDL_Event *event,  Gameboard **game) {
	Window_type next_window = Game; /* default */
	if (event == NULL || window == NULL )
		return ExitGame;
	Gameboard *data_board = window->data->board_widget->board;
	SDL_Rect *data_location = window->data->board_widget->location;
	if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
		if (mouse_in_rec(event->button.x, event->button.y, data_location)) { /* the event was inside the board */
			int relative_x = event->button.x - data_location->x;
			int relative_y = event->button.y - data_location->y;
			int x_board = (8 * relative_x / data_location->w); /* calculating the x (column) coordinate on the board */
			int y_board = 7 - (8 * relative_y / data_location->h); /* calculating the y (row) coordinate on the board */
			Piece *piece = data_board->board[y_board][x_board]; /* getting the piece on (row, col) coordinates */
			if (piece->type != Empty &&  event->type == SDL_MOUSEBUTTONDOWN && /* checking the conditions for selecting a piece */
				((piece->colur == data_board->user_color && data_board->game_mode == 1)
				|| (piece->colur == data_board->turn && data_board->game_mode == 2))) {
				window->data->selected_piece_color = piece->colur;
				window->data->selected_piece_index = piece->indexat;
				if (event->button.button == SDL_BUTTON_LEFT) /* selecting the piece for moving */
					window->data->picked_piece  = true;
				else if (event->button.button == SDL_BUTTON_RIGHT && /* turning on the highlight squares feature */
						((*game)->game_mode == 1) && ((*game)->difficulty == 1 || (*game)->difficulty == 2))
					window->data->highlight_moves = true;
			} else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && window->data->picked_piece) {
				window->data->picked_piece  = false; /* the piece is no longer selected */
				piece = data_board->all_pieces[window->data->selected_piece_color][window->data->selected_piece_index];
				int move_result = 0;
				bool had_step = false;
				int dlta_cast = delta_castling_move(data_board, piece, piece->row, piece->col, y_board, x_board);
				if (dlta_cast == 1 || dlta_cast == -1){
					move_result = graphical_handle_move(window, piece->row, 4, piece->row, 4 + 2 * dlta_cast); //4 is the init position of king on the board
					had_step = true;
				} else if (dlta_cast == 0 && is_valid_step(data_board, piece->row, piece->col, y_board, x_board) == CHESS_BOARD_SUCCESS) {
					move_result = graphical_handle_move(window, piece->row, piece->col, y_board, x_board);
					had_step = true;
				}
				if(had_step){
					window->data->saved_game = false;
					next_window = next_window_after_making_move(move_result);
				}
				if (ArrayListSize(data_board->history) > 1 && data_board->game_mode == 1)
					window->buttons[UndoButtonIndex]->active = true; /* updating the undo button, if necessary */
			}
		} else if (event->button.button == SDL_BUTTON_LEFT && event->type == SDL_MOUSEBUTTONUP) { /* the click wasn't inside the board, and it's a left click  */
			Button* clicked_button = get_button_clicked(event, window->buttons, window->num_buttons);
			if (clicked_button != NULL) /* some button was clicked */
				return handle_game_buttons(window, clicked_button, game);
		}
	} else if (event->type == SDL_MOUSEMOTION && (!window->data->picked_piece
			|| !mouse_in_rec(event->motion.x, event->motion.y, data_location))) {
		window->data->picked_piece = false;
	}
	return next_window;
}

int delta_castling_move(Gameboard* data_board, Piece* p_chosen, int srow, int scol, int drow, int dcol){
	if(p_chosen->type == King && abs(scol - dcol) == 2)
		return 2;
	int delta = 0;
	if(p_chosen->type == King && data_board->board[drow][dcol]->type == Rook){
		delta = (dcol > scol ? 1: -1);
		if(is_valid_step(data_board, srow, 4, srow, 4 + 2 * delta) == CHESS_BOARD_SUCCESS){
			return delta;
		}
	}
	if(p_chosen->type == Rook && data_board->board[drow][dcol]->type == King){
		delta = (dcol > scol ? -1: 1);
		if(is_valid_step(data_board, srow, 4, srow, 4 + 2 * delta) == CHESS_BOARD_SUCCESS){
			return delta;
		}
	}
	return delta;
}

Window_type next_window_after_making_move(int move_result){
	if (move_result == 1)
		return ExitGame; /* the game is over */
	else if (move_result == 0)
		return Game; /* the game is still on */
	else
		return SDLErrorWindow; /* some SDL related error occurred */
}
