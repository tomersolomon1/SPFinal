/*
 * ArrayList.c
 *
 *  Created on: May 25, 2017
 *      Author: sapir
 */

#include "ArrayList.h"


ArrayList* ArrayListCreate(int maxSize)
{
	ArrayList* array = (ArrayList*) malloc(sizeof(ArrayList));

	array->maxSize = maxSize;
	array->elements = (Move*) malloc(sizeof(Move) * maxSize);

	array->actualSize = 0;
	return array;
}

ArrayList* ArrayListCopy(ArrayList* src)
{
	if(src == NULL){ return NULL; }
	ArrayList* dst = (ArrayList*) malloc(sizeof(ArrayList));

	dst->actualSize = src->actualSize;
	dst->maxSize = src->maxSize;
	Move *dst_elements = (Move*) malloc(sizeof(Move) * src->maxSize);

	if(dst_elements == NULL){ return NULL;}
	memcpy(dst_elements, src->elements, (sizeof(Move) * src->maxSize));
	dst->elements = dst_elements;
	return dst;
}

void ArrayListDestroy(ArrayList* src)
{
	if(src != NULL){
		free(src->elements);
		free(src);
	}
}

SP_ARRAY_LIST_MESSAGE ArrayListClear(ArrayList* src){
	if(src == NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	free(src->elements);
	src->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE ArrayListAddAt(ArrayList* src, Move elem, int index){
	if(src == NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if(src->actualSize == src->maxSize){
		return SP_ARRAY_LIST_FULL;
	}
	if((src->actualSize) < index){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}

	Move one_before = *((src->elements) + index);
	*((src->elements) + index) = elem;
	Move temp = 0;

	for (int i = index + 1; i < (src->maxSize); i++){
		temp = *((src->elements) + i);
		*((src->elements) + i) = one_before;
		one_before = temp;
	}
	(src->actualSize)++;
	return SP_ARRAY_LIST_SUCCESS;
}

 SP_ARRAY_LIST_MESSAGE ArrayListAddFirst(ArrayList* src, Move elem){
	 return ArrayListAddAt(src, elem, 0);
 }

SP_ARRAY_LIST_MESSAGE ArrayListAddLast(ArrayList* src, Move elem){
	return ArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE ArrayListRemoveAt(ArrayList* src, int index){
	if(src == NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if(src->actualSize == 0){
			return SP_ARRAY_LIST_EMPTY;
	}
	if(index + 1 > (src->actualSize)){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	for(int i = index; i <= (src->actualSize) - 2; i++){
		*((src->elements) + i) = *((src->elements) + i + 1);
	}
	*((src->elements) + src->actualSize - 1) = 0;
	(src->actualSize)--;
	return SP_ARRAY_LIST_SUCCESS;

}

SP_ARRAY_LIST_MESSAGE ArrayListRemoveFirst(ArrayList* src){
	return ArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE ArrayListRemoveLast(ArrayList* src){
	return ArrayListRemoveAt(src, (src->actualSize) - 1);
}

Move ArrayListGetAt(ArrayList* src, int index){
	return *(src->elements + index);
}

Move ArrayListGetFirst(ArrayList* src){
	return ArrayListGetAt(src, 0);
}

Move ArrayListGetLast(ArrayList* src){
	return ArrayListGetAt(src, (src->actualSize) - 1);
}

Move ArrayListMaxCapacity(ArrayList* src){
	return src->maxSize;
}

Move ArrayListSize(ArrayList* src){
	return src->actualSize;
}

bool ArrayListIsFull(ArrayList* src){
	if(src == NULL){
		return false;
	}
	if(src->actualSize == src->maxSize){
		return true;
	}
	return false;
}

bool ArrayListIsEmpty(ArrayList* src){
	if(src == NULL){
		return false;
	}
	if(src->actualSize == 0){
		return true;
	}
	return false;
}

SP_ARRAY_LIST_MESSAGE ArrayListPushFirst(ArrayList* src, Move elem){
	if(ArrayListIsFull(src)){
		SP_ARRAY_LIST_MESSAGE status = ArrayListRemoveLast(src);
		if(status != SP_ARRAY_LIST_SUCCESS){
			return status;
		}
		status = ArrayListAddFirst(src, elem);
		return status;
	}
	else{
		return ArrayListAddFirst(src, elem);
	}
}

void ArrayListPrMove(ArrayList* src){
	for(int i=0; i<src->actualSize; i++){
		if(i<src->actualSize-1){
			printf("%d, ", *((src->elements) + i));
			fflush(stdout);
		}
		else{
			printf("%d", *((src->elements) + i));
			fflush(stdout);
		}
	}
	printf("\n");
	fflush(stdout);
}
