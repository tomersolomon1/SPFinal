/*
 * SPCommon.h
 *
 *  Created on: 27 баев„ 2017
 *      Author: User
 */

#ifndef GRAPHICS_SPCOMMON_H_
#define GRAPHICS_SPCOMMON_H_
#include <SDL.h>
#include <SDL_video.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../GameBoard.h"
#include "../Files.h"

#define IMG(x) "./graphics/images/"#x".bmp"
#define IMG_INCTV(x) "./graphics/images/"#x"_inactive.bmp"
#define SAVED_GAME_PATH() "./graphics/saved_games/save%d.xml"
#define SAVED_GAME_PATH_LENGTH 33

#define AMOUNT_GAME_SLOTS 5

void set_game_path(char* game_path, int i);
int count_saves();
void promote_saves();
SDL_Rect* spCopyRect(SDL_Rect* src);
bool mouse_in_rec(int x, int y, SDL_Rect *rect);
#endif /* GRAPHICS_SPCOMMON_H_ */
