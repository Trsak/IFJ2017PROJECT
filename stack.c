/**
 * @file stack.c
 * @author Jan Bartosek (xbarto92)
 * @brief Operations with Dynamic Stack
 */


#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include "error_codes.h"
#include "garbage_collector.h"

/**
 * @copydoc stackInit
 */
int stackInit(Stack *S) {
	void *pointer;
	pointer = (void *) calloc(2, sizeof(void));
	if (pointer == NULL) {
		printErrAndExit (ERROR_INTERNAL, "There is a memory error while allocating stack structure.");
		return ERROR_INTERNAL;
	}
	S->pointer = pointer;
	S->currentSize = 2;
	S->top = -1;
	return 0;
}

/**
 * @copydoc stackDestroy
 */
void stackDestroy(Stack *S) {
	gcfree(S->pointer);
	S->pointer = NULL;
	S->currentSize = 0;
	S->top = -1;
}

/**
 * @copydoc stackResize
 */
void stackResize(Stack *S) {
	void *resizedContent;
	resizedContent = (void *) calloc(S->currentSize * 2, sizeof(void));
	memcpy(resizedContent, S->pointer, sizeof(void) * S->top + 1);
	gcfree(S->pointer);
	S->pointer = resizedContent;
	S->currentSize = S->currentSize * 2;
}

/**
 * @copydoc stackEmpty
 */
bool stackEmpty(Stack *S) {
	if (S->top < 0)
		return true;
	else
		return false;
}

/**
 * @copydoc stackFull
 */
bool stackFull(Stack *S) {
	if (S->top == S->currentSize - 1)
		return true;
	else
		return false;
}

/**
 * @copydoc stackPush
 */
void stackPush(Stack *S, void* ptr) {
	if (stackFull(S) == 1)
		stackResize(S);
	S->top = S->top + 1;
	//S->pointer[S->top] = *ptr; //TODO causes error after changing to void* ... replaced with the line under !!! CHECK IT !!!
	S->pointer = ptr;
}

/**
 * @copydoc stackPop
 */
void* stackPop(Stack *S) {
	if (stackEmpty(S) == 0) {
		//void* ptr = S->pointer[S->top]; //TODO the same error as above
		void* ptr = S->pointer;
		S->top = S->top - 1;
		return ptr;
	}
	//TODO error when stack is empty?
	//fprintf(stderr, "Stack is empty\n");
	return NULL;
}