/**
 * @file symtable.h
 * @author Jan Bartosek (xbarto92)
 * @brief Symbol Table is used to store every identifier or number and it's value, datatype, etc..
 */

#ifndef IFJ2017_SYMTABLE_H
#define IFJ2017_SYMTABLE_H

#include "strings.h"
#include "garbage_collector.h"

typedef enum {
	TYPE_STRING,
	TYPE_INTEGER,
	TYPE_FLOAT,
	TYPE_DOUBLE,
	TYPE_BOOL,
} datatype;

typedef struct Values {
	const char *name;
	string value;
	datatype type;
	bool declared;
	bool defined;
} Values;

typedef struct BinaryTree {
	struct Values data;
	struct BinaryTree *LPtr;
	struct BinaryTree *RPtr;
} *BinaryTreePtr;


/**
 * @param rootPtr - pointer to Binary Tree root.
 * @param data - data ready for insert.
 *
 * Inserts the data into the Binary Tree based on the value of the name of variable.
 * If the value is lower, the data are going left.
 * If the value is higher, the data are going right.
 * If the same variable name is already in the Binary Tree, only it's value is changed.
 */
void btInsert(BinaryTreePtr *rootPtr, struct Values data);

/**
 * @param rootPtr - pointer to Binary Tree root.
 * @param name - name of the variable to get the data from.
 * @return - Values structure if the correct variable name was found, NULL if not.
 *
 * Goes through the Binary Tree until it find variable of the same name as received.
 * If the variable is not found NULL is returned.
 */
BinaryTreePtr btGetVariable(BinaryTreePtr rootPtr, const char *name);

/**
 * @param rootPtr - pointer to Binary Tree root.
 *
 * Destroys the whole tree and all the sub-roots.
 */
void btDispose(BinaryTreePtr *rootPtr);

#endif //IFJ2017_SYMTABLE_H