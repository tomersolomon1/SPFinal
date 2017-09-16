/*
 * GuiManager_Game.h
 *
 *  Created on: 3 בספט׳ 2017
 *      Author: User
 */

#ifndef GRAPHICS_GUIMANAGER_GAME_H_
#define GRAPHICS_GUIMANAGER_GAME_H_
#include "GameWindow.h"
#include "WindowDataStruct.h"

typedef enum {
	Stay,
	Leave,
	Error
} StayOrLeave;

// handle event and related functions
Window_type handle_game_events(Window *window, SDL_Event* event, Gameboard **board);
int graphical_handle_single_move(Window *window, int srow, int scol, int drow, int dcol, bool is_user_move, Piece_type computer_promotion);
int graphical_handle_move(Window *window, int srow, int scol, int drow, int dcol);
void save_game_from_gui(Gameboard *game);
StayOrLeave suggest_save(Gameboard *game);

// for setting the first move
void gui_first_move(Gameboard *board);
#endif /* GRAPHICS_GUIMANAGER_GAME_H_ */
