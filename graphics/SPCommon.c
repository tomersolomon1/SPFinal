/*
 * SPCommon.c
 */


#include "SPCommon.h"
#include <stdlib.h>
#include <stdio.h>

const char *saved_files[] = {"saved games/save0.xml", "saved games/save1.xml", "saved games/save2.xml",
		"saved games/save3.xml", "saved games/save4.xml" };

/* count how many saved-files exist, the first save file that doesn't exist ends the counting */
int count_saves() {
	int count = 0;
	while (count < 5 && (access(saved_files[count], F_OK) == 0)) {
		count++;
	}
	return count;
}

void promote_saves() {
	int saves = count_saves();
	int i = saves-1; /* default */
	if (saves == 5) {
		remove(saved_files[4]); /* the most old saved file is erased */
		i = saves-2;
	}
	for (; i >= 0; i--) {
		rename(saved_files[i], saved_files[i+1]);
	}
}

//Helper function
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



