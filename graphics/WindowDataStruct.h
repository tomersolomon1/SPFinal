
#ifndef GRAPHICS_WINDOWDATASTRUCT_H_
#define GRAPHICS_WINDOWDATASTRUCT_H_
#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>

#define DEFAULT_MENU_WINDOW_WIDTH 650
#define DEFAULT_MENU_WINDOW_HIGHT 600

#define DEFAULT_GAME_WINDOW_WIDTH 800
#define DEFAULT_GAME_WINDOW_HEIGHT 600

typedef enum window_type_t{
	Enterance,
	LoadGame,
	ModeGame,
	Difficulty,
	ChooseColor,
	Game,
	ExitGame,
	SDLErrorWindow,
} Window_type;

typedef struct board_widget {
	Gameboard *board;
	SDL_Rect  *location;

	// board grid
	SDL_Texture *board_grid;

	// highlighted squares
	SDL_Texture *possible_move_texture;
	SDL_Texture *threatened_move_texture;
	SDL_Texture *capturing_move_texture;

	// pieces
	SDL_Texture *piece_textures[2][6]; // black = 0, and white = 1, each one of them has 6 different type of pieces

} BoardWidget;

typedef struct game_data_t {
	BoardWidget *board_widget;
	int selected_piece_color;
	int selected_piece_index;
	bool picked_piece;
	bool saved_game;
	bool highlight_moves;
} GameData;

typedef struct window_t {
	Window_type type;
	SDL_Window* window;
	SDL_Renderer* windowRenderer;
	int num_buttons;
	Button** buttons;
	GameData* data;
} Window;

#endif /* GRAPHICS_WINDOWDATASTRUCT_H_ */
