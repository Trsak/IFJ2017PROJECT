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
 * Generates code to assign value for variable
 */
void varAssign(BinaryTreePtr var, ast_exp *expression);

#endif //IFJ2017PROJECT_GENERATOR_H
