/**
 * @file parser.h
 * @author Roman Bartl (xbartl06)
 * @brief  Parsing all blocks, statements and expressions (.h file)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include "scanner.h"
#include "error_codes.h"
#include "garbage_collector.h"


int tree[200];          // array for debugging TODO delete after

bool inFunction;        // flag for checking if parser is in body of function (for return statement)
bool unaryOp;           // flag for checking if is possible to do unary operation (for declaration is flag set on false)
token PreviousToken;    // global variable where is stored last token for check



/**
 *
 * @return
 */
void program();


/**
 *
 * @return
 */
void functions();


/**
 *
 * @return
 */
void mainBody();


/**
 *
 * @return
 */
void mainBodyIt();


/**
 *
 * @return
 */
void statement();


/**
 *
 * @return
 */
void statementNext();


/**
 *
 * @param lexem
 * @return
 */
bool functionFirst(int lexem);


/**
 *
 * @return
 */
void functionHeader();


/**
 *
 * @return
 */
void asDataType();


/**
 *
 * @return
 */
void functionNext();


/**
 *
 * @return
 */
void functionEnd();


/**
 *
 * @return
 */
void declareParams();


/**
 *
 * @return
 */
void declareParamsNext();


/**
 *
 * @return
 */
void dataType();


/**
 *
 * @param token
 * @return
 */
bool statementFirst(token Token);


/**
 *
 * @return
 */
void printNext();


/**
 *
 * @return
 */
void end(token Token);


/**
 *
 * @return
 */
void eol(token Token);


/**
 *
 * @return
 */
void expression();


/**
 *
 * @return
 */
void assignment();


/**
 *
 * @param Token
 * @return
 */
bool unaryOperation(token Token);


/**
 *
 * @param token
 * @return
 */
bool expressionFirst(token Token);


/**
 *
 * @return
 */
bool params();


/**
 *
 * @return
 */
bool paramsNext();


/**
 *
 * @return
 */
void ifNext();


/**
 *
 * @return
 */
void elseIf();