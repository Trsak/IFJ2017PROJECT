/**
 * @project IFJ17
 * @file symtable.h
 * @author Jan Bartosek (xbarto92)
 * @author Tomas Odehnal (xodehn08)
 * @brief Symbol Table is used to store every identifier or number and it's value, datatype, etc..
 */

#ifndef IFJ2017_SYMTABLE_H
#define IFJ2017_SYMTABLE_H

#include "strings.h"
#include "garbage_collector.h"

typedef enum {
	TYPE_NUMBER = 1,
	TYPE_DECIMAL = 2,
	TYPE_STRING = 3
} datatype;

typedef struct Values {
	const char *name;
	string value;
	datatype type;
	bool declared;
	bool defined;
	bool hasReturn;
	bool isFunction;
	int paramNumber;
	bool isUsed;
	datatype *typeOfParams;
	struct BinaryTree *treeOfFunction;	/** NULL if isFunction == False, else pointer to subtree of symtable (tree) **/
} Values;

typedef struct BinaryTree {
	struct Values data;
	struct BinaryTree *LPtr;
	struct BinaryTree *RPtr;
} *BinaryTreePtr;

/**
 * @brief Set initial values to members of structure Values.
 *
 * @param name Name of identifier (function or variable).
 * @return Structure Values.
 */
Values initValues(const char *name);

/**
 * @brief Initialization of Binary Tree (symbolic table).
 * @param rootPtr Pointer to Binary Tree root node.
 */
void btInit(BinaryTreePtr *rootPtr);

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


#endif //IFJ2017_SYMTABLE_H