/*
 * Window.h
 *
 *  Created on: 31 баев„ 2017
 *      Author: User
 */

#ifndef GRAPHICS_WINDOW_H_
#define GRAPHICS_WINDOW_H_

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
	SDL_Renderer *renderer;

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

#endif /* GRAPHICS_WINDOW_H_ */
