/*
 * Button.h
 *
 *  Created on: 27 баев„ 2017
 *      Author: User
 */

#ifndef GRAPHICS_BUTTON_H_
#define GRAPHICS_BUTTON_H_

#include "Widget.h" // A button is a widget also

typedef enum {
	/* general button */
	NoButton,
	BackButton,
	StartButton,
	NextButton,

	/* main window */
	NewGameButton,
	LoadButton,
	ExitButton,

	/* load buttons */
	GameSlot1,
	GameSlot2,
	GameSlot3,
	GameSlot4,
	GameSlot5,

	/* game mode */
	OnePlayer,
	TwoPlayer,

	/* difficulty buttons */
	NoobDiff,
	EasyDiff,
	ModerateDiff,
	HardDiff,

	/* set color buttons */
	SetBlack,
	SetBlack
} ButtonType;

typedef struct button_t {
	SDL_Texture* buttonTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	ButtonType type;
} Button;

ButtonType which_button_clicked(Button *buttons, int buttons_number);

//You need a create function:
Widget* createButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image);

// this function in order to destroy all data Associate with a button:
void destroyButton(Widget*);

void handleButtonEvenet(Widget* src, SDL_Event* event);

void drawButton(Widget* src);


#endif /* GRAPHICS_BUTTON_H_ */
