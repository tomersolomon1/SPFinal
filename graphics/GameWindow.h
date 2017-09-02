/*
 * GameWindow.h
 *
 *  Created on: 28 баев„ 2017
 *      Author: User
 */

#ifndef GRAPHICS_GAMEWINDOW_H_
#define GRAPHICS_GAMEWINDOW_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "../GameBoard.h"
#include "../MiniMax.h"
#include "Button.h"
#include "SPCommon.h"
#include "WindowDataStruct.h"

#define CHESS_IMAGE(x) IMG(ChessPieces/x)


#define DEFAULT_GAME_WINDOW_WIDTH 700
#define DEFAULT_GAME_WINDOW_HEIGHT 700

// buttons panel dimensions
#define DEFAULT_GAME_BUTTON_PANEL_WIDTH (DEFAULT_GAME_WINDOW_WIDTH / 4)
#define DEFAULT_GAME_BUTTON_PANEL_HEIGHT DEFAULT_GAME_WINDOW_HEIGHT

// button related measures
#define DEFAULT_GAME_BUTTON_WIDTH (5*DEFAULT_GAME_BUTTON_PANEL_WIDTH / 6)
#define DEFAULT_GAME_BUTTON_HEIGHT (DEFAULT_GAME_WINDOW_HEIGHT / 10)
#define GAME_WINDOW_NUMBER_OF_BUTTONS 8 /* including the empty buttons */
#define DEFAULT_GAME_BUTTON_VERTICAL_GAP ((DEFAULT_GAME_WINDOW_HEIGHT - (GAME_WINDOW_NUMBER_OF_BUTTONS * DEFAULT_GAME_BUTTON_HEIGHT)) / 8)
#define DEFAULT_GAME_BUTTON_HORIZONTAL_GAP (DEFAULT_GAME_BUTTON_PANEL_WIDTH / 20)

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

// destroy functions
void destroy_game_textures(BoardWidget *board_widget);
void destory_data(GameData *data);

// create functions
GameData *create_game_data(SDL_Renderer* renderer, Gameboard *board);
BoardWidget *create_widget_board(SDL_Renderer *window_renderer, Gameboard *board, SDL_Rect* location);
Button **create_game_buttons(SDL_Renderer* window_renderer);

// handle event && draw functions
void draw_board(GameData *data, SDL_Renderer *renderer, SDL_Event* event);
Window_type handle_game_events(Window *window, SDL_Event* event, Gameboard **board, Button *clicked_button);


////////////////// debug functions
Window *create_game_window(Gameboard *board);
void drawGameWindow(Window* src, SDL_Event* event);
void recognize_square(Window *window, int x, int y);

#endif /* GRAPHICS_GAMEWINDOW_H_ */
