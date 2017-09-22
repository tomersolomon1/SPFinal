/*
 * SPCommon.c
 */


#include "SPCommon.h"
#include <stdlib.h>

/* count how many saved-files exist, the first save file that doesn't exist ends the counting */
void set_game_path(char* game_path, int i){
	sprintf(game_path, SAVED_GAME_PATH(), i);
}

int count_saves() {
	int count = 0;
	char game_path[SAVED_GAME_PATH_LENGTH];
	set_game_path(game_path, count);
	while (count < AMOUNT_GAME_SLOTS && (access(game_path, F_OK) == 0)) {
		count++;
		set_game_path(game_path, count);
	}
	return count;
}

void promote_saves(){
	int saves = count_saves();
	char game_path_old[SAVED_GAME_PATH_LENGTH];
	char game_path_new[SAVED_GAME_PATH_LENGTH];
	int i = saves - 1; /* default */
	if (saves == AMOUNT_GAME_SLOTS) {
		set_game_path(game_path_old, AMOUNT_GAME_SLOTS - 1);
		remove(game_path_old); /* the most old saved file is erased */
		i = saves - 2;
	}
	for (; i >= 0; i--) {
		set_game_path(game_path_old, i);
		set_game_path(game_path_new, i + 1);
		rename(game_path_old, game_path_new);
	}
}

// Helper functions
SDL_Rect* spCopyRect(SDL_Rect* src){
	if(src == NULL){
		return NULL;
	}
	SDL_Rect* res = malloc(sizeof(SDL_Rect));
	if(res==NULL){
		return NULL;
	}
	res->h = src->h;
	res->w = src->w;
	res->x = src->x;
	res->y = src->y;
	return res;
}

/* helper function, to be converted to macro later */
bool mouse_in_rec(int x, int y, SDL_Rect *rect) {
	SDL_Point point = {.x = x, .y = y };
	return SDL_PointInRect(&point, rect);
}


/* return the texture on success, otherwise return NULL (the returned texture might be NULL) */
SDL_Texture *create_texure_from_bmp(SDL_Renderer *renderer, const char *bmp_path, bool transparent_background) {
	SDL_Surface *surface = SDL_LoadBMP(bmp_path); // don't forget to check the board later
	if (surface == NULL) {
		return NULL;
	}
	if (transparent_background) {
		int success = SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255)); // make it's background transparent
		if (success == -1) {
			SDL_FreeSurface(surface);
			return NULL;
		}
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}
