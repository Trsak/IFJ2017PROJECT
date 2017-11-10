/**
 * @file symtable.c
 * @author Jan Bartosek (xbarto92)
 * @brief Operations with Binary Tree and Pointer Stack
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "symtable.h"

/**
 * @copydoc stackInit
 */
void stackInit(Stack *S) {
	BinaryTreePtr *pointer;
	pointer = (BinaryTreePtr *) calloc(2, sizeof(BinaryTreePtr));
	if (pointer == NULL) {
		//TODO what error call on malloc fail?
		//fprintf(stderr, "Not enough memory to initialize stack\n");
		exit(1);
	}
	S->pointer = pointer;
	S->currentSize = 2;
	S->top = -1;
}

/**
 * @copydoc stackDestroy
 */
void stackDestroy(Stack *S) {
	free(S->pointer);
	S->pointer = NULL;
	S->currentSize = 0;
	S->top = -1;
}

/**
 * @copydoc stackResize
 */
void stackResize(Stack *S) {
	BinaryTreePtr *resizedContent;
	resizedContent = (BinaryTreePtr *) calloc(S->currentSize * 2, sizeof(BinaryTreePtr));
	memcpy(resizedContent, S->pointer, sizeof(BinaryTreePtr) * S->top + 1);
	free(S->pointer);
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
void stackPush(Stack *S, BinaryTreePtr ptr) {
	if (stackFull(S) == 1)
		stackResize(S);
	S->top = S->top + 1;
	S->pointer[S->top] = ptr;
}

/**
 * @copydoc stackPop
 */
BinaryTreePtr stackPop(Stack *S) {
	if (stackEmpty(S) == 0) {
		BinaryTreePtr ptr = S->pointer[S->top];
		S->top = S->top - 1;
		return ptr;
	}
	//TODO error when stack is empty?
	//fprintf(stderr, "Stack is empty\n");
	exit(1);
}

/**
 * @copydoc treeInsert
 */
void treeInsert(BinaryTreePtr rootPtr, struct Values data) {
	int compare;
	bool equal = false;
	while (rootPtr != NULL) {
		compare = strcmp(rootPtr->data.name, data.name);
		if (compare == 0) {
			rootPtr->data.value = data.value;
			equal = true;
			break;
		} else if (compare < 0) {
			*rootPtr = *rootPtr->LPtr;
		} else {
			*rootPtr = *rootPtr->RPtr;
		}
	}
	if (!equal) {
		BinaryTreePtr newPtr = (BinaryTreePtr) malloc(sizeof(struct BinaryTree));
		//TODO check malloc
		newPtr->LPtr = NULL;
		newPtr->RPtr = NULL;
		newPtr->data = data;
		*rootPtr = *newPtr;
	}
}

/**
 * @copydoc getVariable
 */
struct Values *getVariable(BinaryTreePtr rootPtr, const char *name) {
	int compare;
	while (rootPtr != NULL) {
		compare = strcmp(rootPtr->data.name, name);
		if (compare == 0) {
			return &rootPtr->data;
		} else if (compare < 0) {
			*rootPtr = *rootPtr->LPtr;
		} else {
			*rootPtr = *rootPtr->RPtr;
		}
	}
	return NULL;
}

/**
 * @copydoc DisposeTree
 */
void DisposeTree(BinaryTreePtr *rootPtr) {
	if (*rootPtr != NULL) {
		Stack *stack = (Stack *) malloc(sizeof(Stack));
		stackInit(stack);
		do {
			if (*rootPtr == NULL) {
				if (!stackEmpty(stack)) {
					*rootPtr = stackPop(stack);
				}
			} else {
				if ((*rootPtr)->RPtr != NULL) {
					stackPush(stack, (*rootPtr)->RPtr);
				}
				BinaryTreePtr helpPtr = *rootPtr;
				*rootPtr = (*rootPtr)->LPtr;
				free(helpPtr);
			}
		} while (*rootPtr != NULL || !stackEmpty(stack));
		free(stack);
	}
}