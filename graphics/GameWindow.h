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


#define DEFAULT_GAME_WINDOW_WIDTH 800
#define DEFAULT_GAME_WINDOW_HEIGHT 800

// buttons panel dimensions
#define DEFAULT_GAME_BUTTON_PANEL_WIDTH (DEFAULT_GAME_WINDOW_SIZE / 8)
#define DEFAULT_GAME_BUTTON_PANEL_HEIGHT DEFAULT_GAME_WINDOW_HEIGHT

// button related measures
#define DEFAULT_GAME_BUTTON_WIDTH (DEFAULT_GAME_BUTTON_PANEL_WIDTH / 2)
#define DEFAULT_GAME_BUTTON_HEIGHT (DEFAULT_GAME_WINDOW_HEIGHT / 10)
#define GAME_WINDOW_NUMBER_OF_BUTTONS 8 /* including the empty buttons */
#define DEFAULT_GAME_BUTTON_VERTICAL_GAP (DEFAULT_GAME_WINDOW_HEIGHT - (GAME_WINDOW_NUMBER_OF_BUTTONS * DEFAULT_GAME_BUTTON_HEIGHT)) / 8
#define DEFAULT_GAME_BUTTON_HORIZONTAL_GAP (DEFAULT_GAME_BUTTON_PANEL_WIDTH / 10)

// board panel dimensions
#define BOARD_PANEL_WIDTH (DEFAULT_GAME_WINDOW_WIDTH - DEFAULT_GAME_BUTTON_PANEL_WIDTH)
#define BOARD_PANEL_HEIGHT DEFAULT_GAME_WINDOW_HEIGHT

// board basic measures
#define DEFAULT_BOARD_MAXIMAL_WIDTH  (BOARD_PANEL_WIDTH - (2 * DEFAULT_GAME_BUTTON_HORIZONTAL_GAP))
#define DEFAULT_BOARD_MAXIMAL_HEIGHT (BOARD_PANEL_HEIGHT - (2 * DEFAULT_GAME_BUTTON_VERTICAL_GAP))
//#define BOARD_DIMENSION MIN(DEFAULT_BOARD_MAXIMAL_WIDTH, DEFAULT_BOARD_MAXIMAL_HEIGHT)

typedef struct board_widget {
	Gameboard *board;
	SDL_Rect  *location;

	// board grid
	SDL_Texture *board_grid;

	// white pieces
	SDL_Texture *WKing;
	SDL_Texture *WQueen;
	SDL_Texture *WRock;
	SDL_Texture *WBishop;
	SDL_Texture *WKnight;
	SDL_Texture *WPawn;

	// black pieces
	SDL_Texture *BKing;
	SDL_Texture *BQueen;
	SDL_Texture *BRock;
	SDL_Texture *BBishop;
	SDL_Texture *BKnight;
	SDL_Texture *BPawn;
} BoardWidget;

typedef struct game_window_t {
	//menu_window_type type;
	SDL_Window* window;
	SDL_Renderer* windowRenderer;
	int num_buttons;
	Button** buttons;
	BoardWidget *board_widget;
} GameWindow;

#endif /* GRAPHICS_GAMEWINDOW_H_ */
