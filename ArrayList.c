/*
 * ArrayList.c
 *
 *  Created on: May 25, 2017
 *      Author: sapir
 */

#include "ArrayList.h"

/**
 *  Creates an empty array list with the ecified maximum capacity.
 *  @param maxSize - the maximum capacity of the target array list.
 *  @return
 *  NULL, if an allocation error occurred or maxSize  <= 0.
 *  An instant of an array list otherwise.
 */
ArrayList* ArrayListCreate(int maxSize)
{
	ArrayList* array = (ArrayList*) malloc(sizeof(ArrayList));

	array->maxSize = maxSize;
	array->elements = (Step**) malloc(sizeof(Step*) * maxSize);
	for(int i = 0; i < maxSize; i++){
		array->elements[i] = NULL;
	}
	array->actualSize = 0;
	return array;
}

/**
 *	Creates an exact copy of the src array list. Elements in the new copy will
 *	be in the same order as they appeared in the source list.
 *	@param src - the source array list.
 *	@return
 *	NULL if either an allocation error occurs or src == NULL.
 *	A new copy of the source array list, otherwise.
 */
ArrayList* ArrayListCopy(ArrayList* src)
{
	if(src == NULL){ return NULL; }
	ArrayList* dst = (ArrayList*) malloc(sizeof(ArrayList));

	dst->actualSize = src->actualSize;
	dst->maxSize = src->maxSize;
	dst->elements = (Step**) malloc(sizeof(Step*) * src->maxSize);

	for(int i = 0; i < src->maxSize; i++){
		dst->elements[i] = NULL;
	}
	for(int i = 0; i< src->actualSize; i++){
		dst->elements[i] = copy_step(src->elements[i]);
	}
	return dst;
}

/**
 * Frees all memory resources associated with the source array list. If the
 * source array is NULL, then the function does nothing.
 * @param src - the source array list
 */
void ArrayListDestroy(ArrayList* src)
{
	if(src != NULL){
		for(int i = 0; i< src->actualSize; i++){
			destroy_step(src->elements[i]);
		}
		free(src->elements);
		free(src);
	}
}

/**
 * Clears all elements from the source array list. After invoking this function,
 * the size of the source list will be reduced to zero and maximum capacity is
 * not affected.
 * @param src - the source array list
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT if src == NULL
 * ARRAY_LIST_SUCCESS otherwise
 */
ARRAY_LIST_MESSAGE ArrayListClear(ArrayList* src){
	if(src == NULL){
		return ARRAY_LIST_INVALID_ARGUMENT;
	}
	for(int i = 0; i< src->actualSize; i++){
		destroy_step(src->elements[i]);
		src->elements[i] = NULL;
	}
	src->actualSize = 0;
	return ARRAY_LIST_SUCCESS;
}

/**
 * Inserts element at a ecified index. The elements residing at and after the
 * ecified index will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @param index - the index where the new element will be placed. The index is
 *                0-based.
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * ARRAY_LIST_SUCCESS - otherwise
 */
ARRAY_LIST_MESSAGE ArrayListAddAt(ArrayList* src, Step *elem, int index){
	if(src == NULL){
		return ARRAY_LIST_INVALID_ARGUMENT;
	}
	if(src->actualSize == src->maxSize){
		return ARRAY_LIST_FULL;
	}
	if((src->actualSize) < index){
		return ARRAY_LIST_INVALID_ARGUMENT;
	}

	Step *one_before = src->elements[index];
	src->elements[index] = elem;
	Step *temp = NULL;

	for (int i = index + 1; i < (src->maxSize); i++){
		temp = src->elements[i];
		src->elements[i] = one_before;
		one_before = temp;
	}
	(src->actualSize)++;
	return ARRAY_LIST_SUCCESS;
}

/**
 * Inserts element at a the beginning of the source element. The elements
 * will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * ARRAY_LIST_SUCCESS - otherwise
 */
 ARRAY_LIST_MESSAGE ArrayListAddFirst(ArrayList* src, Step *elem){
	 return ArrayListAddAt(src, elem, 0);
 }

/**
 * Inserts element at a the end of the source element. If the array list
 * reached its maximum capacity and error message is returned and the source
 * list is not affected.
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * ARRAY_LIST_SUCCESS - otherwise
 */
