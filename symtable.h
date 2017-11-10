/**
 * @file symtable.h
 * @author Jan Bartosek (xbarto92)
 * @brief Symbol Table is used to store every identifier or number and it's value, datatype, etc..
 */

#ifndef IFJ2017_SYMTABLE_H
#define IFJ2017_SYMTABLE_H

#include "strings.h"

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
	//bool defined;
} Values;

typedef struct BinaryTree {
	struct Values data;
	struct BinaryTree *LPtr;
	struct BinaryTree *RPtr;
} *BinaryTreePtr;

typedef struct {
	BinaryTreePtr *pointer;
	int currentSize;
	int top;
} Stack;



/**
 * @param S - this is a stack to operate with.
 *
 * Initialises the stack before the first use
 */
void stackInit(Stack *S);

/**
 * @param S - this is a stack to operate with.
 *
 * Destroys the whole stack
 */
void stackDestroy(Stack *S);

/**
 * @param S - this is a stack to operate with.
 *
 * Resize the stack size if the current stack reach it's maximum.
 * The new stack size is always double as the previous one.
 */
void stackResize(Stack *S);

/**
 * @param S - this is a stack to operate with.
 * @return - true if stack is empty, false if it's not.
 */
bool stackEmpty(Stack *S);

/**
 * @param S - this is a stack to operate with.
 * @return - true if stack is full, false if it's not.
 */
bool stackFull(Stack *S);

/**
 * @param S - this is a stack to operate with.
 * @param ptr - pointer to save on the top of the stack.
 *
 * Saves new pointer on the top of the stack.
 * If the stack size is already full, resizes the stack by calling stackResize function and then saves the pointer.
 */
void stackPush(Stack *S, BinaryTreePtr ptr);

/**
 * @param S - this is a stack to operate with.
 * @return - Returns the pointer to a Binary Tree that is on the top of the stack.
 *
 * Function also deletes the returned pointer from the stack.
 */
BinaryTreePtr stackPop(Stack *S);

/**
 * @param rootPtr - pointer to Binary Tree root.
 * @param data - data ready for insert.
 *
 * Inserts the data into the Binary Tree based on the value of the name of variable.
 * If the value is lower, the data are going left.
 * If the value is higher, the data are going right.
 * If the same variable name is already in the Binary Tree, only it's value is changed.
 */
void treeInsert(BinaryTreePtr rootPtr, struct Values data);

/**
 * @param rootPtr - pointer to Binary Tree root.
 * @param name - name of the variable to get the data from.
 * @return - Values structure if the correct variable name was found, NULL if not.
 *
 * Goes through the Binary Tree until it find variable of the same name as received.
 * If the variable is not found NULL is returned.
 */
struct Values *getVariable(BinaryTreePtr rootPtr, const char *name);

/**
 * @param rootPtr - pointer to Binary Tree root.
 *
 * Destroys the whole tree and all the sub-roots.
 */
void DisposeTree(BinaryTreePtr *rootPtr);

#endif //IFJ2017_SYMTABLE_H
