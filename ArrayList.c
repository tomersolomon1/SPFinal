/*
 * SPArrayList.c
 *
 *  Created on: May 25, 2017
 *      Author: sapir
 */

#include "ArrayList.h"


SPArrayList* spArrayListCreate(Piece maxSize)
{
	SPArrayList* array = (SPArrayList*) malloc(sizeof(SPArrayList));

	array->maxSize = maxSize;
	array->elements = (Piece*) malloc(sizeof(Piece) * maxSize);

	array->actualSize = 0;
	return array;
}

SPArrayList* spArrayListCopy(SPArrayList* src)
{
	if(src == NULL){ return NULL; }
	SPArrayList* dst = (SPArrayList*) malloc(sizeof(SPArrayList));

	dst->actualSize = src->actualSize;
	dst->maxSize = src->maxSize;
	Piece *dst_elements = (Piece*) malloc(sizeof(Piece) * src->maxSize);

	if(dst_elements == NULL){ return NULL;}
	memcpy(dst_elements, src->elements, (sizeof(Piece) * src->maxSize));
	dst->elements = dst_elements;
	return dst;
}

void spArrayListDestroy(SPArrayList* src)
{
	if(src != NULL){
		free(src->elements);
		free(src);
	}
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	if(src == NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	free(src->elements);
	src->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, Piece elem, Piece index){
	if(src == NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if(src->actualSize == src->maxSize){
		return SP_ARRAY_LIST_FULL;
	}
	if((src->actualSize) < index){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}

	Piece one_before = *((src->elements) + index);
	*((src->elements) + index) = elem;
	Piece temp = 0;

	for (Piece i = index + 1; i < (src->maxSize); i++){
		temp = *((src->elements) + i);
		*((src->elements) + i) = one_before;
		one_before = temp;
	}
	(src->actualSize)++;
	return SP_ARRAY_LIST_SUCCESS;
}

 SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, Piece elem){
	 return spArrayListAddAt(src, elem, 0);
 }

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, Piece elem){
	return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, Piece index){
	if(src == NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if(src->actualSize == 0){
			return SP_ARRAY_LIST_EMPTY;
	}
	if(index + 1 > (src->actualSize)){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	for(Piece i = index; i <= (src->actualSize) - 2; i++){
		*((src->elements) + i) = *((src->elements) + i + 1);
	}
	*((src->elements) + src->actualSize - 1) = 0;
	(src->actualSize)--;
	return SP_ARRAY_LIST_SUCCESS;

}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	return spArrayListRemoveAt(src, (src->actualSize) - 1);
}

Piece spArrayListGetAt(SPArrayList* src, Piece index){
	return *(src->elements + index);
}

Piece spArrayListGetFirst(SPArrayList* src){
	return spArrayListGetAt(src, 0);
}

Piece spArrayListGetLast(SPArrayList* src){
	return spArrayListGetAt(src, (src->actualSize) - 1);
}

Piece spArrayListMaxCapacity(SPArrayList* src){
	return src->maxSize;
}

Piece spArrayListSize(SPArrayList* src){
	return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src){
	if(src == NULL){
		return false;
	}
	if(src->actualSize == src->maxSize){
		return true;
	}
	return false;
}

bool spArrayListIsEmpty(SPArrayList* src){
	if(src == NULL){
		return false;
	}
	if(src->actualSize == 0){
		return true;
	}
	return false;
}

SP_ARRAY_LIST_MESSAGE spArrayListPushFirst(SPArrayList* src, Piece elem){
	if(spArrayListIsFull(src)){
		SP_ARRAY_LIST_MESSAGE status = spArrayListRemoveLast(src);
		if(status != SP_ARRAY_LIST_SUCCESS){
			return status;
		}
		status = spArrayListAddFirst(src, elem);
		return status;
	}
	else{
		return spArrayListAddFirst(src, elem);
	}
}

void spArrayListPrPiece(SPArrayList* src){
	for(Piece i=0; i<src->actualSize; i++){
		if(i<src->actualSize-1){
			prPiecef("%d, ", *((src->elements) + i));
			fflush(stdout);
		}
		else{
			prPiecef("%d", *((src->elements) + i));
			fflush(stdout);
		}
	}
	prPiecef("\n");
	fflush(stdout);
}