ARRAY_LIST_MESSAGE ArrayListAddLast(ArrayList* src, Step *elem){
	return ArrayListAddAt(src, elem, src->actualSize);
}

/**
 * Removes an element from a ecified index. The elements residing after the
 * ecified index will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @param index - The index from where the new element will be removed.
 *                The index is 0-based.
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * ARRAY_LIST_EMPTY - if the source array list is empty
 * ARRAY_LIST_SUCCESS - otherwise
 */
ARRAY_LIST_MESSAGE ArrayListRemoveAt(ArrayList* src, int index){
	if(src == NULL){
		return ARRAY_LIST_INVALID_ARGUMENT;
	}
	if(src->actualSize == 0){
			return ARRAY_LIST_EMPTY;
	}
	if(index + 1 > (src->actualSize)){
		return ARRAY_LIST_INVALID_ARGUMENT;
	}
	destroy_step(src->elements[index]);
	for(int i = index; i <= (src->actualSize) - 2; i++){
		src->elements[i] = src->elements[i + 1];
	}
	src->elements[src->actualSize - 1] = NULL;
	(src->actualSize)--;
	return ARRAY_LIST_SUCCESS;

}

/**
 * Removes an element from a the beginning of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * ARRAY_LIST_EMPTY - if the source array list is empty
 * ARRAY_LIST_SUCCESS - otherwise
 */
ARRAY_LIST_MESSAGE ArrayListRemoveFirst(ArrayList* src){
	return ArrayListRemoveAt(src, 0);
}

/**
 * Removes an element from a the end of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * ARRAY_LIST_EMPTY - if the source array list is empty
 * ARRAY_LIST_SUCCESS - otherwise.
 */
ARRAY_LIST_MESSAGE ArrayListRemoveLast(ArrayList* src){
	return ArrayListRemoveAt(src, (src->actualSize) - 1);
}

/**
 * Returns the element at the ecified index. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @param index - the ecified index, the index is 0-based.
 * @return
 * Undefined value if either src == NULL or index out of bound.
 * Otherwise, the element at the ecified index is returned.
 */
Step *ArrayListGetAt(ArrayList* src, int index){
	if(index >= src->actualSize){
		return NULL;
	}
	return src->elements[index];
}

/**
 * Returns the element at the beginning of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the beginning of the list is returned.
 */
Step *ArrayListGetFirst(ArrayList* src){
	return ArrayListGetAt(src, 0);
}

/**
 * Returns the element at the end of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the end of the list is returned.
 */
Step *ArrayListGetLast(ArrayList* src){
	return ArrayListGetAt(src, (src->actualSize) - 1);
}

/**
 * Returns the maximum capacity of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the maximum capacity of the list is returned.
 */
int ArrayListMaxCapacity(ArrayList* src){
	return src->maxSize;
}

/**
 * Returns the number of elements in the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the number of the elements in the list is returned.
 */
int ArrayListSize(ArrayList* src){
	return src->actualSize;
}

/**
 * Returns true if the list is full, that is the number of elements in the list
 * equals its maximum capacity.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is less
 * than its maximum capacity.
 * Otherwise, true is returned.
 */
bool ArrayListIsFull(ArrayList* src){
	if(src == NULL){
		return false;
	}
	if(src->actualSize == src->maxSize){
		return true;
	}
	return false;
}

/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */
bool ArrayListIsEmpty(ArrayList* src){
	if(src == NULL){
		return false;
	}
	if(src->actualSize == 0){
		return true;
	}
	return false;
}

/*
 * push element at first, if the src is full:
 * remove the last element and then add at first
 * **/

ARRAY_LIST_MESSAGE ArrayListPushFirst(ArrayList* src, Step *elem){
	if(ArrayListIsFull(src)){
		ARRAY_LIST_MESSAGE status = ArrayListRemoveLast(src);
		if(status != ARRAY_LIST_SUCCESS){
			return status;
		}
		status = ArrayListAddFirst(src, elem);
		return status;
	}
	else{
		return ArrayListAddFirst(src, elem);
	}
}

/**
 * print the list
 * */
void ArrayListPrint(ArrayList* src){
	for(int i=0; i<src->actualSize; i++){
		print_step(src->elements[i]);
	}
	printf("\n");
	fflush(stdout);
}
