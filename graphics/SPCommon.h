/*
 * SPCommon.h
 *
 *  Created on: 27 баев„ 2017
 *      Author: User
 */

#ifndef GRAPHICS_SPCOMMON_H_
#define GRAPHICS_SPCOMMON_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#define IMG(x) "./graphics/images/"#x".bmp"
#define IMG_INCTV(x) "./graphics/images/"#x"_inactive.bmp"

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
} GameData;

typedef struct window_t{
	menu_window_type type;
	SDL_Window* window;
	SDL_Renderer* windowRenderer;
	int num_buttons;
	Button** buttons;
	GameData* data;
} Window;

SDL_Rect* spCopyRect(SDL_Rect* src);
#endif /* GRAPHICS_SPCOMMON_H_ */
