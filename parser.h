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

int tree[200];
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
bool functionFollow(int token);


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
bool functionIt();


/**
 *
 * @param token
 * @return
 */
bool functionItFirst(int token);


/**
 *
 * @param token
 * @return
 */
bool functionAsFirst(int token);


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
 * @param token
 * @return
 */
bool declareParamsFollow(int token);


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
bool end();


/**
 *
 * @return
 */
bool expression();


/**
 *
 * @return
 */
bool varDeclaration();


/**
 *
 * @return
 */
bool assignment();


/**
 *
 * @return
 */
bool assignmentMark();


/**
 *
 * @param token
 * @return
 */
bool assignmentMarkFirst(int token);


/**
 *
 * @param token
 * @return
 */
bool statementFollow(int token);


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
 * @param token
 * @return
 */
bool paramsFollow(int token);


/**
 *
 * @return
 */
bool ifNext();


/**
 *
 * @return
 */
bool ifNextFollow();


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