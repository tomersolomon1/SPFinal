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

/* this file contains general helping functions for the gui code files */

#define IMG(x) "./graphics/images/"#x".bmp" //string format of path of active button image
#define IMG_INCTV(x) "./graphics/images/"#x"_inactive.bmp" //string format of path of inactive button image
#define SAVED_GAME_PATH_FORMAT "./graphics/saved_games/save%d.xml" //string format of saved-to-gui game path
#define SAVED_GAME_PATH_LENGTH 33
#define AMOUNT_GAME_SLOTS 5

/* set into var game_path the path of the i-th saved-to-gui-game
 * the game paths are in the string format of SAVED_GAME_PATH_FORMAT,
 * and the "%d" in SAVED_GAME_PATH_FORMAT string format is replaced with i*/
void set_i_th_game_path(char* game_path, int i);

/* count how many saved-files-for-gui exist,
 * the first save file that doesn't exist ends the counting
 * assuming the files name are counted 0,1,...,AMOUNT_GAME_SLOTS-1
 * while 0 = newest game, AMOUNT_GAME_SLOTS-1 = oldest game*/
int count_saves();

/* promote the saved game
 * i-th game -> (i+1)-th game
 * delete the oldest game if there were already AMOUNT_GAME_SLOTS games saved */
void promote_saves();

/* copy SDL Rectangle
 * on failure returns NULL*/
SDL_Rect* spCopyRect(SDL_Rect* src);

/* returns point in rectangle by (x,y) coordinates */
bool mouse_in_rec(int x, int y, SDL_Rect *rect);

/* return the texture on success,
 * otherwise return NULL (the returned texture might be NULL) */
SDL_Texture *create_texure_from_bmp(SDL_Renderer *renderer, const char *bmp_path, bool transparent_background);

#endif /* GRAPHICS_SPCOMMON_H_ */
