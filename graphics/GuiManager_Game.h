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

// handle event and related functions
Window_type handle_game_events(Window *window, SDL_Event* event, Gameboard **board);
bool graphical_handle_single_move(Window *window, int srow, int scol, int drow, int dcol);
bool graphical_handle_move(Window *window, int srow, int scol, int drow, int dcol);
void save_game_from_gui(Gameboard *game);


#endif /* GRAPHICS_GUIMANAGER_GAME_H_ */
