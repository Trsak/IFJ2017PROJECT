/**
 * @file garbage_collector.c
 * @author Tomas Odehnal (xodehn08)
 */

#include "garbage_collector.h"

struct GCGlobal gcGlobal;


//void signalHandler(int signal) {
void signalHandler() {
	exit(ERROR_INTERNAL);
}


void *gcmalloc(unsigned size) {
	if(gcGlobal.firstItem->ptr == NULL && gcGlobal.firstItem == gcGlobal.lastItem) {
		gcGlobal.lastItem->ptr = malloc(size);
		gcGlobal.lastItem->next = NULL;
	}
	else {
		gcGlobal.lastItem->next = malloc(sizeof(struct AllocatingItem));
		gcGlobal.lastItem = gcGlobal.lastItem->next;
		gcGlobal.lastItem->ptr = malloc(size);
		gcGlobal.lastItem->next = NULL;
	}

	return gcGlobal.lastItem->ptr;
}


void *gcrealloc(void *ptr, unsigned size) {
	struct AllocatingItem *temp = gcGlobal.firstItem;

	while(temp->ptr != NULL) {
		if(temp->ptr == ptr) {
			temp->ptr = realloc(temp->ptr, size);
			return temp->ptr;
		}
		temp = temp->next;
	}
	return NULL;
}


void gcfree(void *ptr) {
	struct AllocatingItem *temp = gcGlobal.firstItem;
	struct AllocatingItem *tempNext = gcGlobal.firstItem;

	while(tempNext != NULL) {
		if(tempNext->ptr == ptr) {
			if(temp == tempNext && tempNext->next != NULL) {
				gcGlobal.firstItem = temp->next;
			}
			else if(temp != tempNext && tempNext->next != NULL) {
				temp->next = tempNext->next;
			}

			if(tempNext->next == NULL) {
				gcGlobal.lastItem = temp;
				temp->next = NULL;
			}

			if(temp == tempNext && tempNext->next == NULL) {
				temp->ptr = NULL;
			}
			else {
				free(tempNext);
			}
			break;
		}
		else {
			temp = tempNext;
			tempNext = tempNext->next;
		}
	}
	free(ptr);
	ptr = NULL;
}


void gcInit() {
	gcGlobal.firstItem = malloc(sizeof(struct AllocatingItem));
	gcGlobal.firstItem->ptr = NULL;
	gcGlobal.firstItem->next = NULL;
	gcGlobal.lastItem = gcGlobal.firstItem;

	//TODO: stackInit(gcGlobal.stack);

	//TODO: treeInit(gcGlobal.binTree);

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	atexit(gcFreeAll);
}


void gcFreeAll() {
	struct AllocatingItem *temp = gcGlobal.firstItem;

	while(temp != NULL) {
		struct AllocatingItem *tempToFree = temp;
		free(temp->ptr);
		temp = temp->next;
		free(tempToFree);
	}
}

/*
void main() {
	gcInit();
	char *string = (char *) gcmalloc(5 * sizeof(char));
	int *integer = (int *) gcmalloc(sizeof(int));
	char *string2 = (char *) gcmalloc(485 * sizeof(char));

	integer = gcrealloc(integer, sizeof(int));

	gcfree(string);

	*integer = 42;

	printf("%d\n", *integer);
	for (int i = 0; i < 1000000000; ++i) {
		NULL;
	}
}*/