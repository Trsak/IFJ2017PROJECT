/**
 * @file stack.h
 * @author Jan Bartosek (xbarto92)
 * @brief Operations with Dynamic Stack
 */

#include "ast.h"
#include "scanner.h"

#ifndef IFJ2017_STACK_H
#define IFJ2017_STACK_H

#define MAX_VALUE 18

typedef enum {
	PREC_MULTIPLY = 0,
	PREC_DIVISION,
	PREC_BACKSLASH,
	PREC_PLUS,
	PREC_MINUS,
	PREC_ASSIGNMENT,
	PREC_NOT_EQUAL,
	PREC_LESS,
	PREC_LESS_EQUAL,
	PREC_GREATER,
	PREC_GREATER_EQUAL,
	PREC_BRACKET_LEFT,
	PREC_BRACKET_RIGHT,
	PREC_ID,
	PREC_NUMBER,
	PREC_DECIMAL_NUMBER,
	PREC_STRING_EXPRESSION,
	PREC_DOLLAR,                // special symbol '$' in precedence table
	PREC_E,                     // special symbol 'E' in precedence table
	PREC_LT                     // special symbol '<' in precedence table
} precedStack;


typedef struct stackItem {
    BinaryTreePtr *ptr;
    ast_exp *Exp;
    token *Token;
    precedStack symbol;
} stackItem;


typedef struct {
    stackItem *item;
	int size;
	int top;
    int maxTerm;
} Stack;

/**
 * @param S - this is a stack to operate with.
 * @return - returns ERROR_INTERNAL if there was a memory error, 0 if not
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
void stackPush(Stack *S, BinaryTreePtr *ptr, ast_exp *Exp, token *Token, precedStack symbol);

/**
 * @param S - this is a stack to operate with.
 * @return - Returns the pointer that is on the top of the stack.
 *
 * Function also deletes the returned pointer from the stack.
 */
void stackPop(Stack *S);


void stackTop(Stack *S, stackItem *item);

void stackTopTerminal(Stack *S, stackItem *item);

#endif //IFJ2017_STACK_H