/**
 * @file precedence.h
 * @author Roman Bartl (xbartl06)
 * @brief Precedence table for syntax analysis of expressions
 */

#ifndef IFJ2017PROJECT_PRECEDENCE_H
#define IFJ2017PROJECT_PRECEDENCE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h"
#include "symtable.h"
#include "ast.h"
#include "stack.h"
#include "parser.h"


/**
 * Returns position in precedence table of given lexem from Token
 *
 * @param lexem
 * @return
 */
int getPositionInTable(int lexem);


/**
 * Parses expression by precedence table
 */
void parseExpression();

#endif

