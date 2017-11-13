/**
 * @file stack.h
 * @author Jan Bartosek (xbarto92)
 * @brief Operations with Dynamic Stack
 */

#ifndef IFJ2017_STACK_H
#define IFJ2017_STACK_H

typedef struct {
	void* pointer;
	int currentSize;
	int top;
} Stack;

/**
 * @param S - this is a stack to operate with.
 * @return - returns ERROR_INTERNAL if there was a memory error, 0 if not
 *
 * Initialises the stack before the first use
 */
int stackInit(Stack *S);

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
void stackPush(Stack *S, void* ptr);

/**
 * @param S - this is a stack to operate with.
 * @return - Returns the pointer that is on the top of the stack.
 *
 * Function also deletes the returned pointer from the stack.
 */
void* stackPop(Stack *S);

#endif //IFJ2017_STACK_H