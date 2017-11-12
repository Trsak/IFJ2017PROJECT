/**
 * @file garbage_collector.h
 * @author Tomas Odehnal (xodehn08)
 * @brief Automatic memory management.
 */

#ifndef IFJ2017_GARBAGE_COLLECTOR_H
#define IFJ2017_GARBAGE_COLLECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "error_codes.h"
//#include "symtable.h"

/**
 * Structure of one allocated item (pointer).
 */
struct AllocatingItem {
	void *ptr;
	struct AllocatingItem *next;
};

/**
 * Structure of global variable containing pointer to all structures in project.
 */
struct GCGlobal {
	struct AllocatingItem *firstItem; /* First allocated item in allocation list */
	struct AllocatingItem *lastItem; /* Last allocated item in allocation list */

	//BinaryTreePtr *binTree;// Pointer to Binary Tree (symtable)
	//Stack *stack;// Pointer to Stack

};

/**
 * Global variable of garbage collector.
 * !You need to include "garbage_collector.h"!
 */
extern struct GCGlobal gcGlobal;

/**
 * @brief Handle signal (SIGINT, SIGTERM) and exit program (on exit memory is cleared).
 *
 * @param signal Code of signal.
 */
void signalHandler(int signal);

/**
 * @brief Function for memory allocation and add pointer into allocation list.
 * Work same as malloc.
 *
 * @param size Number of bytes to be allocated.
 * @return Return pointer to allocated memory, or NULL if fail.
 */
void *gcmalloc(unsigned size);

/**
 * @brief Function for memory reallocation and remove old pointer.
 * Work same as realloc.
 *
 * @param ptr Pointer to currently allocated memory.
 * @param size Number of bytes to be allocated.
 * @return Return pointer allocated memory, or NULL if fail.
 */
void *gcrealloc(void *ptr, unsigned size);

/**
 * @brief Free allocated memory and remove pointer from allocation list.
 * Work same as free.
 *
 * @param ptr Pointer to currently allocated memory.
 */
void gcfree(void *ptr);

/**
 * @brief Initialize all pointers to default values in global variable.
 */
void gcInit();

/**
 * @brief Free all allocated blocks in memory.
 */
void gcFreeAll();

#endif //IFJ2017_GARBAGE_COLLECTOR_H
