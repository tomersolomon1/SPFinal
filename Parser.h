/*
 * Parser.h
 *
 *  Created on: 10 баев„ 2017
 *      Author: User
 */

#ifndef PARSER_H_
#define PARSER_H_

typedef enum {
	/* Settings-Mode commands */
	Set_GameMode,
	Set_Difficulty,
	Set_UserColur,
	Load,
	Restore_Default,
	Print_Settings,
	Start,

	/* Game-Mode commands */
	Move,
	Save,
	Undo_Move,
	Reset,

	Quit, /* this command is for both modes */
};

#endif /* PARSER_H_ */
