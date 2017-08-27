/*
 * Widget.h
 *
 *  Created on: 27 баев„ 2017
 *      Author: User
 */

#ifndef GRAPHICS_WIDGET_H_
#define GRAPHICS_WIDGET_H_

#ifndef WIDGET_H_
#define WIDGET_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

struct widget_t {
	void (*drawWidget)(Widget*);
	void (*handleEvent)(Widget*, SDL_Event*);
	void (*destroyWidget)(Widget*);
	void* data;
} Widget;

//This function would be usefull for NULL safe desetroy
void destroyWidget(Widget* src);
#endif


#endif /* GRAPHICS_WIDGET_H_ */
