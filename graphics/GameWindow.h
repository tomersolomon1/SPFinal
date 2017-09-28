/*
 * GameWindow.h
 *
 *  Created on: 28 баев„ 2017
 *      Author: User
 */

#ifndef GRAPHICS_GAMEWINDOW_H_
#define GRAPHICS_GAMEWINDOW_H_

#include "../GameBoard.h"
#include "../MiniMax.h"
#include "Button.h"
#include "SPCommon.h"
#include "WindowDataStruct.h"

#define CHESS_IMAGE(x) IMG(ChessPieces/x)

// buttons panel dimensions
#define DEFAULT_GAME_BUTTON_PANEL_WIDTH (DEFAULT_GAME_WINDOW_WIDTH / 4)
#define DEFAULT_GAME_BUTTON_PANEL_HEIGHT DEFAULT_GAME_WINDOW_HEIGHT

// button related measures
#define DEFAULT_GAME_BUTTON_WIDTH (5 * DEFAULT_GAME_BUTTON_PANEL_WIDTH / 6)
#define DEFAULT_GAME_BUTTON_HEIGHT (DEFAULT_GAME_WINDOW_HEIGHT / 10)
#define GAME_WINDOW_NUMBER_OF_BUTTONS 8 /* including the empty buttons */
#define AMOUNT_BUTTONS_GAME 6			/* amount buttons actually seen in the game window */
#define DEFAULT_GAME_BUTTON_VERTICAL_GAP ((DEFAULT_GAME_WINDOW_HEIGHT - (GAME_WINDOW_NUMBER_OF_BUTTONS * DEFAULT_GAME_BUTTON_HEIGHT)) / 8)
#define DEFAULT_GAME_BUTTON_HORIZONTAL_GAP (DEFAULT_GAME_BUTTON_PANEL_WIDTH / 20)
#define UndoButtonIndex 3

// board panel dimensions
#define BOARD_PANEL_WIDTH (DEFAULT_GAME_WINDOW_WIDTH - DEFAULT_GAME_BUTTON_PANEL_WIDTH)
#define BOARD_PANEL_HEIGHT DEFAULT_GAME_WINDOW_HEIGHT

// board basic measures
#define DEFAULT_BOARD_MAXIMAL_WIDTH  (BOARD_PANEL_WIDTH - (2 * DEFAULT_GAME_BUTTON_HORIZONTAL_GAP))
#define DEFAULT_BOARD_MAXIMAL_HEIGHT (BOARD_PANEL_HEIGHT - (2 * DEFAULT_GAME_BUTTON_VERTICAL_GAP))
//#define BOARD_DIMENSION MIN(DEFAULT_BOARD_MAXIMAL_WIDTH, DEFAULT_BOARD_MAXIMAL_HEIGHT)

// for drawing the pieces in the center of the squares
#define VERTICAL_CENTERING 2
#define HORIZONTAL_CENTERING 4

/* destroy game texture */
void destroy_game_textures(BoardWidget *board_widget);

/* destroy game data */
void destory_data(GameData *data);

/* create game data and returns a pointer to it
 * on failure returns NULL */
GameData *create_game_data(SDL_Renderer* renderer, Gameboard *board);

/* create widget board and returns a pointer to it
 * on failure returns NULL */
BoardWidget *create_widget_board(SDL_Renderer *window_renderer, Gameboard *board, SDL_Rect* location);

/* helping function for create_widget_board
 * initialize widget board*/
BoardWidget *init_widget_board(Gameboard *board, SDL_Rect* location);

/* create buttons to game window
 * sets all parameters and create the buttons
 * returns NULL on failure */
Button **create_game_buttons(SDL_Renderer* window_renderer);

/* highlight the squares of possible moves of the chosen selected_piece (in data)*/
int highlight_moves_feature(GameData *data, SDL_Renderer *renderer, int row_dim, int col_dim);

/* helping function for highlight_moves_feature
 * highlight a specific square with texture
 * delta_x, delta_y, specify the square position in the board*/
int highlight_square(GameData* data, SDL_Renderer *renderer, SDL_Texture* texture, int delta_x, int delta_y, int row_dim, int col_dim);

/* helping function for highlight_moves_feature
 * if the selected_piece is rook, we want to check if castling is possible,
 * and if so - highlight the king's position*/
int highlight_king_for_castling(GameData *data, SDL_Renderer *renderer, Piece* selected_piece, int row_dim, int col_dim);

/* draw board the board
 * on success return 0
 * on failure returns -1 */
int draw_board(GameData *data, SDL_Renderer *renderer, SDL_Event* event);

/* debug function */
Window *create_game_window(Gameboard *board);
/* debug function */
void drawGameWindow(Window* src, SDL_Event* event);
/* debug function */
void recognize_square(Window *window, int x, int y);

#endif /* GRAPHICS_GAMEWINDOW_H_ */
