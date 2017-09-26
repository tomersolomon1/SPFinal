/*
 * Button.h
 *
 *  Created on: 27 баев„ 2017
 *      Author: User
 */

#ifndef GRAPHICS_BUTTON_H_
#define GRAPHICS_BUTTON_H_
#define DEFAULT_MENU_WINDOW_WIDTH 700
#define DEFAULT_MENU_WINDOW_HIGHT 650
#define DEFAULT_BTN_HIGHT 50
#define DEFAULT_BTN_WIDTH 150
#define DEFAULT_BTN_GAP_VERTICAL 60
#define DEFAULT_BTN_GAP_HORIZONTAL 170
#define DEFAULT_GAP_WINDOW 100
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL.h>
#include <SDL_video.h>

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
	SetWhite,
	/* game buttons */
	RestartButton,
	SaveButton,
	UndoButton,
	MenuButton
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

/* create Button and returns a pointer to it
 * returns NULL on failure*/
Button *createButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char *active_image, const char *inactive_image, ButtonType type, bool active, bool visibility);

/* helper function, for creating list of buttons to a window by the parameters given
 * return:
 * on success - array of pointer to buttons
 * on failure - NULL*/
Button **create_buttons(SDL_Renderer* renderer, ButtonType types[], int buttons_number, int x_btn_places[],
		int y_btn_places[],	const char* image[], const char* image_inavtice[], bool active[], bool visible[], int btn_height, int btn_width);

/* returns the Button Type of the Button that was clicked in the event
 * returns NoButton if no Button was clicked */
ButtonType which_button_clicked(SDL_Event *event, Button **buttons, int buttons_number);

/* returns a pointer to the Button that was clicked in the event
 * returns NULL if no Button was clicked */
Button *get_button_clicked(SDL_Event* event, Button** buttons, int buttons_number);

/* this function is in charge of destroying all the data Associated with a button */
void destroyButton(Button *);

/* draw the button
 * in failure returns -1, in success returns 0*/
int drawButton(Button *);

#endif /* GRAPHICS_BUTTON_H_ */
