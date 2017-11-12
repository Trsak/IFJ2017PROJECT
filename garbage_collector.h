/**
 * @file garbage_collector.h
 * @author Tomas Odehnal (xodehn08)
 * @brief Automatic memory management.
 */

#ifndef IFJ2017_GARBAGE_COLLECTOR_H
#define IFJ2017_GARBAGE_COLLECTOR_H

#include <stdio.h>
#include <stdlib.h>
//#include "symtable.h"

struct AllocatingItem {
	void *ptr;
	struct AllocatingItem *next;
};

struct GCGlobal {
	struct AllocatingItem *firstItem; /* First allocated item in allocation list */
	struct AllocatingItem *lastItem; /* Last allocated item in allocation list */

	//BinaryTreePtr *binTree;// Pointer to Binary Tree (symtable)
	//Stack *stack;// Pointer to Stack

};

extern struct GCGlobal gcGlobal;

void *gcmalloc(unsigned size);
void *gcrealloc(void *ptr, unsigned size);
void gcfree(void *ptr);

void gcInit();
void gcFreeAll();

#endif //IFJ2017_GARBAGE_COLLECTOR_H
