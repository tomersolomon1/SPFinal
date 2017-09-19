/*
 * SPCommon.c
 */


#include "SPCommon.h"
#include <stdlib.h>

/* count how many saved-files exist, the first save file that doesn't exist ends the counting */
int count_saves() {
	int count = 0;
	while (count < AMOUNT_GAME_SLOTS && (access(SAVED_GAME(count), F_OK) == 0)) {
		count++;
	}
	return count;
}

void promote_saves(){
	int saves = count_saves();
	int i = saves - 1; /* default */
	if (saves == AMOUNT_GAME_SLOTS) {
		remove(SAVED_GAME(AMOUNT_GAME_SLOTS - 1)); /* the most old saved file is erased */
		i = saves - 2;
	}
	for (; i >= 0; i--) {
		rename(SAVED_GAME(i), SAVED_GAME(i+1));
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

