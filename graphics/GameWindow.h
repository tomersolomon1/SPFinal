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
#include "Button.h"
#include "SPCommon.h"

#define CHESS_IMAGE(x) IMG(ChessPieces/x)


#define DEFAULT_GAME_WINDOW_WIDTH 600
#define DEFAULT_GAME_WINDOW_HEIGHT 600

// buttons panel dimensions
#define DEFAULT_GAME_BUTTON_PANEL_WIDTH (DEFAULT_GAME_WINDOW_WIDTH / 8)
#define DEFAULT_GAME_BUTTON_PANEL_HEIGHT DEFAULT_GAME_WINDOW_HEIGHT

// button related measures
#define DEFAULT_GAME_BUTTON_WIDTH (DEFAULT_GAME_BUTTON_PANEL_WIDTH / 2)
#define DEFAULT_GAME_BUTTON_HEIGHT (DEFAULT_GAME_WINDOW_HEIGHT / 10)
#define GAME_WINDOW_NUMBER_OF_BUTTONS 8 /* including the empty buttons */
#define DEFAULT_GAME_BUTTON_VERTICAL_GAP ((DEFAULT_GAME_WINDOW_HEIGHT - (GAME_WINDOW_NUMBER_OF_BUTTONS * DEFAULT_GAME_BUTTON_HEIGHT)) / 8)
#define DEFAULT_GAME_BUTTON_HORIZONTAL_GAP (DEFAULT_GAME_BUTTON_PANEL_WIDTH / 10)

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


typedef struct board_widget {
	Gameboard *board;
	SDL_Rect  *location;
	SDL_Renderer *renderer;

	// board grid
	SDL_Texture *board_grid;

	// pieces
	SDL_Texture *piece_textures[2][6]; // black = 0, and white = 1, each one of them has 6 different type of pieces

} BoardWidget;

typedef struct game_window_t {
	//menu_window_type type;
	SDL_Window* window;
	SDL_Renderer* windowRenderer;
	int num_buttons;
	Button** buttons;
	bool picked_piece;
	int selected_piece_color;
	int selected_piece_index;
	BoardWidget *board_widget;
} GameWindow;

GameWindow *create_game_window(Gameboard *board);
BoardWidget *create_widget_board(SDL_Renderer *window_renderer, Gameboard *board, SDL_Rect* location);
void drawGameWindow(GameWindow* src);
void handle_game_events(GameWindow *window, SDL_Event* event);

#endif /* GRAPHICS_GAMEWINDOW_H_ */
