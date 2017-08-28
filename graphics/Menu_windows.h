/*
 * Menu_windows.h
 *
 *  Created on: Aug 27, 2017
 *      Author: sapir
 */

#ifndef GRAPHICS_MENU_WINDOWS_H_
#define GRAPHICS_MENU_WINDOWS_H_
#include "Button.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#define IMG(x) "./images/"#x".bmp"
#define IMG_INCTV(x) "./images/"#x"_inactive.bmp"


typedef enum menu_window_type_t{
	Enterance,
	LoadGame,
	ModeGame,
	Difficulty,
	ChooseColor
} menu_window_type;

typedef struct menu_window_t{
	menu_window_type type;
	SDL_Window* window;
	SDL_Renderer* windowRenderer;
	int num_buttons;
	Button** buttons;
} MenuWindow;

MenuWindow* create_menu_window(menu_window_type type);
Button** create_enterance_buttons(SDL_Renderer* renderer);
Button** create_load_game_buttons(SDL_Renderer* renderer);
Button** create_game_mode_buttons(SDL_Renderer* renderer);
Button** create_difficulty_buttons(SDL_Renderer* renderer);
Button** create_choose_color_buttons(SDL_Renderer* renderer);
void destroyMenuWindow(MenuWindow src);
void drawWindow(MenuWindow* src);
ButtonType handleEvenet_enterance(MenuWindow* src, SDL_Event* event);
#endif /* GRAPHICS_MENU_WINDOWS_H_ */
