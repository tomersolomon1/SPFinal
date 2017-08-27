/*
 * Widget.h
 *
 *  Created on: 27 ����� 2017
 *      Author: User
 */

#ifndef GRAPHICS_WIDGET_H_
#define GRAPHICS_WIDGET_H_

#ifndef WIDGET_H_
#define WIDGET_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
typedef enum window_type_t{
	Menu,
	Load,
	Game
} WindowType;

typedef struct widget_t Widget;

typedef void (*EventOP)(Widget*, SDL_Event*);

struct widget_t {
	void (*drawWidget)(Widget*);
	void (*handleEvent)(Widget*, SDL_Event*);
	void (*op)(Widget*, SDL_Event*);
	void (*destroyWidget)(Widget*);
	void *data;
};

typedef struct window_t Window;
struct window_t {
	WindowType type;
	SDL_Window* window;
	void (*drawWindow)(Window* );
	void (*handleEventWindow)(Window* , SDL_Event* );
	void (*destroyWindow)(Window* );
	SDL_Renderer* windowRenderer;
	Widget** widgets;
	int numOfWidgets;
};

//This function would be usefull for NULL safe desetroy
void destroyWidget(Widget* src);
#endif


#endif /* GRAPHICS_WIDGET_H_ */