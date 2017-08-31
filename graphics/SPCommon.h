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

#define IMG(x) "./graphics/images/"#x".bmp"
#define IMG_INCTV(x) "./graphics/images/"#x"_inactive.bmp"

SDL_Rect* spCopyRect(SDL_Rect* src);
#endif /* GRAPHICS_SPCOMMON_H_ */
