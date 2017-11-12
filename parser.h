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


int tree[200];          // array for debugging TODO delete after

bool inFunction;        // flag for checking if parser is in body of function (for return statement)
int previousToken;      // global variable where is stored last token for check
int returnError;        // global variable where is stored error code


/**
 *
 * @return
 */
int parse();


/**
 *
 * @return
 */
bool nextToken(int *returnToken);


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
 * @param token
 * @return
 */
bool functionFirst(int token);


/**
 *
 * @return
 */
bool functionHeader();


/**
 *
 * @return
 */
bool functionAs();


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
bool statementFirst(int token);


/**
 *
 * @return
 */
bool printNext();


/**
 *
 * @return
 */
bool end(int token);


/**
 *
 * @return
 */
bool eol(int token);


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
 * @param token
 * @return
 */
bool expressionFirst(int token);


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


/**
 *
 * @return
 */
bool elseIfNext();