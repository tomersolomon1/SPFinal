/*
 * Widget.c
 *
 *  Created on: 27 баев„ 2017
 *      Author: User
 */


#include <stdlib.h>
#include "Widget.h"

void destroyWidget(Widget* src) {//Making NULL Safe Destroy
	if (src == NULL ) {
		return;
	}
	src->destroyWidget(src);
}
