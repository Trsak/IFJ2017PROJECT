/**
 * @file symtable.h
 * @author Jan Bartosek (xbarto92)
 * @brief Symbol Table is used to store every identifier or number and it's value, datatype, etc..
 */

#ifndef IFJ2017_SYMTABLE_H
#define IFJ2017_SYMTABLE_H

#include "strings.h"

#define MAXSTACK 30

typedef enum {
	STRING,
	INTEGER,
	FLOAT,
	DOUBLE,
	BOOL,
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

#endif //IFJ2017_SYMTABLE_H
