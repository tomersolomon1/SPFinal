/*
 * Button.c
 *
 *  Created on: 27 ����� 2017
 *      Author: User
 */

#include <string.h>
#include <stdlib.h>

#include "Button.h"
#include "SPCommon.h"

Button *createButton(SDL_Renderer* windowRender, SDL_Rect* location, const char *active_image,
		const char *inactive_image, ButtonType type, bool active, bool visibility) {
	if (windowRender == NULL || location == NULL || active_image == NULL || inactive_image == NULL) {
		return NULL ;
	}
	Button *button = (Button*) malloc(sizeof(Button)); /* allocate the button*/
	assert(button != NULL);
	button->active_buttonTexture = create_texure_from_bmp(windowRender, active_image, false);
	if(button->active_buttonTexture == NULL){
		free(button);
		return NULL;
	}
	button->inactive_buttonTexture = create_texure_from_bmp(windowRender, inactive_image, false);
	if (button->inactive_buttonTexture == NULL) {
		SDL_DestroyTexture(button->active_buttonTexture);
		free(button);
		return NULL; /* we had had problem in creating textures one of the textures, so we return NULL */
	}
	button->location = spCopyRect(location);
	button->windowRenderer = windowRender;
	button->type = type;
	button->active = active;
	button->visibility = visibility;
	return button;
}

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
	if (button == NULL )
		return;
	free(button->location);
	SDL_DestroyTexture(button->active_buttonTexture);
	SDL_DestroyTexture(button->inactive_buttonTexture);
	free(button);
}

int drawButton(Button* button) {
	int success;
	if (button == NULL || !(button->visibility))
		return 0; /* not drawing the button at all */
	if (button->active)
		success = SDL_RenderCopy(button->windowRenderer, button->active_buttonTexture, NULL, button->location);
	else
		success = SDL_RenderCopy(button->windowRenderer, button->inactive_buttonTexture, NULL, button->location);
	if(success == -1){
		printf("Error: There was a problem with SDL_RenderCopy\n");
		return -1;
	}
	return 0;
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
