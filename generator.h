/**
 * @file generator.h
 * @author Petr Sopf (xsopfp00)
 * @brief Generating IFJCode2017 from AST
 */

#ifndef IFJ2017PROJECT_GENERATOR_H
#define IFJ2017PROJECT_GENERATOR_H

#include "parser.h"

/**
 * @return void
 *
 * Starts code generating
 */
void startGenerating();

/**
 * @return void
 *
 * Generates code to print given expression
 */
void printExpression(ast_exp *expression);

/**
 * @return void
 *
 * Calculates binary expression and prints it
 */
void printBinaryExpression(ast_exp *expression);

/**
 * @return void
 *
 * Generates code to assign value for variable
 */
void varAssign(BinaryTreePtr var, ast_exp *expression);

/**
 * @return integer
 *
 * Gets integer result for expression
 */
int getIntegerExpressionValue(ast_exp *expression);

/**
 * @return double
 *
 * Gets float result for expression
 */
double getFloatExpressionValue(ast_exp *expression);

/**
 * @return string
 *
 * Gets string result for expression
 */
char *getStringExpressionValue(ast_exp *expression);

#endif //IFJ2017PROJECT_GENERATOR_H
