/*
 * SPCommon.h
 *
 *  Created on: 27 баев„ 2017
 *      Author: User
 */

#ifndef GRAPHICS_SPCOMMON_H_
#define GRAPHICS_SPCOMMON_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <unistd.h>

#define IMG(x) "./graphics/images/"#x".bmp"
#define IMG_INCTV(x) "./graphics/images/"#x"_inactive.bmp"

extern const char *saved_files[];
int count_saves();
void promote_saves();
SDL_Rect* spCopyRect(SDL_Rect* src);
#endif /* GRAPHICS_SPCOMMON_H_ */
