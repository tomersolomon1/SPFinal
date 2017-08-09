/*
 * SPArrayList.c
 *
 *  Created on: May 25, 2017
 *      Author: sapir
 */

#include "SPArrayList.h"


SPArrayList* spArrayListCreate(int maxSize)
{
	SPArrayList* array = (SPArrayList*) malloc(sizeof(SPArrayList));

	array->maxSize = maxSize;
	array->elements = (int*) malloc(sizeof(int) * maxSize);

	array->actualSize = 0;
	return array;
}

SPArrayList* spArrayListCopy(SPArrayList* src)
{
	if(src == NULL){ return NULL; }
	SPArrayList* dst = (SPArrayList*) malloc(sizeof(SPArrayList));

	dst->actualSize = src->actualSize;
	dst->maxSize = src->maxSize;
	int *dst_elements = (int*) malloc(sizeof(int) * src->maxSize);

	if(dst_elements == NULL){ return NULL;}
	memcpy(dst_elements, src->elements, (sizeof(int) * src->maxSize));
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

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index){
	if(src == NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if(src->actualSize == src->maxSize){
		return SP_ARRAY_LIST_FULL;
	}
	if((src->actualSize) < index){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}

	int one_before = *((src->elements) + index);
	*((src->elements) + index) = elem;
	int temp = 0;

	for (int i = index + 1; i < (src->maxSize); i++){
		temp = *((src->elements) + i);
		*((src->elements) + i) = one_before;
		one_before = temp;
	}
	(src->actualSize)++;
	return SP_ARRAY_LIST_SUCCESS;
}

 SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem){
	 return spArrayListAddAt(src, elem, 0);
 }

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem){
	return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
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

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	return spArrayListRemoveAt(src, (src->actualSize) - 1);
}

int spArrayListGetAt(SPArrayList* src, int index){
	return *(src->elements + index);
}

int spArrayListGetFirst(SPArrayList* src){
	return spArrayListGetAt(src, 0);
}

int spArrayListGetLast(SPArrayList* src){
	return spArrayListGetAt(src, (src->actualSize) - 1);
}

int spArrayListMaxCapacity(SPArrayList* src){
	return src->maxSize;
}

int spArrayListSize(SPArrayList* src){
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

SP_ARRAY_LIST_MESSAGE spArrayListPushFirst(SPArrayList* src, int elem){
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

void spArrayListPrint(SPArrayList* src){
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
