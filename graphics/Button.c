/*
 * Button.c
 *
 *  Created on: 27 баев„ 2017
 *      Author: User
 */

#include <string.h>
#include <stdlib.h>

#include "Button.h"
#include "SPCommon.h"

Button *createButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char *active_image, const char *inactive_image, ButtonType type, bool active, bool visibility) {
	if (windowRender == NULL || location == NULL || active_image == NULL || inactive_image == NULL) {
		return NULL ;
	}
	// Allocate data
	Button *button = (Button*) malloc(sizeof(Button));
	assert(button != NULL);
	SDL_Surface* loadingSurface1        = SDL_LoadBMP(active_image); // We use the surface as a temp var;
	SDL_Surface* loadingSurface2        = SDL_LoadBMP(inactive_image); // We use the surface as a temp var;
	SDL_Texture* active_buttonTexture   = SDL_CreateTextureFromSurface(windowRender, loadingSurface1);
	SDL_Texture* inactive_buttonTexture = SDL_CreateTextureFromSurface(windowRender, loadingSurface2);

	if (loadingSurface1 == NULL || loadingSurface2 == NULL
			|| active_buttonTexture == NULL || inactive_buttonTexture == NULL) {
		free(button);
		SDL_FreeSurface(loadingSurface1); // It's safe to pass NULL
		SDL_FreeSurface(loadingSurface2); // It's safe to pass NULL
		SDL_DestroyTexture(active_buttonTexture); // It's safe to pass NULL
		SDL_DestroyTexture(inactive_buttonTexture); // It's safe to pass NULL
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface1);
	SDL_FreeSurface(loadingSurface2);
	button->active_buttonTexture   = active_buttonTexture;
	button->inactive_buttonTexture = inactive_buttonTexture;
	button->location = spCopyRect(location);
	button->windowRenderer = windowRender;
	button->type = type;
	button->active = active;
	button->visibility = visibility;
	return button;
}

/* helper function, for creating list of buttons
 * return:
 * 		on success - array of pointer to buttons
 * 		on failure - NULL
 */
Button **create_buttons(SDL_Renderer* renderer, ButtonType types[], int buttons_number, int x_btn_places[],
		int y_btn_places[],	const char* image[], const char* image_inavtice[], bool active[], bool visible[], int btn_height, int btn_width) {
	Button** buttons = (Button **) malloc(sizeof(Button*) * buttons_number);
	assert(buttons != NULL);
	for (int i = 0; i < buttons_number; i++) {
		SDL_Rect Rec = {.x = x_btn_places[i], .y = y_btn_places[i], .h = btn_height, .w = btn_width};
		buttons[i] = createButton(renderer, &Rec, image[i], image_inavtice[i], types[i], active[i], visible[i]);
		if(buttons[i] == NULL){
			for(int j = 0; j < i; j++) {
				destroyButton(buttons[j]);
			}
			free(buttons);
			return NULL;
		}
	}
	return buttons;
}


void destroyButton(Button* button) {
	if (button == NULL ) {
		return;
	}
	free(button->location);
	SDL_DestroyTexture(button->active_buttonTexture);
	SDL_DestroyTexture(button->inactive_buttonTexture);
	free(button);
}

void drawButton(Button* button) {
	if (button == NULL || !(button->visibility)) {
		return; /* not drawing the button at all */
	}
	if (button->active) {
		SDL_RenderCopy(button->windowRenderer, button->active_buttonTexture, NULL, button->location);
	} else {
		SDL_RenderCopy(button->windowRenderer, button->inactive_buttonTexture, NULL, button->location);
	}
}

ButtonType which_button_clicked(SDL_Event* event, Button** buttons, int buttons_number) {
	for (int i = 0; i < buttons_number; i++) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, buttons[i]->location)) {
			return buttons[i]->type;
		}
	}
	return NoButton; /* the mouse-click wasn't inside any of the buttons */
}

Button* get_button_clicked(SDL_Event* event, Button** buttons, int buttons_number) {
	for (int i = 0; i < buttons_number; i++) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, buttons[i]->location) && buttons[i]->visibility) {
			return buttons[i];
		}
	}
	return NULL; /* the mouse-click wasn't inside any of the buttons */
}
