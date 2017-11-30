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
 *
 * @param left
 * @param right
 * @param oper
 * @param Token
 * @return
 */
datatype getDatatype(datatype left, datatype right, string oper, token Token);


/**
 * Parses expression by precedence table
 */
void parseExpression(token *PreviousToken, ast_exp** expressionTree);


/**
 * Returns true if given symbol is relational operator, otherwise false
 * @param symbol
 * @return
 */
bool isRelationalOperator(precedStack symbol);


/**
 * Returns true if given symbol is operator, otherwise false
 * @param symbol
 * @return
 */
bool isOperator(precedStack symbol);


#endif

