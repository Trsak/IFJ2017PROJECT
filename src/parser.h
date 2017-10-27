/**
 * @file parser.h
 * @author Roman Bartl (xbartl06)
 * @brief  TODO
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include "scanner.h"
#include "error_codes.h"


bool program();

bool functions();

bool mainBody();

bool mainBodyIt();

bool statement();

bool functionFollow(int token);

bool functionFirst(int token);

bool functionHeader();

bool functionIt();

bool functionItFirst(int token);

bool functionAsFirst(int token);

bool functionAs();

bool functionNext();

bool functionEnd();

bool declareParams();

bool declareParamsNext();

bool declareParamsFollow(int token);

bool dataType();

bool statement();

bool mainBody();

bool mainBodyIt();

bool end();