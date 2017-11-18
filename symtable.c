/**
 * @file symtable.c
 * @author Jan Bartosek (xbarto92)
 * @author Tomas Odehnal (xodehn08)
 * @brief Operations with Binary Tree
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "symtable.h"
#include "stack.h"
#include "error_codes.h"

/**
 * @copydoc
 */
Values initValues(const char *name) {
	Values data;
	data.name = name;
	data.declared = false;
	data.defined = false;
	data.isFunction = false;
	btInit(&data.treeOfFunction);
	return data;
}

/**
 * @copydoc
 */
void btInit(BinaryTreePtr *rootPtr) {
	*rootPtr = NULL;
}

/**
 * @copydoc btInsert
 */
void btInsert(BinaryTreePtr *rootPtr, struct Values data) {
	int compare = 0;
	bool equal = false;
    BinaryTreePtr tmpRootPtr = *rootPtr;
    BinaryTreePtr tmpPtr = NULL;

	while (!equal && tmpRootPtr != NULL) {
		compare = strcmp(tmpRootPtr->data.name, data.name);
        tmpPtr = tmpRootPtr;

		if (compare == 0) {
            tmpRootPtr->data.value = data.value;
			equal = true;

		} else if (compare < 0) {
			tmpRootPtr = tmpRootPtr->LPtr;

		} else {
            tmpRootPtr = tmpRootPtr->RPtr;
		}
	}

	if (!equal) {
		BinaryTreePtr newPtr = (BinaryTreePtr) gcmalloc(sizeof(struct BinaryTree));
		if (newPtr == NULL) {
			printErrAndExit (ERROR_INTERNAL, "There is a memory error while allocating tree structure.");
		}
		newPtr->LPtr = NULL;
		newPtr->RPtr = NULL;
		newPtr->data = data;

        if (tmpPtr == NULL) {
            *rootPtr = newPtr;

        } else {
            if (compare < 0) {
                tmpPtr->LPtr = newPtr;

            } else {
                tmpPtr->RPtr = newPtr;
            }
        }

	}
}

/**
 * @copydoc btGetVariable
 */
BinaryTreePtr btGetVariable(BinaryTreePtr rootPtr, const char *name) {
	int compare;
	while (rootPtr != NULL) {
		compare = strcmp(rootPtr->data.name, name);
		if (compare == 0) {
			return rootPtr;
		} else if (compare < 0) {
			rootPtr = rootPtr->LPtr;
		} else {
			rootPtr = rootPtr->RPtr;
		}
	}
	return NULL;
}
