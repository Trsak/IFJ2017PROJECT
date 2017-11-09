/**
 * @file symtable.c
 * @author Jan Bartosek (xbarto92)
 * @brief
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "symtable.h"

void stackInit(PointerStack *S) {
	S->top = 0;
}

bool stackEmpty(PointerStack *S) {
	return (S->top == 0);
}

void stackPush(PointerStack *S, BinaryTreePtr ptr) {
	if (S->top == MAXSTACK)
		//TODO
		return;
		//printf("Chyba: Došlo k přetečení zásobníku s ukazateli!\n");
	else {
		S->top++;
		S->a[S->top] = ptr;
	}
}

BinaryTreePtr stackPop(PointerStack *S) {
	if (S->top == 0) {
		//TODO
		//printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		return (NULL);
	} else {
		return (S->a[S->top--]);
	}
}

void treeInsert(BinaryTreePtr rootPtr, struct Values data) {

	int compare;
	bool equal = false;

	while (rootPtr != NULL){
		compare = strcmp(rootPtr->data.name, data.name);
		if (compare == 0){
			rootPtr->data.value = data.value;
			equal = true;
			break;
		} else if (compare < 0){
			*rootPtr = *rootPtr->LPtr;
		} else {
			*rootPtr = *rootPtr->RPtr;
		}
	}
	if (!equal){
		BinaryTreePtr newPtr = (BinaryTreePtr) malloc(sizeof(struct BinaryTree));
		//TODO check malloc
		newPtr->LPtr = NULL;
		newPtr->RPtr = NULL;
		newPtr->data = data;
		*rootPtr = *newPtr;
	}
}

void DisposeTree(BinaryTreePtr *rootPtr) {
	if (*rootPtr != NULL) {

		PointerStack *stack = (PointerStack *) malloc(sizeof(PointerStack));
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