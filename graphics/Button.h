/*
 * Button.h
 *
 *  Created on: 27 баев„ 2017
 *      Author: User
 */

#ifndef GRAPHICS_BUTTON_H_
#define GRAPHICS_BUTTON_H_

#include "Widget.h" // A button is a widget also

struct button_t {
	SDL_Texture* buttonTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
} Button;

//You need a create function:
Widget* createButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image);

// this function in order to destroy all data Associate with a button:
void destroyButton(Widget*);

void handleButtonEvenet(Widget* src, SDL_Event* event);

void drawButton(Widget* src);


#endif /* GRAPHICS_BUTTON_H_ */
