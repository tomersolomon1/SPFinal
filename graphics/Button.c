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
Widget* createButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image) {
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	Button* data = (Button*) malloc(sizeof(Button));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //We use the surface as a temp var;
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(windowRender,
			loadingSurface);
	if (res == NULL || data == NULL || loadingSurface == NULL
			|| buttonTexture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(buttonTexture); ////It is safe to pass NULL
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //Surface is not actually needed after texture is created
	data->buttonTexture = buttonTexture;
	data->location = spCopyRect(location);
	data->windowRenderer = windowRender;
	res->destroyWidget = destroyButton;
	res->drawWidget = drawButton;
	res->handleEvent = handleButtonEvenet;
	res->data = data;
	return res;
}

//You need this function in order to destroy all data Associate with a button:
void destroyButton(Widget* src) {
	if (src == NULL ) {
		return;
	}
	Button* castData = (Button*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->buttonTexture);
	free(castData);
	free(src);
}

void handleButtonEvenet(Widget* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return; //Better to return an error value
	}
	Button* castData = (Button*) src->data;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			SDL_ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "Title",
					"We did it", NULL );
		}
	} else {
		return;
	}
}

void drawButton(Widget* src) {
	if (src == NULL ) {
		return;
	}
	Button* castData = (Button*) src->data;
	SDL_RenderCopy(castData->windowRenderer, castData->buttonTexture, NULL,
			castData->location);
}
