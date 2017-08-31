/*
 * Menu_windows.h
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */

#ifndef GRAPHICS_WINDOWS_H_
#define GRAPHICS_WINDOWS_H_
#include "Button.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "../GameBoard.h"
#include "SPCommon.h"

#include "GameWindow.h"

typedef enum window_type_t{
	Enterance,
	LoadGame,
	ModeGame,
	Difficulty,
	ChooseColor,
	Game,
	ExitGame
} Window_type;

typedef struct board_widget {
	Gameboard *board;
	SDL_Rect  *location;

	// board grid
	SDL_Texture *board_grid;

	// pieces
	SDL_Texture *piece_textures[2][6]; // black = 0, and white = 1, each one of them has 6 different type of pieces

} BoardWidget;

typedef struct game_data_t {
	BoardWidget *board_widget;
	int selected_piece_color;
	int selected_piece_index;
	bool picked_piece;
} GameData;

typedef struct window_t {
	Window_type type;
	SDL_Window* window;
	SDL_Renderer* windowRenderer;
	int num_buttons;
	Button** buttons;
	GameData* data;
} Window;


typedef Button** (*buttons_creator)(SDL_Renderer*);

Window* create_window(Window_type type, Gameboard* game);
Button** create_enterance_buttons(SDL_Renderer* renderer);
Button** create_load_game_buttons(SDL_Renderer* renderer);
Button** create_game_mode_buttons(SDL_Renderer* renderer);
Button** create_difficulty_buttons(SDL_Renderer* renderer);
Button** create_choose_color_buttons(SDL_Renderer* renderer);
void destroyWindow(Window* src);
void drawWindow(Window* src, SDL_Event* event);
Button* get_button_by_type(Window* wndw, ButtonType type);
void set_buttons_by_game_params(Window* wndw, Gameboard** game);
Window_type handleEvenet(Window* wndw, Gameboard** game);
Window_type handleEvenet_enterance(Window* wndw, Button* btn);
Window_type handleEvenet_load_game(Window* wndw, Button* btn, Gameboard** game);
Window_type handleEvenet_mode_game(Window* wndw, Button* btn, Gameboard** game);
Window_type handleEvenet_difficulty(Window* wndw, Button* btn, Gameboard** game);
Window_type handleEvenet_choose_color(Window* wndw, Button* btn, Gameboard** game);
#endif /* GRAPHICS_WINDOWS_H_ */
