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
#define MAX_MSG_LENGTH 50

typedef enum {
	Stay,
	Leave,
	Error
} StayOrLeave;

/* handle event and related functions
 * returns the next window type to be open
 * in case of problem returns SDLErrorWindow */
Window_type handle_game_events(Window *window, SDL_Event* event, Gameboard **board);

/* handle the case that button_clicked clicked
 * returns the next window type to be open
 * in case of problem returns SDLErrorWindow */
Window_type handle_game_buttons(Window *window, Button* clicked_button, Gameboard **game);

/* assuming the move is legal
 * set the step, and show a SimpleMessageBox if the game is over
 * return:
 * 		1  	- the game is over
 * 		0 	- the game isn't over
 * 	   -1 	- if some error occurred */
int graphical_handle_single_move(Window *window, int srow, int scol, int drow, int dcol, bool is_user_move, Piece_type computer_promotion);

/* return 0 if the game is still on, 1 if the game is over, and -1 if error occurred */
int graphical_handle_move(Window *window, int srow, int scol, int drow, int dcol);

/* saves game as a gui-game*/
void save_game_from_gui(Gameboard *game);

/* open a message box to ask the user
 * if he wants to save the game before leaving
 * returns Error if an error occurred */
StayOrLeave suggest_save(Gameboard *game);

/* setting the first move
 * return:
 * 		ExitGame  		- the game is over
 * 		Game 			- the game isn't over
 * 	    SDLErrorWindow 	- if some error occurred */
Window_type gui_begin_game(Window *window);

/* shows a message box for the user to choose the type of the piece in the promotion
 * returns the chosen peace type
 * if problem occurred return Empty */
Piece_type choose_promotion();

#endif /* GRAPHICS_GUIMANAGER_GAME_H_ */
