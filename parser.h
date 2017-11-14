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
int returnError;        // global variable where is stored error code
token PreviousToken;    // global variable where is stored last token for check


/**
 *
 * @return
 */
int parse();


/**
 *
 * @return
 */
bool program();


/**
 *
 * @return
 */
bool functions();


/**
 *
 * @return
 */
bool mainBody();


/**
 *
 * @return
 */
bool mainBodyIt();


/**
 *
 * @return
 */
bool statement();


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
bool functionHeader();


/**
 *
 * @return
 */
bool asDataType();


/**
 *
 * @return
 */
bool functionNext();


/**
 *
 * @return
 */
bool functionEnd();


/**
 *
 * @return
 */
bool declareParams();


/**
 *
 * @return
 */
bool declareParamsNext();


/**
 *
 * @return
 */
bool dataType();


/**
 *
 * @return
 */
bool statement();


/**
 *
 * @return
 */
bool statementNext();


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
bool printNext();


/**
 *
 * @return
 */
bool end(token Token);


/**
 *
 * @return
 */
bool eol(token Token);


/**
 *
 * @return
 */
bool expression();


/**
 *
 * @return
 */
bool assignment();


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
bool ifNext();


/**
 *
 * @return
 */
bool elseIf();