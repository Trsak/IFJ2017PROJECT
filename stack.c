/**
 * @file stack.c
 * @author Jan Bartosek (xbarto92)
 * @author Roman Bartl (xbartl06)
 * @brief Operations with Dynamic Stack
 */


#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include "error_codes.h"
#include "garbage_collector.h"

/**
 * @copydoc stackInit
 */
void stackInit(Stack *S) {
    S->item = gcmalloc(MAX_VALUE * sizeof(struct stackItem));
    S->top = -1;
    S->maxTerm = -1;
    S->size = MAX_VALUE;
}


/**
 * @copydoc stackDestroy
 */
void stackDestroy(Stack *S) {
	gcfree(S->item);
    S->maxTerm = -1;
	S->top = -1;
    S->size = 0;
}


/**
 * @copydoc stackResize
 */
void stackResize(Stack *S) {
    //TODO - maybe here will be a mistake .. but for now it seems it's ok
	S->item = gcrealloc(S->item, 2 * S->size * sizeof(struct stackItem));
}


/**
 * @copydoc stackEmpty
 */
bool stackEmpty(Stack *S) {
    if (S->top < 0)
        return true;

    return false;
}

/**
 * @copydoc stackFull
 */
bool stackFull(Stack *S) {
    if (S->top == S->size - 1)
        return true;

    return false;
}

/**
 * @copydoc stackPush
 */
void stackPush(Stack *S, BinaryTreePtr *ptr, ast_exp *Exp, token *Token, precedStack symbol, ast_stmt* stmt) {
    if (stackFull(S)) {
        stackResize(S);
    }

    S->top++;

    S->item[S->top].Exp = Exp;
    S->item[S->top].ptr = ptr;
    S->item[S->top].symbol = symbol;
    S->item[S->top].Token = Token;
    S->item[S->top].stmt = stmt;
}

/**
 * @copydoc stackPop
 */
void stackPop(Stack *S) {
    if (!stackEmpty(S)) {
        S->top--;
    }
}


void stackTop(Stack *S, stackItem *item) {
    if (stackEmpty(S)) {
        return ;
    }

    *item = S->item[S->top];
}


void stackTopTerminal(Stack *S, stackItem *item) {
    if (stackEmpty(S)) {
        return ;
    }

    *item = S->item[S->maxTerm];
}