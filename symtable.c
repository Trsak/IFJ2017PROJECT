/**
 * @file symtable.c
 * @author Jan Bartosek (xbarto92)
 * @brief Operations with Binary Tree
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "symtable.h"
#include "stack.h"
#include "error_codes.h"

/**
 * @copydoc treeInsert
 */
int treeInsert(BinaryTreePtr rootPtr, struct Values data) {
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
		BinaryTreePtr newPtr = (BinaryTreePtr) gcmalloc(sizeof(struct BinaryTree));
		if (newPtr == NULL) {
			printErrAndExit (ERROR_INTERNAL, "There is a memory error while allocating tree structure.");
			return ERROR_INTERNAL;
		}
		newPtr->LPtr = NULL;
		newPtr->RPtr = NULL;
		newPtr->data = data;
		*rootPtr = *newPtr;
	}
	return 0;
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
		Stack *stack = (Stack *) gcmalloc(sizeof(Stack));
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
				gcfree(helpPtr);
			}
		} while (*rootPtr != NULL || !stackEmpty(stack));
		gcfree(stack);
	}
}