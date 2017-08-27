/*
 * Button.h
 *
 *  Created on: 27 ����� 2017
 *      Author: User
 */

#ifndef GRAPHICS_BUTTON_H_
#define GRAPHICS_BUTTON_H_
#define DEFAULT_MENU_WINDOW_WIDTH 1000
#define DEFAULT_MENU_WINDOW_HIGHT 1000
#define DEFAULT_BTN_HIGHT 70
#define DEFAULT_BTN_WIDTH 150
#define DEFAULT_BTN_GAP_VERTICAL 120
#define DEFAULT_BTN_GAP_HORIZONTAL 60
#define DEFAULT_GAP_WINDOW 100
#include <stdbool.h>
//.y = position_buttons + DEFAULT_BTN_GAP*(i++)

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
	SetWhite
} ButtonType;

typedef struct button_t {
	SDL_Texture* active_buttonTexture;
	SDL_Texture* inactive_buttonTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	ButtonType type;
	bool active;
	bool visibility;
} Button;

ButtonType which_button_clicked(SDL_Event* event, Button *buttons, int buttons_number);

//You need a create function:
Button *createButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char *active_image, const char *inactive_image, ButtonType type, bool active, bool visibility);

// this function is in charge of destroying all the data Associated with a button
void destroyButton(Button*);

void drawButton(Button*);


#endif /* GRAPHICS_BUTTON_H_ */
