/**
 * @file generator.h
 * @author Petr Sopf (xsopfp00)
 * @brief Generating IFJCode2017 from AST
 */

#ifndef IFJ2017PROJECT_GENERATOR_H
#define IFJ2017PROJECT_GENERATOR_H

#include "parser.h"

int currentRegister;

int currentHelpRegister;

char *frame;

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
void printStatement(ast_exp *expression);

/**
 * @return void
 *
 * Generates code to assign value for variable
 */
void varAssign(BinaryTreePtr var, ast_exp *expression);

char *generateSymbol(datatype type, char *value);

char *getRegister();

void generateBinaryExp(ast_exp *expression);

char *generateIntegerSymbol(int value);

char *getNextRegister(char *nextReg);

char *getHelpRegister();

char *generateFloatSymbol(double value);

void varDeclare(BinaryTreePtr var);

#endif //IFJ2017PROJECT_GENERATOR_H
