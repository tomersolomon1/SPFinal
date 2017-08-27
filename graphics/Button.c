/*
 * Button.c
 *
 *  Created on: 27 баев„ 2017
 *      Author: User
 */

#include <string.h>
#include <stdlib.h>
#include "Widget.h"
#include "Button.h"
#include "SPCommon.h"

// #include "SPCommon.h"

// You need a create function:
Button *createButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image, ButtonType type) {
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	// Allocate data
	Button *button = (Button*) malloc(sizeof(Button));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //We use the surface as a temp var;
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(windowRender,
			loadingSurface);
	if (button == NULL || button == NULL || loadingSurface == NULL
			|| buttonTexture == NULL) {
		free(button);
		free(button);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(buttonTexture); ////It is safe to pass NULL
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //Surface is not actually needed after texture is created
	button->buttonTexture = buttonTexture;
	button->location = spCopyRect(location);
	button->windowRenderer = windowRender;

	return button;
}


void destroyButton(Button* button) {
	if (button == NULL ) {
		return;
	}
	free(button->location);
	SDL_DestroyTexture(button->buttonTexture);
	free(button);
}

void drawButton(Button button) {
	if (button == NULL ) {
		return;
	}
	SDL_RenderCopy(button->windowRenderer, button->buttonTexture, NULL,
			button->location);
}
